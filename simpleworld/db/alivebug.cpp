/**
 * @file simpleworld/db/alivebug.cpp
 * Information about an alive bug.
 *
 *  Copyright (C) 2007-2010  Xosé Otero <xoseotero@gmail.com>
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

#include <cstring>

#include <boost/format.hpp>

#include <sqlite3x.hpp>

#include <simpleworld/ints.hpp>
#include <simpleworld/cpu/types.hpp>
#include "exception.hpp"
#include "alivebug.hpp"

namespace simpleworld
{
namespace db
{

/**
 * Constructor.
 * It's not checked if the id is in the table, only when accessing the data
 * the id is checked.
 * @param db database.
 * @param bug_id id of the bug.
 */
AliveBug::AliveBug(DB* db, ID bug_id)
  : Table("AliveBug", db, bug_id)
{
}


/**
 * Insert a alive bug.
 * @param db database.
 * @param bug_id id of the bug.
 * @param world_id id of the world.
 * @param birth birth time.
 * @param energy energy.
 * @param registers data stored in the registers.
 * @param size size of the data.
 * @return the id of the new row (the same as bug_id).
 * @exception DBException if there is an error with the insertion.
 */
ID AliveBug::insert(DB* db, ID bug_id, ID world_id, Time birth, Energy energy,
		    const void* registers, Uint32 size)
{
  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
INSERT INTO AliveBug(bug_id, world_id, birth, energy, registers)\n\
VALUES(?, ?, ?, ?, ?);");
    sql.bind(1, static_cast<sqlite3x::int64_t>(bug_id));
    sql.bind(2, static_cast<sqlite3x::int64_t>(world_id));
    sql.bind(3, static_cast<int>(birth));
    sql.bind(4, static_cast<int>(energy));
    sql.bind(5, registers, size);

    sql.executenonquery();
    return db->insertid();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + db->errormsg() + ")");
  }
}

/**
 * Insert a alive bug from a egg.
 * The egg is deleted.
 * @param db database.
 * @param egg egg to convert in a alive bug.
 * @param birth birth time.
 * @return the id of the new row (the same as the bug_id of the egg).
 * @exception DBException if there is an error with the insertion.
 */
ID AliveBug::insert(DB* db, Egg* egg, Time birth)
{
  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
INSERT INTO AliveBug(bug_id, world_id, birth, energy, registers)\n\
VALUES(?, ?, ?, ?, ?);");
    sql.bind(1, static_cast<sqlite3x::int64_t>(egg->bug_id()));
    sql.bind(2, static_cast<sqlite3x::int64_t>(egg->world_id()));
    sql.bind(3, static_cast<int>(birth));
    sql.bind(4, static_cast<int>(egg->energy()));
    Uint32 size = 16 * sizeof(cpu::Word);
    Uint8* registers = new Uint8[size];
    std::memset(registers, 0, size);
    sql.bind(5, registers, size);

    sql.executenonquery();
    ID id = db->insertid();

    Egg::remove(db, egg->id());

    return id;
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + db->errormsg() + ")");
  }
}

/**
 * Delete a alive bug.
 * @param db database.
 * @param id id of the bug.
 * @exception DBException if there is an error with the deletion.
 */
void AliveBug::remove(DB*db, ID id)
{
  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
DELETE FROM AliveBug\n\
WHERE bug_id = ?;");
    sql.bind(1, id);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + db->errormsg() + ")");
  }
}


/**
 * Get the id of the bug.
 * @return the id.
 * @exception DBException if there is an error with the query.
 */
ID AliveBug::bug_id() const
{
  return this->id_;
}

/**
 * Set the id of the bug.
 * @param bug_id the new id.
 * @exception DBException if there is an error with the update.
 */
void AliveBug::bug_id(ID bug_id)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE AliveBug\n\
SET bug_id = ?\n\
WHERE bug_id = ?;");
    sql.bind(1, static_cast<sqlite3x::int64_t>(bug_id));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }

  this->id_ = bug_id;
}


/**
 * Get the id of the world.
 * @return the id.
 * @exception DBException if there is an error with the query.
 */
ID AliveBug::world_id() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT world_id\n\
FROM AliveBug\n\
WHERE bug_id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table AliveBug")
                                              % this->id_));

    return cursor.getint64(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set the id of the world.
 * @return the new id.
 * @exception DBException if there is an error with the update.
 */
void AliveBug::world_id(ID world_id)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE AliveBug\n\
SET world_id = ?\n\
WHERE bug_id = ?;");
    sql.bind(1, static_cast<sqlite3x::int64_t>(world_id));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Get the birth of the bug.
 * @return the birth.
 * @exception DBException if there is an error with the query.
 */
Time AliveBug::birth() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT birth\n\
FROM AliveBug\n\
WHERE bug_id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table AliveBug")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set the birth of the bug.
 * @param birth the new birth.
 * @exception DBException if there is an error with the update.
 */
void AliveBug::birth(Time birth)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE AliveBug\n\
SET birth = ?\n\
WHERE bug_id = ?;");
    sql.bind(1, static_cast<int>(birth));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Get the energy of the bug.
 * @return the energy.
 * @exception DBException if there is an error with the query.
 */
Energy AliveBug::energy() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT energy\n\
FROM AliveBug\n\
WHERE bug_id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table AliveBug")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set the energy of the bug.
 * @param energy the new energy.
 * @exception DBException if there is an error with the update.
 */
void AliveBug::energy(Energy energy)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE AliveBug\n\
SET energy = ?\n\
WHERE bug_id = ?;");
    sql.bind(1, static_cast<int>(energy));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}


/**
 * Get when the bug did the last action.
 * time_last_action is NULL if no action was done yet.
 * @return the time.
 * @exception DBException if there is an error with the query.
 */
Time AliveBug::time_last_action() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT time_last_action\n\
FROM AliveBug\n\
WHERE bug_id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table AliveBug")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set when the bug did the last action.
 * @param time_last_action the new time.
 * @exception DBException if there is an error with the update.
 */
void AliveBug::time_last_action(Time time_last_action)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE AliveBug\n\
SET time_last_action = ?\n\
WHERE bug_id = ?;");
    sql.bind(1, static_cast<int>(time_last_action));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Get when the bug will finish the current action.
 * action_time is NULL if the bug is not doing a action.
 * @return the time.
 * @exception DBException if there is an error with the query.
 */
Time AliveBug::action_time() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT action_time\n\
FROM AliveBug\n\
WHERE bug_id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table AliveBug")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set when the bug will finish the current action.
 * @param action_time the new time.
 * @exception DBException if there is an error with the update.
 */
void AliveBug::action_time(Time action_time)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE AliveBug\n\
SET action_time = ?\n\
WHERE bug_id = ?;");
    sql.bind(1, static_cast<int>(action_time));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Get the registers of the bug.
 * @return the registers.
 * @exception DBException if there is an error with the query.
 */
Blob AliveBug::registers() const
{
  return Blob(this->db_, "AliveBug", "registers", this->id_);
}

}
}
