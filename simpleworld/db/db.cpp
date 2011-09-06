/**
 * @file simpleworld/db/db.cpp
 * Simple World database.
 *
 *  Copyright (C) 2006-2011  Xosé Otero <xoseotero@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <boost/format.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
namespace fs = boost::filesystem;

#include "exception.hpp"
#include "wrongversion.hpp"
#include "db.hpp"
#include "transaction.hpp"
#include "default.hpp"
#include "environment.hpp"

#define DATABASE_VERSION 5

namespace simpleworld
{
namespace db
{

/**
 * Create the tables.
 * @param db database connection.
 * @exception DBException The tables can't be created.
 */
static void create_tables(DB* db)
{
  const char* sql_commands[] = {
    /*******************
     * Environment
     */
    "\
CREATE TABLE Environment\n\
(\n\
  id INTEGER NOT NULL,\n\
\n\
  time INTEGER NOT NULL,\n\
\n\
  -- the rest of the rows shouldn't change\n\
  size_x INTEGER NOT NULL,\n\
  size_y INTEGER NOT NULL,\n\
\n\
time_rot INTEGER NOT NULL,\n\
size_rot INTEGER NOT NULL,\n\
\n\
  mutations_probability REAL NOT NULL,  -- Values from 0 to 1\n\
  time_birth INTEGER NOT NULL,\n\
  time_mutate INTEGER NOT NULL,\n\
\n\
  time_laziness INTEGER NOT NULL,\n\
  energy_laziness INTEGER NOT NULL,\n\
\n\
  attack_multiplier REAL NOT NULL,\n\
\n\
  time_nothing INTEGER NOT NULL,\n\
  time_myself INTEGER NOT NULL,\n\
  time_detect INTEGER NOT NULL,\n\
  time_info INTEGER NOT NULL,\n\
  time_move INTEGER NOT NULL,\n\
  time_turn INTEGER NOT NULL,\n\
  time_attack INTEGER NOT NULL,\n\
  time_eat INTEGER NOT NULL,\n\
  time_egg INTEGER NOT NULL,\n\
  energy_nothing INTEGER NOT NULL,\n\
  energy_myself INTEGER NOT NULL,\n\
  energy_detect INTEGER NOT NULL,\n\
  energy_info INTEGER NOT NULL,\n\
  energy_move INTEGER NOT NULL,\n\
  energy_turn INTEGER NOT NULL,\n\
  energy_attack INTEGER NOT NULL,\n\
  energy_eat INTEGER NOT NULL,\n\
  energy_egg INTEGER NOT NULL,\n\
\n\
  PRIMARY KEY(id),\n\
  CHECK(time >= 0),\n\
  CHECK(size_x > 0 AND size_y > 0),\n\
  CHECK(time_rot >= 0),\n\
  CHECK(size_rot >= 0),\n\
  CHECK(mutations_probability >= 0 AND mutations_probability <= 1),\n\
  CHECK(time_birth >= 0),\n\
  CHECK(time_mutate >= 0),\n\
  CHECK(time_laziness >= 0),\n\
  CHECK(energy_laziness >= 0),\n\
  CHECK(time_nothing >= 0 AND time_myself >= 0 AND\n\
        time_detect >= 0 AND time_info >= 0 AND\n\
        time_move >= 0 AND time_turn >= 0 AND\n\
        time_attack >= 0 AND time_eat >= 0 AND\n\
        time_egg >= 0),\n\
  CHECK(energy_nothing >= 0 AND energy_myself >= 0 AND\n\
        energy_detect >= 0 AND energy_info >= 0 AND\n\
        energy_move >= 0 AND energy_turn >= 0 AND\n\
        energy_attack >= 0 AND energy_eat >= 0 AND\n\
        energy_egg >= 0)\n\
);",

    "\
CREATE INDEX Environment_index ON Environment(time);",

    /* time must be the older of the table */
    "\
CREATE TRIGGER Environment_insert_time\n\
BEFORE INSERT\n\
ON Environment\n\
FOR EACH ROW\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'There is a older time')\n\
  WHERE (SELECT max(time)\n\
         FROM Environment) > NEW.time;\n\
END;",

    "\
CREATE TRIGGER Environment_update_time\n\
BEFORE UPDATE OF time\n\
ON Environment\n\
FOR EACH ROW\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'There is a older time')\n\
  WHERE (SELECT max(time)\n\
         FROM Environment) > NEW.time;\n\
END;",

    /* The size of the World can't change */
    "\
CREATE TRIGGER Environment_insert\n\
BEFORE INSERT\n\
ON Environment\n\
FOR EACH ROW\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'The size of the World can''t change')\n\
  WHERE (SELECT count(id)\n\
         FROM Environment) > 0\n\
        AND\n\
        (SELECT id\n\
         FROM Environment\n\
         WHERE size_x=NEW.size_x AND size_y=NEW.size_y) IS NULL;\n\
END;",


    /*******************
     * World
     */
    "\
CREATE TABLE World\n\
(\n\
  id INTEGER NOT NULL,\n\
\n\
  position_x INTEGER NOT NULL,\n\
  position_y INTEGER NOT NULL,\n\
  orientation INTEGER,\n\
\n\
  PRIMARY KEY(id),\n\
  UNIQUE(position_x, position_y),\n\
  CHECK(position_x >= 0 AND position_y >= 0),\n\
  CHECK(orientation IS NULL OR\n\
        (orientation >= 0 AND orientation <= 3)) -- 4 possible orientations\n\
);",

    /* positions must be less than the size of the world */
    "\
CREATE TRIGGER World_insert\n\
BEFORE INSERT\n\
ON World\n\
FOR EACH ROW\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'position_x is out of the World')\n\
  WHERE (SELECT size_x\n\
         FROM Environment\n\
         WHERE time=(SELECT max(time)\n\
                     FROM Environment)) <= NEW.position_x;\n\
  SELECT RAISE(ROLLBACK, 'position_y is out of the World')\n\
  WHERE (SELECT size_y\n\
         FROM Environment\n\
         WHERE time=(SELECT max(time)\n\
                     FROM Environment)) <= NEW.position_y;\n\
END;",

    "\
CREATE TRIGGER World_update_position_x\n\
BEFORE UPDATE OF position_x\n\
ON World\n\
FOR EACH ROW\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'position_x is out of the World')\n\
  WHERE (SELECT size_x\n\
         FROM Environment\n\
         WHERE time=(SELECT max(time)\n\
                     FROM Environment)) <= NEW.position_x;\n\
END;",

    "\
CREATE TRIGGER World_update_position_y\n\
BEFORE UPDATE OF position_y\n\
ON World\n\
FOR EACH ROW\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'position_y is out of the World')\n\
  WHERE (SELECT size_y\n\
         FROM Environment\n\
         WHERE time=(SELECT max(time)\n\
                     FROM Environment)) <= NEW.position_y;\n\
END;",


    /*******************
     * Spawn
     */
    "\
CREATE TABLE Spawn(\n\
  id INTEGER NOT NULL,\n\
\n\
  frequency INTGER NOT NULL,\n\
\n\
  start_x INTEGER NOT NULL,\n\
  start_y INTEGER NOT NULL,\n\
  end_x INTEGER NOT NULL,\n\
  end_y INTEGER NOT NULL,\n\
\n\
  max INTEGER NOT NULL,\n\
  energy INTENER NOT NULL,\n\
  code BLOB NOT NULL,\n\
\n\
  PRIMARY KEY(id)\n\
  CHECK(start_x >= 0),\n\
  CHECK(start_y >= 0),\n\
  CHECK(end_x > start_x),\n\
  CHECK(end_y > start_y),\n\
  CHECK(max <= (end_x - start_x) * (end_y - start_y)),\n\
  CHECK(energy > 0),\n\
  CHECK(length(code) >= 0 AND (length(code) % 4 = 0))\n \
);",

    /* regions must be inside the world */
    "\
CREATE TRIGGER Spawn_insert\n\
BEFORE INSERT\n\
ON Spawn\n\
FOR EACH ROW\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'start_x is out of the World')\n\
  WHERE (SELECT size_x\n\
         FROM Environment\n\
         WHERE time=(SELECT max(time)\n\
                     FROM Environment)) <= NEW.start_x;\n\
  SELECT RAISE(ROLLBACK, 'start_y is out of the World')\n\
  WHERE (SELECT size_y\n\
         FROM Environment\n\
         WHERE time=(SELECT max(time)\n\
                     FROM Environment)) <= NEW.start_y;\n\
  SELECT RAISE(ROLLBACK, 'end_x is out of the World')\n\
  WHERE (SELECT size_x\n\
         FROM Environment\n\
         WHERE time=(SELECT max(time)\n\
                     FROM Environment)) < NEW.end_x;\n\
  SELECT RAISE(ROLLBACK, 'end_y is out of the World')\n\
  WHERE (SELECT size_y\n\
         FROM Environment\n\
         WHERE time=(SELECT max(time)\n\
                     FROM Environment)) < NEW.end_y;\n\
END;",

    "\
CREATE TRIGGER Spawn_update_start_x\n\
BEFORE UPDATE OF start_x\n\
ON Spawn\n\
FOR EACH ROW\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'start_x is out of the World')\n\
  WHERE (SELECT size_x\n\
         FROM Environment\n\
         WHERE time=(SELECT max(time)\n\
                     FROM Environment)) <= NEW.start_x;\n\
END;",

    "\
CREATE TRIGGER Spawn_update_start_y\n\
BEFORE UPDATE OF start_y\n\
ON Spawn\n\
FOR EACH ROW\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'start_y is out of the World')\n\
  WHERE (SELECT size_y\n\
         FROM Environment\n\
         WHERE time=(SELECT max(time)\n\
                     FROM Environment)) <= NEW.start_y;\n\
END;",

    "\
CREATE TRIGGER Spawn_update_end_x\n\
BEFORE UPDATE OF end_x\n\
ON Spawn\n\
FOR EACH ROW\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'end_x is out of the World')\n\
  WHERE (SELECT size_x\n\
         FROM Environment\n\
         WHERE time=(SELECT max(time)\n\
                     FROM Environment)) < NEW.end_x;\n\
END;",

    "\
CREATE TRIGGER Spawn_update_end_y\n\
BEFORE UPDATE OF end_y\n\
ON Spawn\n\
FOR EACH ROW\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'end_y is out of the World')\n\
  WHERE (SELECT size_y\n\
         FROM Environment\n\
         WHERE time=(SELECT max(time)\n\
                     FROM Environment)) < NEW.end_y;\n\
END;",


    /*******************
     * Resource
     */
    "\
CREATE TABLE Resource(\n\
  id INTEGER NOT NULL,\n\
\n\
  frequency INTGER NOT NULL,\n\
\n\
  max INTEGER NOT NULL,\n\
  start_x INTEGER NOT NULL,\n\
  start_y INTEGER NOT NULL,\n\
  end_x INTEGER NOT NULL,\n\
  end_y INTEGER NOT NULL,\n\
\n\
  size INTENER NOT NULL,\n\
\n\
  PRIMARY KEY(id)\n\
  CHECK(frequency > 0),\n\
  CHECK(start_x >= 0),\n\
  CHECK(start_y >= 0),\n\
  CHECK(end_x > start_x),\n\
  CHECK(end_y > start_y),\n\
  CHECK(max <= (end_x - start_x) * (end_y - start_y)),\n\
  CHECK(size > 0)\n\
);",

    /* regions must be inside the world */
    "\
CREATE TRIGGER Resource_insert\n\
BEFORE INSERT\n\
ON Resource\n\
FOR EACH ROW\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'start_x is out of the World')\n\
  WHERE (SELECT size_x\n\
         FROM Environment\n\
         WHERE time=(SELECT max(time)\n\
                     FROM Environment)) <= NEW.start_x;\n\
  SELECT RAISE(ROLLBACK, 'start_y is out of the World')\n\
  WHERE (SELECT size_y\n\
         FROM Environment\n\
         WHERE time=(SELECT max(time)\n\
                     FROM Environment)) <= NEW.start_y;\n\
  SELECT RAISE(ROLLBACK, 'end_x is out of the World')\n\
  WHERE (SELECT size_x\n\
         FROM Environment\n\
         WHERE time=(SELECT max(time)\n\
                     FROM Environment)) < NEW.end_x;\n\
  SELECT RAISE(ROLLBACK, 'end_y is out of the World')\n\
  WHERE (SELECT size_y\n\
         FROM Environment\n\
         WHERE time=(SELECT max(time)\n\
                     FROM Environment)) < NEW.end_y;\n\
END;",

    "\
CREATE TRIGGER Resource_update_start_x\n\
BEFORE UPDATE OF start_x\n\
ON Resource\n\
FOR EACH ROW\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'start_x is out of the World')\n\
  WHERE (SELECT size_x\n\
         FROM Environment\n\
         WHERE time=(SELECT max(time)\n\
                     FROM Environment)) <= NEW.start_x;\n\
END;",

    "\
CREATE TRIGGER Resource_update_start_y\n\
BEFORE UPDATE OF start_y\n\
ON Resource\n\
FOR EACH ROW\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'start_y is out of the World')\n\
  WHERE (SELECT size_y\n\
         FROM Environment\n\
         WHERE time=(SELECT max(time)\n\
                     FROM Environment)) <= NEW.start_y;\n\
END;",

    "\
CREATE TRIGGER Resource_update_end_x\n\
BEFORE UPDATE OF end_x\n\
ON Resource\n\
FOR EACH ROW\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'end_x is out of the World')\n\
  WHERE (SELECT size_x\n\
         FROM Environment\n\
         WHERE time=(SELECT max(time)\n\
                     FROM Environment)) < NEW.end_x;\n\
END;",

    "\
CREATE TRIGGER Resource_update_end_y\n\
BEFORE UPDATE OF end_y\n\
ON Resource\n\
FOR EACH ROW\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'end_y is out of the World')\n\
  WHERE (SELECT size_y\n\
         FROM Environment\n\
         WHERE time=(SELECT max(time)\n\
                     FROM Environment)) < NEW.end_y;\n\
END;",


    /*******************
     * Bug
     */
    "\
CREATE TABLE Bug\n\
(\n\
  id INTEGER NOT NULL,\n\
\n\
  creation INTEGER NOT NULL,\n\
  father_id INTEGER,                    -- NULL if the bug is added manually\n\
\n\
  /* The blob is the last col for performance */\n\
  code BLOB NOT NULL,\n\
\n\
  PRIMARY KEY(id),\n\
  FOREIGN KEY(father_id) REFERENCES Bug(id) ON UPDATE CASCADE ON DELETE SET NULL,\n\
  CHECK(creation >= 0),\n\
  CHECK(father_id),\n\
  CHECK(length(code) >= 0 AND (length(code) % 4 = 0))\n\
);",

    "\
CREATE INDEX Bug_index ON Bug(father_id);",

    /* creation must be the current time */
    "\
CREATE TRIGGER Bug_insert_creation\n\
BEFORE INSERT\n\
ON Bug\n\
FOR EACH ROW\n\
BEGIN\n\
SELECT RAISE(ROLLBACK, 'creation is not now')\n\
WHERE (SELECT max(time)\n\
FROM Environment) <> NEW.creation;\n\
END;",


    /*******************
     * Egg
     */
    "\
CREATE TABLE Egg\n\
(\n\
  bug_id INTEGER NOT NULL,\n\
\n\
  world_id INTEGER NOT NULL,\n\
  energy INTEGER NOT NULL,\n\
\n\
  PRIMARY KEY(bug_id),\n\
  FOREIGN KEY(bug_id) REFERENCES Bug(id) ON UPDATE CASCADE ON DELETE CASCADE,\n\
  FOREIGN KEY(world_id) REFERENCES World(id) ON UPDATE CASCADE ON DELETE CASCADE,\n\
  UNIQUE(world_id),\n\
  CHECK(energy > 0)\n\
);",


    /*******************
     * Alive bug
     */
    "\
CREATE TABLE AliveBug\n\
(\n\
  bug_id INTEGER NOT NULL,\n\
\n\
  world_id INTEGER NOT NULL,\n\
  birth INTEGER NOT NULL,\n\
  energy INTEGER NOT NULL,\n\
\n\
  time_last_action INTEGER,             -- NULL if no action was done yet\n\
  action_time INTEGER,                  -- NULL if the bug is not doing a\n\
                                        -- action\n\
\n\
  /* The blob is the last col for performance */\n\
  registers BLOB NOT NULL,\n\
\n\
  PRIMARY KEY(bug_id),\n\
  FOREIGN KEY(bug_id) REFERENCES Bug(id) ON UPDATE CASCADE ON DELETE CASCADE,\n\
  FOREIGN KEY(world_id) REFERENCES World(id) ON UPDATE CASCADE ON DELETE CASCADE,\n\
  UNIQUE(world_id),\n\
  CHECK(time_last_action IS NULL OR time_last_action >= 0),\n\
  CHECK(action_time IS NULL OR action_time > 0),\n\
  CHECK(birth >= 0),\n\
  CHECK(energy > 0)\n\
);",

    /* Check that the time_last_action isn't in the future */
    "\
CREATE TRIGGER AliveBug_insert_time_last_action\n\
BEFORE INSERT\n\
ON AliveBug\n\
FOR EACH ROW\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'The time_last_action is in the future')\n\
  WHERE (SELECT max(time)\n\
         FROM Environment)\n\
        < NEW.time_last_action;\n\
END;",

    "\
CREATE TRIGGER AliveBug_update_time_last_action\n\
BEFORE UPDATE OF time_last_action\n\
ON AliveBug\n\
FOR EACH ROW\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'The time_last_action is in the future')\n\
  WHERE (SELECT max(time)\n\
         FROM Environment)\n\
        < NEW.time_last_action;\n\
END;",

    /* Check that the action_time isn't in the past */
    "\
CREATE TRIGGER AliveBug_insert_action_time\n\
BEFORE INSERT\n\
ON AliveBug\n\
FOR EACH ROW\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'The action_time is in the past')\n\
  WHERE (SELECT max(time)\n\
         FROM Environment)\n\
        > NEW.action_time;\n\
END;",

    "\
CREATE TRIGGER AliveBug_update_action_time\n\
BEFORE UPDATE OF action_time\n\
ON AliveBug\n\
FOR EACH ROW\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'The action_time is in the past')\n\
  WHERE (SELECT max(time)\n\
         FROM Environment)\n\
        > NEW.action_time;\n\
END;",


    /*******************
     * Dead bug
     */
    "\
CREATE TABLE DeadBug\n\
(\n\
  bug_id INTEGER NOT NULL,\n\
\n\
  birth INTEGER,                        -- NULL if it was a egg when it died\n\
  death INTEGER NOT NULL,\n\
\n\
  killer_id INTEGER,                    -- NULL if the bug wasted his energy\n\
\n\
  PRIMARY KEY(bug_id),\n\
  FOREIGN KEY(bug_id) REFERENCES Bug(id) ON UPDATE CASCADE ON DELETE CASCADE,\n\
  FOREIGN KEY(killer_id) REFERENCES Bug(id) ON UPDATE CASCADE ON DELETE SET NULL,\n\
  CHECK(birth IS NULL OR birth >= 0),\n\
  CHECK(birth IS NULL OR birth <= death),\n\
  CHECK(bug_id <> killer_id)\n\
);",

    "\
CREATE INDEX DeadBug_index ON DeadBug(killer_id);",


    /*******************
     * Mutation
     */
    "\
CREATE TABLE Mutation\n\
(\n\
  id INTEGER NOT NULL,\n\
\n\
  bug_id INTEGER NOT NULL,\n\
\n\
  time INTEGER NOT NULL,\n\
\n\
  /* type is 0 for mutation, 1 for partial mutation, 2 for permutation,\n\
     3 for addition, 4 for duplication and 5 for deletion */\n\
  type INTEGER NOT NULL,\n\
  position INTEGER NOT NULL,\n\
  original INTEGER,                     -- NULL if new code was added\n\
  mutated INTEGER,                      -- NULL if code was deleted\n\
\n\
  PRIMARY KEY(id),\n\
  FOREIGN KEY(bug_id) REFERENCES Bug(id) ON UPDATE CASCADE ON DELETE CASCADE,\n\
  CHECK(time >= 0),\n\
  CHECK(position >= 0),\n\
  CHECK(type >= 0 AND type <= 5),\n\
  CHECK((type == 0 AND original IS NOT NULL AND mutated IS NOT NULL) OR\n\
        (type == 1 AND original IS NOT NULL AND mutated IS NOT NULL) OR\n\
        (type == 2 AND original IS NOT NULL AND mutated IS NOT NULL) OR\n\
        (type == 3 AND original IS NULL AND mutated IS NOT NULL) OR\n\
        (type == 4 AND original IS NULL AND mutated IS NOT NULL) OR\n\
        (type == 5 AND original IS NOT NULL AND mutated IS NULL))\n\
);",

    "\
CREATE INDEX Mutation_index ON Mutation(bug_id);",


    /*******************
     * Food
     */
    "\
CREATE TABLE Food\n\
(\n\
  id INTEGER NOT NULL,\n\
\n\
  time INTEGER NOT NULL,\n\
  world_id INTEGER NOT NULL,\n\
  size INTEGER NOT NULL,\n\
\n\
  PRIMARY KEY(id),\n\
  FOREIGN KEY(world_id) REFERENCES World(id) ON UPDATE CASCADE ON DELETE CASCADE,\n\
  UNIQUE(world_id),\n\
  CHECK(time >= 0),\n\
  CHECK(size >= 0)\n\
);",


    /*******************
     * Stats
     */
    "\
CREATE TABLE Stats\n\
(\n\
  id INTEGER NOT NULL,\n\
\n\
  time INTEGER NOT NULL,\n\
\n\
  families INTEGER NO NULL,\n\
  alive INTEGER NOT NULL,\n\
  eggs INTEGER NOT NULL,\n\
  food INTEGER NOT NULL,\n\
  energy INTEGER NOT NULL,\n\
  mutations INTEGER NOT NULL,\n\
  age INTEGER NOT NULL,\n\
\n\
  last_births INTEGER NOT NULL,\n\
  last_sons INTEGER NOT NULL,\n\
  last_deaths INTEGER NOT NULL,\n\
  last_kills INTEGER NOT NULL,\n\
  last_mutations INTEGER NOT NULL,\n\
\n\
  PRIMARY KEY(id),\n\
  CHECK(time >= 0),\n\
  CHECK(families >= 0),\n\
  CHECK(alive >= 0),\n\
  CHECK(eggs >= 0),\n\
  CHECK(food >= 0),\n\
  CHECK(energy >= 0),\n\
  CHECK(mutations >= 0),\n\
  CHECK(age >= 0),\n\
  CHECK(last_births >= 0),\n\
  CHECK(last_sons >= 0),\n\
  CHECK(last_deaths >= 0),\n\
  CHECK(last_kills >= 0),\n\
  CHECK(last_mutations >= 0)\n\
);",


    NULL
  };

 Transaction transaction(db, Transaction::immediate);
 char* errmsg;
 for (int i = 0; sql_commands[i] != NULL; i++)
   if (sqlite3_exec(db->db(), sql_commands[i], NULL, NULL, &errmsg)) {
     std::string error(errmsg);
     sqlite3_free(errmsg);
     throw EXCEPTION(DBException, error);
   }
 transaction.commit();
}


/**
 * Constructor.
 * @param filename File name of the database.
 * @exception DBException if the database can't be opened.
 * @exception DBException if there is a error in the database.
 * @exception WrongVersion if the database version is not supported.
 */
DB::DB(std::string filename)
{
  if (sqlite3_open_v2(filename.c_str(), &this->db_, SQLITE_OPEN_READWRITE,
                      NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_));

  // user_version is used for know if the database was new (user_version is 0
  // by default) and the version of the database (user_version is set to
  // DATABASE_VERSION) to check if the database is compatible
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db(), "PRAGMA user_version;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db()));
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db()));
  this->version_ = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);
  if (this->version_ != DATABASE_VERSION)
    throw EXCEPTION(WrongVersion, boost::str(boost::format("\
Database version %1% not supported")
                                             % static_cast<int>(this->version_)));

  // From http://sqlite.org/c3ref/step.html:
  // SQLITE_BUSY means that the database engine was unable to acquire the
  // database locks it needs to do its job. If the statement is a COMMIT or
  // occurs outside of an explicit transaction, then you can retry the
  // statement. If the statement is not a COMMIT and occurs within a explicit
  // transaction then you should rollback the transaction before continuing.
  //
  // It's seems easier to set a big busy timeout and treat SQLITE_BUSY as an
  // error than trying to handle it.
  sqlite3_busy_timeout(this->db_, 60000);

  sqlite3_exec(this->db_, "PRAGMA journal_mode = WAL;", NULL, NULL, NULL);
  sqlite3_exec(this->db_, "PRAGMA foreign_keys = ON;", NULL, NULL, NULL);
}

/**
 * Destructor.
 */
DB::~DB()
{
  sqlite3_close(this->db_);
}


/**
* Create the database with the default Environment.
* @param filename File name of the database.
* @exception DBException if there is an error with the creation.
*/
void DB::create(std::string filename)
{
  DB::create(filename, 0,
             default_environment.size_x,
             default_environment.size_y,
             default_environment.time_rot,
             default_environment.size_rot,
             default_environment.mutations_probability,
             default_environment.time_birth,
             default_environment.time_mutate,
             default_environment.time_laziness,
             default_environment.energy_laziness,
             default_environment.attack_multiplier,
             default_environment.time_nothing,
             default_environment.time_myself,
             default_environment.time_detect,
             default_environment.time_info,
             default_environment.time_move,
             default_environment.time_turn,
             default_environment.time_attack,
             default_environment.time_eat,
             default_environment.time_egg,
             default_environment.energy_nothing,
             default_environment.energy_myself,
             default_environment.energy_detect,
             default_environment.energy_info,
             default_environment.energy_move,
             default_environment.energy_turn,
             default_environment.energy_attack,
             default_environment.energy_eat,
             default_environment.energy_egg);
}

/**
 * Create the database.
 * @param filename File name of the database.
 * @param time time passed since the creation of the World.
 * @param size_x size of the World (x coord).
 * @param size_y size of the World (y coord).
 * @param time_rot time needed to rot the food.
 * @param size_rot size that is substracted to the food.
 * @param mutations_probability probability (0.0-1.0) that the code mutates.
 * @param time_birth time needed to convert a egg into a bug.
 * @param time_mutate time to mutate the code of a old bug.
 * @param time_laziness time without doing a action to consider a bug lazy.
 * @param energy_laziness energy substracted for laziness.
 * @param attack_multiplier multiplier for the energy of a attack.
 * @param time_nothing time needed to do the action nothing.
 * @param time_myself time needed to do the action myself.
 * @param time_detect time needed to do the action detect.
 * @param time_info time needed to do the action info.
 * @param time_move time needed to do the action move.
 * @param time_turn time needed to do the action turn.
 * @param time_attack time needed to do the action attack.
 * @param time_eat time needed to do the action eat.
 * @param time_egg time needed to do the action egg.
 * @param energy_nothing energy used to do the action nothing.
 * @param energy_myself energy used to do the action myself.
 * @param energy_detect energy used to do the action detect.
 * @param energy_info energy used to do the action info.
 * @param energy_move energy used to do the action move.
 * @param energy_turn energy used to do the action turn.
 * @param energy_attack energy used to do the action attack.
 * @param energy_eat energy used to do the action eat.
 * @param energy_egg energy used to do egg 
 * @exception DBException if there is an error with the creation.
 */
void DB::create(std::string filename,
                Time time, Coord size_x, Coord size_y,
                Time time_rot, Energy size_rot,
                double mutations_probability, Time time_birth,
                Time time_mutate, Time time_laziness,
                Energy energy_laziness, double attack_multiplier,
                Time time_nothing, Time time_myself, Time time_detect,
                Time time_info, Time time_move, Time time_turn,
                Time time_attack, Time time_eat, Time time_egg,
                Energy energy_nothing, Energy energy_myself,
                Energy energy_detect, Energy energy_info,
                Energy energy_move, Energy energy_turn,
                Energy energy_attack, Energy energy_eat,
                Energy energy_egg)
{
  if (fs::exists(fs::path(filename, fs::native)))
    throw EXCEPTION(DBException,
                    boost::str(boost::format("File %1% already exists")
                               % filename));

  sqlite3* handler;
  if (sqlite3_open_v2(filename.c_str(), &handler,
                      SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(handler));
  sqlite3_exec(handler, boost::str(boost::format("PRAGMA user_version = %1%;")
                                   % DATABASE_VERSION).c_str(), NULL, NULL, NULL);
  sqlite3_close(handler);

  DB db(filename);
  create_tables(&db);
  Environment::insert(&db, time, size_x, size_y, time_rot, size_rot,
                      mutations_probability, time_birth, time_mutate,
                      time_laziness, energy_laziness, attack_multiplier,
                      time_nothing, time_myself, time_detect, time_info,
                      time_move, time_turn, time_attack, time_eat, time_egg,
                      energy_nothing, energy_myself, energy_detect,
                      energy_info, energy_move, energy_turn, energy_attack,
                      energy_eat, energy_egg);
}


/**
 * List of all the environments (changes), ordered by it's time.
 * @return the list of environments.
 * @exception DBException if there is a error in the database.
 */
std::vector<ID> DB::environments()
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db(), "\
SELECT id\n\
FROM Environment\n\
ORDER BY time;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db()));

  bool done = false;
  std::vector<ID> ids;
  while (not done)
    switch (sqlite3_step(stmt)) {
    case SQLITE_DONE:
      done = true;
      break;
    case SQLITE_ROW:
      ids.push_back(sqlite3_column_int64(stmt, 0));
      break;
    default:
      throw EXCEPTION(DBException, sqlite3_errmsg(this->db()));
    }

  sqlite3_finalize(stmt);

  return ids;
}

/**
 * Get the last environment (change).
 * @return the environment.
 * @exception DBException if there is a error in the database.
 * @exception DBException if there isn't any environments.
 */
ID DB::last_environment()
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db(), "\
SELECT id\n\
FROM Environment\n\
WHERE time = (SELECT max(time)\n\
              FROM Environment)\n\
ORDER BY id DESC\n\
LIMIT 1;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db()));
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, "Table Environment is empty");
  ID id = sqlite3_column_int64(stmt, 0);
  sqlite3_finalize(stmt);

  return id;
}


/**
 * List of spawns.
 * @return the list of spawns.
 * @exception DBException if there is a error in the database.
 */
std::vector<ID> DB::spawns()
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db(), "\
SELECT id\n\
FROM Spawn;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db()));

  bool done = false;
  std::vector<ID> ids;
  while (not done)
    switch (sqlite3_step(stmt)) {
    case SQLITE_DONE:
      done = true;
      break;
    case SQLITE_ROW:
      ids.push_back(sqlite3_column_int64(stmt, 0));
      break;
    default:
      throw EXCEPTION(DBException, sqlite3_errmsg(this->db()));
    }

  sqlite3_finalize(stmt);

  return ids;
}

/**
 * List of resources.
 * @return the list of resources.
 * @exception DBException if there is a error in the database.
 */
std::vector<ID> DB::resources()
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db(), "\
SELECT id\n\
FROM Resource;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db()));

  bool done = false;
  std::vector<ID> ids;
  while (not done)
    switch (sqlite3_step(stmt)) {
    case SQLITE_DONE:
      done = true;
      break;
    case SQLITE_ROW:
      ids.push_back(sqlite3_column_int64(stmt, 0));
      break;
    default:
      throw EXCEPTION(DBException, sqlite3_errmsg(this->db()));
    }

  sqlite3_finalize(stmt);

  return ids;
}


/**
 * List of eggs, ordered by its conception time.
 * @return the list of eggs.
 * @exception DBException if there is a error in the database.
 */
std::vector<ID> DB::eggs()
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db(), "\
SELECT bug_id\n\
FROM Egg\n\
ORDER BY bug_id;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db()));

  bool done = false;
  std::vector<ID> ids;
  while (not done)
    switch (sqlite3_step(stmt)) {
    case SQLITE_DONE:
      done = true;
      break;
    case SQLITE_ROW:
      ids.push_back(sqlite3_column_int64(stmt, 0));
      break;
    default:
      throw EXCEPTION(DBException, sqlite3_errmsg(this->db()));
    }

  sqlite3_finalize(stmt);

  return ids;
}

/**
 * List of the alive bugs, ordered by its birth.
 * @return the list of bugs.
 * @exception DBException if there is a error in the database.
 */
std::vector<ID> DB::alive_bugs()
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db(), "\
SELECT bug_id\n\
FROM AliveBug\n\
ORDER BY birth, bug_id;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db()));

  bool done = false;
  std::vector<ID> ids;
  while (not done)
    switch (sqlite3_step(stmt)) {
    case SQLITE_DONE:
      done = true;
      break;
    case SQLITE_ROW:
      ids.push_back(sqlite3_column_int64(stmt, 0));
      break;
    default:
      throw EXCEPTION(DBException, sqlite3_errmsg(this->db()));
    }

  sqlite3_finalize(stmt);

  return ids;
}

/**
 * List of the dead bugs, ordered by its death.
 * @return the list of bugs.
 * @exception DBException if there is a error in the database.
 */
std::vector<ID> DB::dead_bugs()
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db(), "\
SELECT bug_id\n\
FROM DeadBug\n\
ORDER BY death, bug_id;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db()));

  bool done = false;
  std::vector<ID> ids;
  while (not done)
    switch (sqlite3_step(stmt)) {
    case SQLITE_DONE:
      done = true;
      break;
    case SQLITE_ROW:
      ids.push_back(sqlite3_column_int64(stmt, 0));
      break;
    default:
      throw EXCEPTION(DBException, sqlite3_errmsg(this->db()));
    }

  sqlite3_finalize(stmt);

  return ids;
}


/**
 * List of the food.
 * @return the list of food.
 * @exception DBException if there is a error in the database.
 */
std::vector<ID> DB::food()
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db(), "\
SELECT id\n\
FROM Food\n\
ORDER BY id;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db()));

  bool done = false;
  std::vector<ID> ids;
  while (not done)
    switch (sqlite3_step(stmt)) {
    case SQLITE_DONE:
      done = true;
      break;
    case SQLITE_ROW:
      ids.push_back(sqlite3_column_int64(stmt, 0));
      break;
    default:
      throw EXCEPTION(DBException, sqlite3_errmsg(this->db()));
    }

  sqlite3_finalize(stmt);

  return ids;
}


/**
 * List of the stats.
 * @return the list of stats.
 * @exception DBException if there is a error in the database.
 */
std::vector<ID> DB::stats()
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db(), "\
SELECT id\n\
FROM Stats\n\
ORDER BY id;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db()));

  bool done = false;
  std::vector<ID> ids;
  while (not done)
    switch (sqlite3_step(stmt)) {
    case SQLITE_DONE:
      done = true;
      break;
    case SQLITE_ROW:
      ids.push_back(sqlite3_column_int64(stmt, 0));
      break;
    default:
      throw EXCEPTION(DBException, sqlite3_errmsg(this->db()));
    }

  sqlite3_finalize(stmt);

  return ids;
}

}
}
