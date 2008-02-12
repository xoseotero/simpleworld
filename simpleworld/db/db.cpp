/**
 * @file simpleworld/db/db.cpp
 * Simple World database.
 *
 * begin:     Wed, 24 Jan 2007 06:23:10 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2006-2008  Xosé Otero <xoseotero@users.sourceforge.net>
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

#include "exception.hpp"
#include "wrongversion.hpp"
#include "db.hpp"

#define DATABASE_VERSION 2

namespace SimpleWorld
{
namespace DB
{

/**
 * Constructor.
 * @param filename File name of the database.
 * @exception DBException if the database can't be opened.
 * @exception DBException if there is a error in the database.
 * @exception WrongVersion if the database version is not supported.
 */
DB::DB(std::string filename)
  : sqlite3x::sqlite3_connection()
{
  try {
    this->open(filename);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, e.what());
  }

  this->setbusytimeout(60000);
}


/**
 * List of all the environments (changes), ordered by it's time.
 * @return the list of environments.
 * @exception DBException if there is a error in the database.
 */
std::vector<Time> DB::environments()
{
  std::vector<Time> ids;
  sqlite3x::sqlite3_command sql(*this);

  try {
    sql.prepare("\
SELECT id\n\
FROM Environment\n\
ORDER BY time;");
    sqlite3x::sqlite3_cursor cursor = sql.executecursor();
    while (cursor.step())
      ids.push_back(cursor.getint(0));
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, e.what());
  }

  return ids;
}

/**
 * Get the last environment (change).
 * @return the environment.
 * @exception DBException if there is a error in the database.
 * @exception DBException if there isn't any environments.
 */
Time DB::last_environment()
{
  sqlite3x::sqlite3_command sql(*this);

  try {
    sql.prepare("\
SELECT id\n\
FROM Environment\n\
WHERE time = (SELECT max(time)\n\
              FROM Environment)\n\
ORDER BY id DESC\n\
LIMIT 1;");
    sqlite3x::sqlite3_cursor cursor = sql.executecursor();
    if (cursor.step())
      return cursor.getint(0);
    else
      throw EXCEPTION(DBException, "Table Environment is empty");
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, e.what());
  }
}


/**
 * List of all the bugs, ordered by its birth.
 * @return the list of bugs.
 * @exception DBException if there is a error in the database.
 */
std::vector<ID> DB::bugs()
{
  std::vector<ID> ids;
  sqlite3x::sqlite3_command sql(*this);

  try {
    sql.prepare("\
SELECT id\n\
FROM Bug\n\
ORDER BY birth, id;");
    sqlite3x::sqlite3_cursor cursor = sql.executecursor();
    while (cursor.step())
      ids.push_back(cursor.getint64(0));
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, e.what());
  }

  return ids;
}

/**
 * List of the alive bugs, ordered by its birth.
 * @return the list of bugs.
 * @exception DBException if there is a error in the database.
 */
std::vector<ID> DB::alive_bugs()
{
  std::vector<ID> ids;
  sqlite3x::sqlite3_command sql(*this);

  try {
    sql.prepare("\
SELECT id\n\
FROM Bug_alive\n\
ORDER BY birth, id;");
    sqlite3x::sqlite3_cursor cursor = sql.executecursor();
    while (cursor.step())
      ids.push_back(cursor.getint64(0));
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, e.what());
  }

  return ids;
}

/**
 * List of eggs, ordered by its birth.
 * @return the list of eggs.
 * @exception DBException if there is a error in the database.
 */
std::vector<ID> DB::eggs()
{
  std::vector<ID> ids;
  sqlite3x::sqlite3_command sql(*this);

  try {
    sql.prepare("\
SELECT id\n\
FROM Bug_egg\n\
ORDER BY birth, id;");
    sqlite3x::sqlite3_cursor cursor = sql.executecursor();
    while (cursor.step())
      ids.push_back(cursor.getint64(0));
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, e.what());
  }

  return ids;
}


/**
 * List of the food.
 * @return the list of food.
 * @exception DBException if there is a error in the database.
 */
std::vector<ID> DB::food()
{
  std::vector<ID> ids;
  sqlite3x::sqlite3_command sql(*this);

  try {
    sql.prepare("\
SELECT id\n\
FROM Food\n\
ORDER BY id;");
    sqlite3x::sqlite3_cursor cursor = sql.executecursor();
    while (cursor.step())
      ids.push_back(cursor.getint64(0));
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, e.what());
  }

  return ids;
}


/**
 * This function is called when open() succeeds. Subclasses
 * which wish to do custom db initialization or sanity checks
 * may do them here.
 * @exception WrongVersion if the database version is not supported.
 */
void DB::on_open()
{
  sqlite3x::sqlite3_connection::on_open();

  // user_version is used for know if the database was new (user_version is 0
  // by default) and the version of the database (user_version is set to
  // DATABASE_VERSION) to check if the database is compatible
  this->version_ = this->executeint("PRAGMA user_version;");
  if (this->version_ == 0) {    // Data base was created now
    //sqlite3x::sqlite3_command sql(*this, "PRAGMA user_version = ?;");
    //sql.bind(1, static_cast<int>(DATABASE_VERSION));
    sqlite3x::sqlite3_command sql(*this, boost::str(boost::format("\
PRAGMA user_version = %1%;")
                                                    % DATABASE_VERSION));
    sql.executenonquery();
    this->version_ = DATABASE_VERSION;

    this->create_tables();
  } else if (this->version_ != DATABASE_VERSION)
    throw EXCEPTION(WrongVersion, boost::str(boost::format("\
Database version %1% not supported")
                                             % this->version_));
}

/**
 * Create the tables.
 * @exception DBException The tables can't be created.
 */
void DB::create_tables()
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
  -- the rest of the rows can't change\n\
  size_x INTEGER NOT NULL,\n\
  size_y INTEGER NOT NULL,\n\
\n\
  energy_developed INTEGER NOT NULL,\n\
\n\
  mutations_probability REAL NOT NULL,  -- Values from 0 to 1\n\
\n\
  time_birth INTEGER NOT NULL,\n\
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
  CHECK(energy_developed >= 0),\n\
  CHECK(mutations_probability >= 0 AND mutations_probability <= 1),\n\
  CHECK(time_birth >= 0),\n\
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
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'There is a older time')\n\
  WHERE (SELECT max(time)\n\
         FROM Environment) > NEW.time;\n\
END;",

    "\
CREATE TRIGGER Environment_update_time\n\
BEFORE UPDATE\n\
ON Environment\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'There is a older time')\n\
  WHERE (SELECT max(time)\n\
         FROM Environment) >= NEW.time;\n\
END;",

    /* The size of the World can't change */
    "\
CREATE TRIGGER Environment_insert\n\
BEFORE INSERT\n\
ON Environment\n\
FOR EACH ROW\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, \"The size of the World can't change\")\n\
  WHERE (SELECT count(id)\n\
         FROM Environment) > 0\n\
	      AND\n\
        (SELECT id\n\
         FROM Environment\n\
         WHERE size_x = NEW.size_x AND size_y = NEW.size_y) IS NULL;\n\
END;",

    /* Only time can change */
    "\
CREATE TRIGGER Environment_update\n\
BEFORE UPDATE\n\
ON Environment\n\
FOR EACH ROW\n\
WHEN OLD.size_x != NEW.size_x OR\n\
     OLD.size_y != NEW.size_y OR\n\
     OLD.energy_developed != NEW.energy_developed OR\n\
     OLD.mutations_probability != NEW.mutations_probability OR\n\
     OLD.time_birth != NEW.time_birth OR\n\
     OLD.energy_nothing != NEW.energy_nothing OR\n\
     OLD.energy_myself != NEW.energy_myself OR\n\
     OLD.energy_detect != NEW.energy_detect OR\n\
     OLD.energy_info != NEW.energy_info OR\n\
     OLD.energy_move != NEW.energy_move OR\n\
     OLD.energy_turn != NEW.energy_turn OR\n\
     OLD.energy_attack != NEW.energy_attack OR\n\
     OLD.energy_eat != NEW.energy_eat OR\n\
     OLD.energy_egg != NEW.energy_egg\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'changed invalid col');\n\
END;",


    /*******************
     * Bug
     */
    "\
CREATE TABLE Bug\n\
(\n\
  id INTEGER NOT NULL,\n\
\n\
  energy INTEGER NOT NULL,\n\
\n\
  position_x INTEGER NOT NULL,\n\
  position_y INTEGER NOT NULL,\n\
  orientation INTEGER NOT NULL,\n\
\n\
  action_time INTEGER,                  -- NULL if the bug is not doing a\n\
                                        -- action\n\
\n\
  birth INTEGER NOT NULL,\n\
  dead INTEGER,                         -- NULL if alive\n\
\n\
  father_id INTEGER,                    -- NULL if developed bug\n\
  killer_id INTEGER,                    -- NULL if alive\n\
\n\
  PRIMARY KEY(id),\n\
  FOREIGN KEY(killer_id) REFERENCES Bug(id),\n\
  FOREIGN KEY(father_id) REFERENCES Bug(id),\n\
  CHECK(position_x >= 0 AND position_y >= 0),\n\
  CHECK(orientation >= 0 AND orientation <= 3), -- 4 possible orientations\n\
  CHECK(action_time IS NULL OR action_time > 0),\n\
  CHECK(birth >= 0),\n\
  CHECK(dead IS NULL OR (birth <= dead)), -- If dead IS NOT NULL\n\
  CHECK(killer_id IS NULL OR (killer_id IS NOT NULL AND dead IS NOT NULL))\n\
);",

    "\
CREATE INDEX Bug_position_index ON Bug(position_x, position_y);",

    "\
CREATE INDEX Bug_dead_index ON Bug(dead);",

    "\
CREATE VIEW Bug_egg AS\n\
  SELECT id, energy, position_x, position_y, orientation,\n\
         birth, father_id\n\
  FROM Bug\n\
  WHERE dead IS NULL AND birth > (SELECT max(time)\n\
                                  FROM Environment);",

    "\
CREATE VIEW Bug_alive AS\n\
  SELECT id, energy, position_x, position_y, orientation,\n\
         birth, father_id\n\
  FROM Bug\n\
  WHERE dead IS NULL AND birth <= (SELECT max(time)\n\
                                   FROM Environment);",

    "\
CREATE VIEW Bug_dead AS\n\
  SELECT id, position_x, position_y, orientation,\n\
         birth, dead, father_id, killer_id\n\
  FROM Bug\n\
  WHERE dead IS NOT NULL;",

    /* Check if the id will change */
    "\
CREATE TRIGGER Bug_id_trigger\n\
BEFORE UPDATE\n\
ON Bug\n\
FOR EACH ROW\n\
WHEN OLD.id != NEW.id\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id changed');\n\
END;",

    /* Check if the bug is alive before update */
    "\
CREATE TRIGGER Bug_alive_trigger\n\
BEFORE UPDATE\n\
ON Bug\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'Bug dead')\n\
  WHERE (SELECT id\n\
         FROM Bug\n\
         WHERE id=NEW.id AND dead IS NULL)\n\
        IS NULL;\n\
END;",

    /* Check if father_id is a valid id */
    "\
CREATE TRIGGER Bug_insert_father_trigger\n\
BEFORE INSERT\n\
ON Bug\n\
FOR EACH ROW\n\
WHEN NEW.father_id IS NOT NULL\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'Father not found or dead')\n\
  WHERE (SELECT id\n\
         FROM Bug\n\
         WHERE id=NEW.father_id AND dead IS NULL) IS NULL;\n\
END;",

    "\
CREATE TRIGGER Bug_update_father_trigger\n\
BEFORE UPDATE\n\
ON Bug\n\
FOR EACH ROW\n\
WHEN OLD.father_id != NEW.father_id\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, \"Father can't change\");\n\
END;",

    /* Check if killer_id is a valid id */
    "\
CREATE TRIGGER Bug_insert_killer_trigger\n\
BEFORE INSERT\n\
ON Bug\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'Killer not found')\n\
  WHERE NEW.killer_id IS NOT NULL\n\
        AND\n\
        (SELECT id\n\
         FROM Bug\n\
         WHERE id=NEW.killer_id AND dead IS NULL) IS NULL;\n\
END;",

    "\
CREATE TRIGGER Bug_update_killer_trigger\n\
BEFORE UPDATE\n\
ON Bug\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'Killer not found')\n\
  WHERE NEW.killer_id IS NOT NULL\n\
        AND\n\
        (SELECT id\n\
         FROM Bug\n\
         WHERE id=NEW.killer_id AND dead IS NULL) IS NULL;\n\
END;",

    /* Check that two bugs are not in the same position */
    "\
CREATE TRIGGER Bug_insert_position_trigger\n\
BEFORE INSERT\n\
ON Bug\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'There is a bug in the same position')\n\
  WHERE (SELECT id\n\
         FROM Bug\n\
         WHERE dead IS NULL AND position_x=NEW.position_x AND position_y=NEW.position_y)\n\
        IS NOT NULL;\n\
END;",

    "\
CREATE TRIGGER Bug_update_position_trigger\n\
BEFORE UPDATE\n\
ON Bug\n\
FOR EACH ROW\n\
WHEN OLD.id != NEW.id\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'There is a bug in the same position')\n\
  WHERE (SELECT id\n\
         FROM Bug\n\
         WHERE dead IS NULL AND position_x=NEW.position_x AND position_y=NEW.position_y)\n\
        IS NOT NULL;\n\
END;",

    /* Check that the action_time isn't in the past */
    "\
CREATE TRIGGER Bug_insert_action_time_trigger\n\
BEFORE INSERT\n\
ON Bug\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'The action_time is in the past')\n\
  WHERE (SELECT max(time)\n\
         FROM Environment)\n\
        > NEW.action_time;\n\
END;",

    "\
CREATE TRIGGER Bug_update_action_time_trigger\n\
BEFORE UPDATE\n\
ON Bug\n\
FOR EACH ROW\n\
WHEN OLD.action_time != NEW.action_time\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'The action_time is in the past')\n\
  WHERE (SELECT max(time)\n\
         FROM Environment)\n\
	> NEW.action_time;\n\
END;",

    /* Propagate the dead */
    "\
CREATE TRIGGER Bug_update_dead_trigger\n\
BEFORE UPDATE\n\
ON Bug\n\
FOR EACH ROW\n\
WHEN OLD.dead IS NULL AND NEW.dead IS NOT NULL\n\
BEGIN\n\
  DELETE FROM CPU WHERE bug_id = OLD.id;\n\
END;",

    /* Propagate the deletion */
    "\
CREATE TRIGGER Bug_delete_trigger\n\
BEFORE DELETE\n\
ON Bug\n\
FOR EACH ROW BEGIN\n\
  DELETE FROM Code WHERE bug_id = OLD.id;\n\
  DELETE FROM CPU WHERE bug_id = OLD.id;\n\
END;",


    /*******************
     * Code
     */
    "\
CREATE TABLE Code\n\
(\n\
  bug_id INTEGER NOT NULL,\n\
\n\
  size INTEGER NOT NULL,\n\
  md5 CHAR[32] NOT NULL,\n\
\n\
  /* The blob is the last col for performance */\n\
  code BLOB NOT NULL,\n\
\n\
  PRIMARY KEY(bug_id),\n\
  FOREIGN KEY(bug_id) REFERENCES Bug(id),\n\
  CHECK(size >= 0 AND (size % 4 == 0)),\n\
  CHECK(length(md5) == 32),\n\
  CHECK(length(code) % 4 == 0),\n\
  CHECK(size == length(code))\n\
);",

    /* Check if the id will change */
    "\
CREATE TRIGGER Code_update_id_trigger\n\
BEFORE UPDATE\n\
ON Code\n\
FOR EACH ROW\n\
WHEN OLD.bug_id != NEW.bug_id\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id changed');\n\
END;",

    /* Make sure that the bug_id is valid */
    "\
CREATE TRIGGER Code_insert_valid_trigger\n\
BEFORE INSERT\n\
ON Code\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id of bug not found')\n\
  WHERE (SELECT id\n\
         FROM Bug\n\
         WHERE id=NEW.bug_id AND dead IS NULL) IS NULL;\n\
END;",

    "\
CREATE TRIGGER Code_update_valid_trigger\n\
BEFORE UPDATE\n\
ON Code\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id of bug not found')\n\
  WHERE (SELECT id\n\
         FROM Bug\n\
         WHERE id=NEW.bug_id AND dead IS NULL) IS NULL;\n\
END;",

    /* Make sure that the bug_id is unique */
    "\
CREATE TRIGGER Code_insert_uniq_trigger\n\
BEFORE INSERT\n\
ON Code\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id of bug already in table')\n\
  WHERE (SELECT count(bug_id)\n\
         FROM Code\n\
         WHERE bug_id=NEW.bug_id) != 0;\n\
END;",

    "\
CREATE TRIGGER Code_update_uniq_trigger\n\
BEFORE UPDATE\n\
ON Code\n\
FOR EACH ROW\n\
WHEN OLD.bug_id != NEW.bug_id\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id of bug already in table')\n\
  WHERE (SELECT count(bug_id)\n\
         FROM Code\n\
         WHERE bug_id=NEW.bug_id) != 0;\n\
END;",

    /* Propagate the deletion */
    "\
CREATE TRIGGER Code_delete_trigger\n\
BEFORE DELETE\n\
ON Code\n\
FOR EACH ROW BEGIN\n\
  DELETE FROM Mutation WHERE bug_id = OLD.id;\n\
END;",


    /*******************
     * CPU
     */
    "\
CREATE TABLE CPU\n\
(\n\
  bug_id INTEGER NOT NULL,\n\
\n\
  /* The blob is the last col for performance */\n\
  registers BLOB NOT NULL,\n\
\n\
  PRIMARY KEY(bug_id),\n\
  FOREIGN KEY(bug_id) REFERENCES Bug(id)\n\
);"

    /* Check if the id will change */
    "\
CREATE TRIGGER CPU_update_id_trigger\n\
BEFORE UPDATE\n\
ON CPU\n\
FOR EACH ROW\n\
WHEN OLD.bug_id != NEW.bug_id\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id changed');\n\
END;",

    /* Make sure that the bug_id is valid */
    "\
CREATE TRIGGER CPU_insert_valid_trigger\n\
BEFORE INSERT\n\
ON CPU\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id of bug not found')\n\
  WHERE (SELECT id\n\
         FROM Bug\n\
         WHERE id=NEW.bug_id AND dead IS NULL) IS NULL;\n\
END;",

    "\
CREATE TRIGGER CPU_update_valid_trigger\n\
BEFORE UPDATE\n\
ON CPU\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id of bug not found')\n\
  WHERE (SELECT id\n\
         FROM Bug\n\
         WHERE id=NEW.bug_id AND dead IS NULL) IS NULL;\n\
END;",

    /* Make sure that the bug_id is unique */
    "\
CREATE TRIGGER CPU_insert_uniq_trigger\n\
BEFORE INSERT\n\
ON CPU\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id of bug already in table')\n\
  WHERE (SELECT count(bug_id)\n\
         FROM CPU\n\
         WHERE bug_id=NEW.bug_id) != 0;\n\
END;",

    "\
CREATE TRIGGER CPU_update_uniq_trigger\n\
BEFORE UPDATE\n\
ON CPU\n\
FOR EACH ROW\n\
WHEN OLD.bug_id != NEW.bug_id\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id of bug already in table')\n\
  WHERE (SELECT count(bug_id)\n\
         FROM CPU\n\
         WHERE bug_id=NEW.bug_id) != 0;\n\
END;",


    /*******************
     * Mutation
     */
    "\
CREATE TABLE Mutation\n\
(\n\
  id INTEGER NOT NULL,\n\
\n\
  position INTEGER NOT NULL,\n\
  /* original and mutated can't be NULL at same time */\n\
  original INTEGER,                     -- NULL if new code was added\n\
  mutated INTEGER,                      -- NULL if code was deleted\n\
\n\
  bug_id INTEGER NOT NULL,\n\
\n\
  PRIMARY KEY(id),\n\
  FOREIGN KEY(bug_id) REFERENCES Bug(id),\n\
  CHECK(position >= 0),\n\
  CHECK((original IS NOT NULL) or (mutated IS NOT NULL))\n\
);",

    "\
CREATE INDEX Mutation_index ON Mutation(bug_id);",

    /* Make sure that the bug_id is valid */
    "\
CREATE TRIGGER Mutation_insert_trigger\n\
BEFORE INSERT\n\
ON Mutation\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id of bug not found')\n\
  WHERE (SELECT id\n\
         FROM Bug\n\
         WHERE id=NEW.bug_id AND dead IS NULL) IS NULL;\n\
END;",

    "\
CREATE TRIGGER Mutation_update_trigger\n\
BEFORE UPDATE\n\
ON Mutation\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id of bug not found')\n\
  WHERE (SELECT id\n\
         FROM Bug\n\
         WHERE id=NEW.bug_id AND dead IS NULL) IS NULL;\n\
END;",


    /*******************
     * Food
     */
    "\
CREATE TABLE Food\n\
(\n\
  id INTEGER NOT NULL,\n\
\n\
\n\
  position_x INTEGER NOT NULL,\n\
  position_y INTEGER NOT NULL,\n\
\n\
  size INTEGER NOT NULL,\n\
\n\
  PRIMARY KEY(id),\n\
  CHECK(position_x >= 0 AND position_y >= 0),\n\
  CHECK(size >= 0)\n\
);",

    /* Check that two food elements are not in the same position */
    "\
CREATE TRIGGER Food_insert_position_trigger\n\
BEFORE INSERT\n\
ON Food\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'There is food in the same position')\n\
  WHERE (SELECT id\n\
         FROM Food\n\
         WHERE position_x=NEW.position_x AND position_y=NEW.position_y)\n\
        IS NOT NULL;\n\
END;"

    "\
CREATE TRIGGER Food_update_position_trigger\n\
BEFORE UPDATE\n\
ON Food\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'There is food in the same position')\n\
  WHERE (SELECT id\n\
         FROM Food\n\
         WHERE position_x=NEW.position_x AND position_y=NEW.position_y)\n\
        IS NOT NULL;\n\
END;",

    NULL
  };


  sqlite3x::sqlite3_command sql(*this);

  try {
    // begin a transaction
    sqlite3x::sqlite3_transaction transaction(*this);

    int i;
    for (i = 0; sql_commands[i] != NULL; i++) {
      sql.prepare(sql_commands[i], -1);
      sql.executenonquery();
    }

    transaction.commit();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, e.what());
  }
}

}
}
