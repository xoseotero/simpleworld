/**
 * @file simpleworld/db/db.cpp
 * Simple World database.
 *
 *  Copyright (C) 2006-2010  Xosé Otero <xoseotero@gmail.com>
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
#include "environment.hpp"

#define DATABASE_VERSION 2

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
  mutations_probability REAL NOT NULL,  -- Values from 0 to 1\n\
  time_birth INTEGER NOT NULL,\n\
  time_mutate INTEGER NOT NULL,\n\
\n\
  time_laziness INTEGER NOT NULL,\n\
  energy_laziness INTEGER NOT NULL,\n\
\n\
  attack_multiplier REAL NOT NULL,\n\
\n\
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
  CHECK(mutations_probability >= 0 AND mutations_probability <= 1),\n\
  CHECK(time_birth >= 0),\n\
  CHECK(time_mutate >= 0),\n\
  CHECK(time_laziness >= 0),\n\
  CHECK(energy_laziness >= 0),\n\
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
BEFORE UPDATE\n\
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
CREATE TRIGGER World_insert_position_x\n\
BEFORE INSERT\n\
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
CREATE TRIGGER World_insert_position_y\n\
BEFORE INSERT\n\
ON World\n\
FOR EACH ROW\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'position_y is out of the World')\n\
  WHERE (SELECT size_x\n\
         FROM Environment\n\
         WHERE time=(SELECT max(time)\n\
                     FROM Environment)) <= NEW.position_x;\n\
END;",

    "\
CREATE TRIGGER World_update_position_x\n\
BEFORE UPDATE\n\
ON World\n\
FOR EACH ROW\n\
WHEN OLD.position_x <> NEW.position_x\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'position_x is out of the World')\n\
  WHERE (SELECT size_x\n\
         FROM Environment\n\
         WHERE time=(SELECT max(time)\n\
                     FROM Environment)) <= NEW.position_x;\n\
END;",

    "\
CREATE TRIGGER World_update_position_y\n\
BEFORE UPDATE\n\
ON World\n\
FOR EACH ROW\n\
WHEN OLD.position_y <> NEW.position_y\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'position_y is out of the World')\n\
  WHERE (SELECT size_x\n\
         FROM Environment\n\
         WHERE time=(SELECT max(time)\n\
                     FROM Environment)) <= NEW.position_x;\n\
END;",


    /*******************
     * Bug
     */
    "\
CREATE TABLE Bug\n\
(\n\
  id INTEGER NOT NULL,\n\
\n\
  father_id INTEGER,                    -- NULL if the bug is added manually\n\
\n\
  /* The blob is the last col for performance */\n\
  code BLOB NOT NULL,\n\
\n\
  PRIMARY KEY(id),\n\
  FOREIGN KEY(father_id) REFERENCES Bug(id) ON UPDATE CASCADE ON DELETE SET NULL,\n\
  CHECK(father_id),\n\
  CHECK(length(code) >= 0 AND (length(code) % 4 = 0))\n\
);",

    "\
CREATE INDEX Bug_index ON Bug(father_id);",


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
  conception INTEGER NOT NULL,\n\
\n\
  PRIMARY KEY(bug_id),\n\
  FOREIGN KEY(bug_id) REFERENCES Bug(id) ON UPDATE CASCADE ON DELETE CASCADE,\n\
  FOREIGN KEY(world_id) REFERENCES World(id) ON UPDATE CASCADE ON DELETE CASCADE,\n\
  UNIQUE(world_id),\n\
  CHECK(energy > 0),\n\
  CHECK(conception >= 0)\n\
);",

    /* conception must be in the current time */
    "\
CREATE TRIGGER Egg_insert_conception\n\
BEFORE INSERT\n\
ON Egg\n\
FOR EACH ROW\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'conception is not now')\n\
  WHERE (SELECT max(time)\n\
         FROM Environment) <> NEW.conception;\n\
END;",


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
BEFORE UPDATE\n\
ON AliveBug\n\
FOR EACH ROW\n\
WHEN OLD.time_last_action <> NEW.time_last_action\n\
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
BEFORE UPDATE\n\
ON AliveBug\n\
FOR EACH ROW\n\
WHEN OLD.action_time <> NEW.action_time\n\
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
  position INTEGER NOT NULL,\n\
  /* original and mutated can't be NULL at same time */\n\
  original INTEGER,                     -- NULL if new code was added\n\
  mutated INTEGER,                      -- NULL if code was deleted\n\
\n\
  PRIMARY KEY(id),\n\
  FOREIGN KEY(bug_id) REFERENCES Bug(id) ON UPDATE CASCADE ON DELETE CASCADE,\n\
  CHECK(time >= 0),\n\
  CHECK(position >= 0),\n\
  CHECK((original IS NOT NULL) OR (mutated IS NOT NULL))\n\
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
  world_id INTEGER NOT NULL,\n\
  size INTEGER NOT NULL,\n\
\n\
  PRIMARY KEY(id),\n\
  FOREIGN KEY(world_id) REFERENCES World(id) ON UPDATE CASCADE ON DELETE CASCADE,\n\
  UNIQUE(world_id),\n\
  CHECK(size >= 0)\n\
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
					     % this->version_));

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
 * Create the database.
 * @param filename File name of the database.
 * @param time cycles since the creation of the World.
 * @param size_x size of the World (x coord).
 * @param size_y size of the World (y coord).
 * @param mutations_probability probability (0.0-1.0) that the code mutates.
 * @param time_birth cycles to convert a egg into a bug.
 * @param time_mutate cycles to mutate the code of a old bug.
 * @param time_laziness cycles without doing a action to consider a bug lazy.
 * @param energy_laziness energy substracted for laziness.
 * @param attack_multiplier multiplier for the energy of a attack.
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
		double mutations_probability, Time time_birth,
		Time time_mutate, Time time_laziness,
		Energy energy_laziness, double attack_multiplier,
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
  Environment::insert(&db, time, size_x, size_y, mutations_probability,
		      time_birth, time_mutate, time_laziness, energy_laziness,
		      attack_multiplier, energy_nothing, energy_myself,
		      energy_detect, energy_info, energy_move, energy_turn,
		      energy_attack, energy_eat, energy_egg);
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
ORDER BY conception, bug_id;", -1, &stmt, NULL))
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

}
}
