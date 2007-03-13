/**
 * @file db/db.cpp
 * Simple World Data Base.
 *
 * begin:     Wed, 24 Jan 2007 06:23:10 +0100
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2006, 2007 Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  License: See COPYING file that comes with this distribution
 */

#include "db.hpp"

#include <cstdlib>

#include <db/bug.hpp>
#include <db/hierarchy.hpp>
#include <db/code.hpp>
#include <db/cpu.hpp>
#include <db/mutations.hpp>
#include <db/environment.hpp>

#define DATABASE_VERSION 1

namespace SimpleWorld
{
namespace DB
{

DB::DB(std::string filename)
  : sqlite3x::sqlite3_connection(filename)
{
}


// Bug management
std::vector<ID> DB::bugs()
{
  static sqlite3x::sqlite3_command sql(*this,
				       "\
SELECT id\n\
FROM Bug\n\
ORDER BY birth, id;");
  std::vector<ID> ids;
  try {
    sqlite3x::sqlite3_cursor cursor = sql.executecursor();
    while (cursor.step())
      ids.push_back(cursor.getint64(0));
  } catch (sqlite3x::database_error) {
    throw DBError(__FILE__, __LINE__);
  }

  return ids;
}

std::vector<ID> DB::alive_bugs()
{
  static sqlite3x::sqlite3_command sql(*this,
				       "\
SELECT id\n\
FROM Bug_alive\n\
ORDER BY world_order, id;");
  std::vector<ID> ids;
  try {
    sqlite3x::sqlite3_cursor cursor = sql.executecursor();
    while (cursor.step())
      ids.push_back(cursor.getint64(0));
  } catch (sqlite3x::database_error) {
    throw DBError(__FILE__, __LINE__);
  }

  return ids;
}

boost::shared_ptr<Bug> DB::bug(ID id)
{
  std::map<ID, boost::weak_ptr<Bug> >::iterator iter = this->bug_map_.find(id);
  if (iter == this->bug_map_.end()) {
    boost::shared_ptr<Bug> ptr(new Bug(this, id));
    this->bug_map_[id] = boost::weak_ptr<Bug>(ptr);
    return ptr;
  } else {
    return boost::shared_ptr<Bug>((*iter).second);
  }
}

void DB::free_bug(ID id)
{
  std::map<ID, boost::weak_ptr<Bug> >::iterator iter = this->bug_map_.find(id);
  if (iter == this->bug_map_.end())
    throw IDNotFound(__FILE__, __LINE__);

  this->bug_map_.erase(iter);
}

ID DB::add_bug(SortOrder order, Energy energy,
	       Position x, Position y, Orientation orientation,
	       Time birth)
{
  static sqlite3x::sqlite3_command sql(*this,
				       "\
INSERT INTO Bug(order_world, energy,\n\
                position_x, position_y, orientation,\n\
                birth)\n\
VALUES (?, ?, ?, ?, ?, ?);");
  sql.bind(1, static_cast<int>(order));
  sql.bind(2, static_cast<int>(energy));
  sql.bind(3, static_cast<int>(x));
  sql.bind(4, static_cast<int>(y));
  sql.bind(5, static_cast<int>(orientation));
  sql.bind(6, static_cast<int>(birth));

  try {
    sql.executenonquery();
  } catch (sqlite3x::database_error) {
    throw DBError(__FILE__, __LINE__);
  }

  return this->insertid();
}

void DB::del_bug(ID id)
{
  static sqlite3x::sqlite3_command sql(*this,
				       "\
DELETE FROM Bug\n\
WHERE id = ?;");
  sql.bind(1, id);
  sql.executenonquery();

  if (this->changes() == 0)
    throw IDNotFound(__FILE__, __LINE__);
}


// Hierarchy management
boost::shared_ptr<Hierarchy> DB::hierarchy(ID bug_id)
{
  std::map<ID, boost::weak_ptr<Hierarchy> >::iterator iter =
    this->hierarchy_map_.find(bug_id);
  if (iter == this->hierarchy_map_.end()) {
    boost::shared_ptr<Hierarchy> ptr(new Hierarchy(this, bug_id));
    this->hierarchy_map_[bug_id] = boost::weak_ptr<Hierarchy>(ptr);
    return ptr;
  } else {
    return boost::shared_ptr<Hierarchy>((*iter).second);
  }
}

void DB::free_hierarchy(ID bug_id)
{
  std::map<ID, boost::weak_ptr<Hierarchy> >::iterator iter =
    this->hierarchy_map_.find(bug_id);
  if (iter == this->hierarchy_map_.end())
    throw IDNotFound(__FILE__, __LINE__);

  this->hierarchy_map_.erase(iter);
}


// Code management
boost::shared_ptr<Code> DB::code(ID bug_id)
{
  std::map<ID, boost::weak_ptr<Code> >::iterator iter =
    this->code_map_.find(bug_id);
  if (iter == this->code_map_.end()) {
    boost::shared_ptr<Code> ptr(new Code(this, bug_id));
    this->code_map_[bug_id] = boost::weak_ptr<Code>(ptr);
    return ptr;
  } else {
    return boost::shared_ptr<Code>((*iter).second);
  }
}

void DB::free_code(ID bug_id)
{
  std::map<ID, boost::weak_ptr<Code> >::iterator iter =
    this->code_map_.find(bug_id);
  if (iter == this->code_map_.end())
    throw IDNotFound(__FILE__, __LINE__);

  this->code_map_.erase(iter);
}


// CPU management
boost::shared_ptr<CPU> DB::cpu(ID bug_id)
{
  std::map<ID, boost::weak_ptr<CPU> >::iterator iter =
    this->cpu_map_.find(bug_id);
  if (iter == this->cpu_map_.end()) {
    boost::shared_ptr<CPU> ptr(new CPU(this, bug_id));
    this->cpu_map_[bug_id] = boost::weak_ptr<CPU>(ptr);
    return ptr;
  } else {
    return boost::shared_ptr<CPU>((*iter).second);
  }
}

void DB::free_cpu(ID bug_id)
{
  std::map<ID, boost::weak_ptr<CPU> >::iterator iter =
    this->cpu_map_.find(bug_id);
  if (iter == this->cpu_map_.end())
    throw IDNotFound(__FILE__, __LINE__);

  this->cpu_map_.erase(iter);
}


// Mutations management
boost::shared_ptr<Mutations> DB::mutations(ID bug_id)
{
  std::map<ID, boost::weak_ptr<Mutations> >::iterator iter =
    this->mutations_map_.find(bug_id);
  if (iter == this->mutations_map_.end()) {
    boost::shared_ptr<Mutations> ptr(new Mutations(this, bug_id));
    this->mutations_map_[bug_id] = boost::weak_ptr<Mutations>(ptr);
    return ptr;
  } else {
    return boost::shared_ptr<Mutations>((*iter).second);
  }
}

void DB::free_mutations(ID bug_id)
{
  std::map<ID, boost::weak_ptr<Mutations> >::iterator iter =
    this->mutations_map_.find(bug_id);
  if (iter == this->mutations_map_.end())
    throw IDNotFound(__FILE__, __LINE__);

  this->mutations_map_.erase(iter);
}


// Environment management
std::vector<Time> DB::environments()
{
  static sqlite3x::sqlite3_command sql(*this,
				       "\
SELECT time\n\
FROM Environment\n\
ORDER BY time;");
  std::vector<Time> ids;
  try {
    sqlite3x::sqlite3_cursor cursor = sql.executecursor();
    while (cursor.step())
      ids.push_back(cursor.getint(0));
  } catch (sqlite3x::database_error) {
    throw DBError(__FILE__, __LINE__);
  }

  return ids;
}

Time DB::last_environment()
{
  static sqlite3x::sqlite3_command sql(*this,
				       "\
SELECT time\n\
FROM Environment\n\
ORDER BY time\n\
LIMIT 1;");
  try {
    sqlite3x::sqlite3_cursor cursor = sql.executecursor();
    if (cursor.step())
      return cursor.getint(0);
    else
      throw IDNotFound(__FILE__, __LINE__);
  } catch (sqlite3x::database_error) {
    throw DBError(__FILE__, __LINE__);
  }
}

boost::shared_ptr<Environment> DB::environment(Time time)
{
  std::map<Time, boost::weak_ptr<Environment> >::iterator iter =
    this->environment_map_.find(time);
  if (iter == this->environment_map_.end()) {
    boost::shared_ptr<Environment> ptr(new Environment(this, time));
    this->environment_map_[time] = boost::weak_ptr<Environment>(ptr);
    return ptr;
  } else {
    return boost::shared_ptr<Environment>((*iter).second);
  }
}

void DB::free_environment(Time time)
{
  std::map<Time, boost::weak_ptr<Environment> >::iterator iter =
    this->environment_map_.find(time);
  if (iter == this->environment_map_.end())
    throw IDNotFound(__FILE__, __LINE__);

  this->environment_map_.erase(iter);
}

Time DB::add_environment(World::Time time, Uint8 mutations_percent,
			 Energy energy_detect, Energy energy_move,
			 Energy energy_push, Energy energy_take,
			 Energy energy_attack, Energy energy_defend,
			 Energy energy_msg, Energy energy_sex)
{
  static sqlite3x::sqlite3_command sql(*this,
				       "\
INSERT INTO Environment(time, mutations_percent,\n\
                        energy_detect, energy_move,\n\
                        energy_push, energy_take,\n\
                        energy_attack, energy_defend,\n\
                        energy_msg, energy_sex)\n\
VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
  sql.bind(1, static_cast<int>(time));
  sql.bind(2, static_cast<int>(mutations_percent));
  sql.bind(3, static_cast<int>(energy_detect));
  sql.bind(4, static_cast<int>(energy_move));
  sql.bind(5, static_cast<int>(energy_push));
  sql.bind(6, static_cast<int>(energy_take));
  sql.bind(7, static_cast<int>(energy_attack));
  sql.bind(8, static_cast<int>(energy_defend));
  sql.bind(9, static_cast<int>(energy_msg));
  sql.bind(10, static_cast<int>(energy_sex));

  try {
    sql.executenonquery();
  } catch (sqlite3x::database_error) {
    throw DBError(__FILE__, __LINE__);
  }

  return this->insertid();
}

void DB::del_environment(Time time)
{
  static sqlite3x::sqlite3_command sql(*this,
				       "\
DELETE FROM Environment\n\
WHERE time = ?;");
  sql.bind(1, static_cast<int>(time));
  sql.executenonquery();

  if (this->changes() == 0)
    throw IDNotFound(__FILE__, __LINE__);
}


void DB::on_open()
{
  // user_version is used for know if the database was new (user_version is 0
  // by default) and the version of the database (user_version is set to
  // DATABASE_VERSION) to check if the database is compatible
  this->version_ = this->executeint("PRAGMA user_version;");
  if (this->version_ == 0) {	// Data base was created now.
    sqlite3x::sqlite3_command sql(*this, "PRAGMA user_version = %d;");
    sql.bind(1, static_cast<int>(DATABASE_VERSION));
    sql.executenonquery();
    this->version_ = DATABASE_VERSION;

    this->create_tables();
  } else if (this->version_ != DATABASE_VERSION)
    throw VersionNotSupported(__FILE__, __LINE__);
}

void DB::create_tables()
{
  sqlite3x::sqlite3_command sql(*this,
				"\
/*******************\n\
 * Bug\n\
 */\n\
CREATE TABLE Bug\n\
(\n\
  id INTEGER NOT NULL,\n\
\n\
  order_world INTEGER,                  -- NULL if dead\n\
\n\
  energy INTEGER NOT NULL,\n\
\n\
  position_x INTEGER NOT NULL,\n\
  position_y INTEGER NOT NULL,\n\
  orientation INTEGER NOT NULL,\n\
\n\
  birth INTEGER NOT NULL,\n\
  dead INTEGER,                         -- NULL if alive\n\
\n\
  killer_id INTEGER,                    -- NULL if alive\n\
\n\
  PRIMARY KEY(id),\n\
  FOREIGN KEY(killer_id) REFERENCES Bug(id),\n\
  CHECK(order_world >= 0),\n\
  CHECK(position_x >= 0 AND position_y >= 0),\n\
  CHECK(orientation >= 0 AND orientation <= 3), -- 4 possible orientations\n\
  CHECK(birth >= 0),\n\
  CHECK(birth <= dead),                  -- If dead IS NOT NULL\n\
  CHECK(killer_id IS NULL OR (killer_id IS NOT NULL AND dead IS NOT NULL))\n\
);\n\
\n\
CREATE INDEX Bug_index ON Bug(id);\n\
CREATE INDEX Bug_position_index ON Bug(position_x, position_y);\n\
CREATE INDEX Bug_dead_index ON Bug(dead);\n\
\n\
CREATE VIEW Bug_alive AS SELECT * FROM Bug WHERE dead IS NULL;\n\
\n\
/* Check if the id will change */\n\
CREATE TRIGGER Bug_id_trigger\n\
BEFORE UPDATE\n\
ON Bug\n\
FOR EACH ROW\n\
WHEN OLD.id != NEW.id\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id changed');\n\
END;\n\
\n\
/* Check if the bug is alive before update */\n\
CREATE TRIGGER Bug_alive_trigger\n\
BEFORE UPDATE\n\
ON Bug\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'Bug dead')\n\
  WHERE (SELECT id\n\
         FROM Bug\n\
         WHERE id=NEW.id AND dead IS NULL)\n\
        IS NULL;\n\
END;\n\
\n\
/* Check if killer_id is a valid id */\n\
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
END;\n\
\n\
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
END;\n\
\n\
/* Check that two bugs are not in the same position */\n\
CREATE TRIGGER Bug_insert_position_trigger\n\
BEFORE INSERT\n\
ON Bug\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'There is a bug in the same position')\n\
  WHERE (SELECT id\n\
         FROM Bug\n\
         WHERE dead IS NULL AND position_x=NEW.position_x AND\n\
               position_y=NEW.position_y)\n\
        IS NOT NULL;\n\
END;\n\
\n\
CREATE TRIGGER Bug_update_position_trigger\n\
BEFORE UPDATE\n\
ON Bug\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'There is a bug in the same position')\n\
  WHERE (SELECT id\n\
         FROM Bug\n\
         WHERE dead IS NULL AND position_x=NEW.position_x AND\n\
               position_y=NEW.position_y)\n\
        IS NOT NULL;\n\
END;\n\
\n\
/* Propagate the dead */\n\
CREATE TRIGGER Bug_update_dead_trigger\n\
BEFORE UPDATE\n\
ON Bug\n\
FOR EACH ROW\n\
WHEN OLD.dead IS NULL AND NEW.dead IS NOT NULL\n\
BEGIN\n\
  DELETE FROM CPU WHERE bug_id = OLD.id;\n\
END;\n\
\n\
/* Propagate the deletion */\n\
CREATE TRIGGER Bug_delete_trigger\n\
BEFORE DELETE\n\
ON Bug\n\
FOR EACH ROW BEGIN\n\
  DELETE FROM Hierarchy WHERE son_id = OLD.id OR father_id = OLD.id;\n\
  DELETE FROM Code WHERE bug_id = OLD.id;\n\
  DELETE FROM CPU WHERE bug_id = OLD.id;\n\
  DELETE FROM Mutation WHERE bug_id = OLD.id;\n\
END;\n\
\n\
\n\
/*******************\n\
 * Hierarchy\n\
 */\n\
CREATE TABLE Hierarchy\n\
(\n\
  id INTEGER NOT NULL,\n\
\n\
  /* A Bug without sons won't appear as father_id. */\n\
  father_id INTEGER NOT NULL,\n\
  /* A Bug without fathers won't appear as son_id. */\n\
  son_id INTEGER NOT NULL,\n\
\n\
  PRIMARY KEY(id),\n\
  FOREIGN KEY(father_id) REFERENCES Bug(id),\n\
  FOREIGN KEY(son_id) REFERENCES Bug(id),\n\
  CHECK(father_id != son_id)\n\
);\n\
\n\
/* Make sure that the ids of the father and the son are valid */\n\
CREATE TRIGGER Hierarchy_insert_ids_trigger\n\
BEFORE INSERT\n\
ON Hierarchy\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id of father or son not found')\n\
  WHERE (SELECT id\n\
         FROM Bug\n\
         WHERE id=NEW.father_id AND dead IS NULL) IS NULL\n\
        OR\n\
        (SELECT id\n\
         FROM Bug\n\
         WHERE id=NEW.son_id AND dead IS NULL) IS NULL;\n\
END;\n\
\n\
CREATE TRIGGER Hierarchy_update_ids_trigger\n\
BEFORE UPDATE\n\
ON Hierarchy\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id of father or son not found')\n\
  WHERE (SELECT id\n\
         FROM Bug\n\
         WHERE id=NEW.father_id AND dead IS NULL) IS NULL\n\
        OR\n\
        (SELECT id\n\
         FROM Bug\n\
         WHERE id=NEW.son_id AND dead IS NULL) IS NULL;\n\
END;\n\
\n\
\n\
/*******************\n\
 * Code\n\
 */\n\
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
);\n\
\n\
CREATE INDEX Code_index ON Code(bug_id);\n\
\n\
/* Check if the id will change */\n\
CREATE TRIGGER Code_update_id_trigger\n\
BEFORE UPDATE\n\
ON Code\n\
FOR EACH ROW\n\
WHEN OLD.bug_id != NEW.bug_id\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id changed');\n\
END;\n\
\n\
/* Make sure that the bug_id is valid */\n\
CREATE TRIGGER Code_insert_valid_trigger\n\
BEFORE INSERT\n\
ON Code\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id of bug not found')\n\
  WHERE (SELECT id\n\
         FROM Bug\n\
         WHERE id=NEW.bug_id AND dead IS NULL) IS NULL;\n\
END;\n\
\n\
CREATE TRIGGER Code_update_valid_trigger\n\
BEFORE UPDATE\n\
ON Code\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id of bug not found')\n\
  WHERE (SELECT id\n\
         FROM Bug\n\
         WHERE id=NEW.bug_id AND dead IS NULL) IS NULL;\n\
END;\n\
\n\
/* Make sure that the bug_id is unique */\n\
CREATE TRIGGER Code_insert_uniq_trigger\n\
BEFORE INSERT\n\
ON Code\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id of bug already in table')\n\
  WHERE (SELECT count(bug_id)\n\
         FROM Code\n\
         WHERE bug_id=NEW.bug_id) != 0;\n\
END;\n\
\n\
CREATE TRIGGER Code_update_uniq_trigger\n\
BEFORE UPDATE\n\
ON Code\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id of bug already in table')\n\
  WHERE (SELECT count(bug_id)\n\
         FROM Code\n\
         WHERE bug_id=NEW.bug_id) != 0;\n\
END;\n\
\n\
\n\
/*******************\n\
 * CPU\n\
 */\n\
CREATE TABLE CPU\n\
(\n\
  bug_id INTEGER NOT NULL,\n\
\n\
  /* CPU registers */\n\
  r0 INTEGER NOT NULL DEFAULT 0,\n\
  r1 INTEGER NOT NULL DEFAULT 0,\n\
  r2 INTEGER NOT NULL DEFAULT 0,\n\
  r3 INTEGER NOT NULL DEFAULT 0,\n\
  r4 INTEGER NOT NULL DEFAULT 0,\n\
  r5 INTEGER NOT NULL DEFAULT 0,\n\
  r6 INTEGER NOT NULL DEFAULT 0,\n\
  r7 INTEGER NOT NULL DEFAULT 0,\n\
  r8 INTEGER NOT NULL DEFAULT 0,\n\
  r9 INTEGER NOT NULL DEFAULT 0,\n\
  r10 INTEGER NOT NULL DEFAULT 0,\n\
  r11 INTEGER NOT NULL DEFAULT 0,\n\
  r12 INTEGER NOT NULL DEFAULT 0,\n\
  pc INTEGER NOT NULL DEFAULT 0,\n\
  sp INTEGER NOT NULL DEFAULT 0,\n\
  etp INTEGER NOT NULL DEFAULT 0,\n\
\n\
  PRIMARY KEY(bug_id),\n\
  FOREIGN KEY(bug_id) REFERENCES Bug(id)\n\
);\n\
\n\
CREATE INDEX CPU_index ON CPU(bug_id);\n\
\n\
/* Check if the id will change */\n\
CREATE TRIGGER CPU_update_id_trigger\n\
BEFORE UPDATE\n\
ON CPU\n\
FOR EACH ROW\n\
WHEN OLD.bug_id != NEW.bug_id\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id changed');\n\
END;\n\
\n\
/* Make sure that the bug_id is valid */\n\
CREATE TRIGGER CPU_insert_valid_trigger\n\
BEFORE INSERT\n\
ON CPU\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id of bug not found')\n\
  WHERE (SELECT id\n\
         FROM Bug\n\
         WHERE id=NEW.bug_id AND dead IS NULL) IS NULL;\n\
END;\n\
\n\
CREATE TRIGGER CPU_update_valid_trigger\n\
BEFORE UPDATE\n\
ON CPU\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id of bug not found')\n\
  WHERE (SELECT id\n\
         FROM Bug\n\
         WHERE id=NEW.bug_id AND dead IS NULL) IS NULL;\n\
END;\n\
\n\
/* Make sure that the bug_id is unique */\n\
CREATE TRIGGER CPU_insert_uniq_trigger\n\
BEFORE INSERT\n\
ON CPU\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id of bug already in table')\n\
  WHERE (SELECT count(bug_id)\n\
         FROM CPU\n\
         WHERE bug_id=NEW.bug_id) != 0;\n\
END;\n\
\n\
CREATE TRIGGER CPU_update_uniq_trigger\n\
BEFORE UPDATE\n\
ON CPU\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id of bug already in table')\n\
  WHERE (SELECT count(bug_id)\n\
         FROM CPU\n\
         WHERE bug_id=NEW.bug_id) != 0;\n\
END;\n\
\n\
\n\
/*******************\n\
 * Mutation\n\
 */\n\
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
  CHECK(original >= 0),                 -- If original IS NOT NULL\n\
  CHECK(mutated >= 0)                   -- If mutated IS NOT NULL\n\
);\n\
\n\
CREATE INDEX Mutation_index ON Mutation(bug_id);\n\
\n\
/* Make sure that the bug_id is valid */\n\
CREATE TRIGGER Mutation_insert_trigger\n\
BEFORE INSERT\n\
ON Mutation\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id of bug not found')\n\
  WHERE (SELECT id\n\
         FROM Bug\n\
         WHERE id=NEW.bug_id AND dead IS NULL) IS NULL;\n\
END;\n\
\n\
CREATE TRIGGER Mutation_update_trigger\n\
BEFORE UPDATE\n\
ON Mutation\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id of bug not found')\n\
  WHERE (SELECT id\n\
         FROM Bug\n\
         WHERE id=NEW.bug_id AND dead IS NULL) IS NULL;\n\
END;\n\
\n\
\n\
/*******************\n\
 * Environment\n\
 */\n\
CREATE TABLE Environment\n\
(\n\
  time INTEGER NOT NULL,\n\
\n\
  order_world INTEGER NOT NULL DEFAULT 0,\n\
\n\
  -- The rest of the values can't change\n\
  mutations_percent INTEGER NOT NULL,\n\
\n\
  energy_detect INTEGER NOT NULL,\n\
  energy_move INTEGER NOT NULL,\n\
  energy_push INTEGER NOT NULL,\n\
  energy_take INTEGER NOT NULL,\n\
  energy_attack INTEGER NOT NULL,\n\
  energy_defend INTEGER NOT NULL,\n\
  energy_msg INTEGER NOT NULL,\n\
  energy_sex INTEGER NOT NULL,\n\
\n\
  PRIMARY KEY(time),\n\
  CHECK(time >= 0),\n\
  CHECK(mutations_percent >= 0),\n\
  CHECK(energy_detect >= 0 AND energy_move >= 0 AND\n\
        energy_push >= 0 AND energy_take >= 0 AND\n\
        energy_attack >= 0 AND energy_defend >= 0 AND\n\
        energy_msg >= 0 AND energy_sex >= 0)\n\
);\n\
\n\
CREATE INDEX Environment_index ON Environment(time);\n\
\n\
/* time must be the older of the table */\n\
CREATE TRIGGER Environment_insert_time\n\
BEFORE INSERT\n\
ON Environment\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'There is a older time')\n\
  WHERE (SELECT max(time)\n\
         FROM Environment) > NEW.time;\n\
END;\n\
\n\
CREATE TRIGGER Environment_update_time\n\
BEFORE UPDATE\n\
ON Environment\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'There is a older time')\n\
  WHERE (SELECT max(time)\n\
         FROM Environment) >= NEW.time;\n\
END;\n\
\n\
/* Only time and order_world can change */\n\
CREATE TRIGGER Environment_update_valid_time\n\
BEFORE UPDATE\n\
ON Environment\n\
FOR EACH ROW\n\
WHEN OLD.mutations_percent != NEW.mutations_percent OR\n\
     OLD.energy_detect != NEW.energy_detect OR\n\
     OLD.energy_move != NEW.energy_move OR\n\
     OLD.energy_push != NEW.energy_push OR\n\
     OLD.energy_take != NEW.energy_take OR\n\
     OLD.energy_attack != NEW.energy_attack OR\n\
     OLD.energy_defend != NEW.energy_defend OR\n\
     OLD.energy_msg != NEW.energy_msg OR\n\
     OLD.energy_sex != NEW.energy_sex\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'changed invalid col');\n\
END;");

  try {
    sql.executenonquery();
  } catch (sqlite3x::database_error) {
    throw DBError(__FILE__, __LINE__);
  }
}

}
}
