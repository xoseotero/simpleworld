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

#include <sqlite3.h>

#include <simpleworld/ints.hpp>
#include <simpleworld/cpu/types.hpp>
#include <simpleworld/cpu/cpu.hpp>
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
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
INSERT INTO AliveBug(bug_id, world_id, birth, energy, registers)\n\
VALUES(?, ?, ?, ?, ?);", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_bind_int64(stmt, 1, bug_id);
  sqlite3_bind_int64(stmt, 2, world_id);
  sqlite3_bind_int(stmt, 3, birth);
  sqlite3_bind_int(stmt, 4, energy);
  sqlite3_bind_blob(stmt, 5, registers, size, SQLITE_TRANSIENT);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_finalize(stmt);

  return sqlite3_last_insert_rowid(db->db());
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
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
INSERT INTO AliveBug(bug_id, world_id, birth, energy, registers)\n\
VALUES(?, ?, ?, ?, ?);", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_bind_int64(stmt, 1, egg->bug_id());
  sqlite3_bind_int64(stmt, 2, egg->world_id());
  sqlite3_bind_int(stmt, 3, birth);
  sqlite3_bind_int(stmt, 4, egg->energy());
  sqlite3_bind_zeroblob(stmt, 5, TOTAL_REGISTERS * sizeof(cpu::Word));
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_finalize(stmt);
  ID id = sqlite3_last_insert_rowid(db->db());

  Egg::remove(db, egg->id());

  return id;
}

/**
 * Delete a alive bug.
 * @param db database.
 * @param id id of the bug.
 * @exception DBException if there is an error with the deletion.
 */
void AliveBug::remove(DB*db, ID id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
DELETE FROM AliveBug\n\
WHERE bug_id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_bind_int64(stmt, 1, id);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_finalize(stmt);
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
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE AliveBug\n\
SET bug_id = ?\n\
WHERE bug_id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, bug_id);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);

  this->id_ = bug_id;
}


/**
 * Get the id of the world.
 * @return the id.
 * @exception DBException if there is an error with the query.
 */
ID AliveBug::world_id() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT world_id\n\
FROM AliveBug\n\
WHERE bug_id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table AliveBug")
					    % this->id_));
  ID world_id = sqlite3_column_int64(stmt, 0);
  sqlite3_finalize(stmt);

  return world_id;
}

/**
 * Set the id of the world.
 * @return the new id.
 * @exception DBException if there is an error with the update.
 */
void AliveBug::world_id(ID world_id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE AliveBug\n\
SET world_id = ?\n\
WHERE bug_id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, world_id);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

/**
 * Get the birth of the bug.
 * @return the birth.
 * @exception DBException if there is an error with the query.
 */
Time AliveBug::birth() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT birth\n\
FROM AliveBug\n\
WHERE bug_id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table AliveBug")
					    % this->id_));
  Time birth = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return birth;
}

/**
 * Set the birth of the bug.
 * @param birth the new birth.
 * @exception DBException if there is an error with the update.
 */
void AliveBug::birth(Time birth)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE AliveBug\n\
SET birth = ?\n\
WHERE bug_id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, birth);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

/**
 * Get the energy of the bug.
 * @return the energy.
 * @exception DBException if there is an error with the query.
 */
Energy AliveBug::energy() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT energy\n\
FROM AliveBug\n\
WHERE bug_id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table AliveBug")
					    % this->id_));
  Energy energy = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return energy;
}

/**
 * Set the energy of the bug.
 * @param energy the new energy.
 * @exception DBException if there is an error with the update.
 */
void AliveBug::energy(Energy energy)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE AliveBug\n\
SET energy = ?\n\
WHERE bug_id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, energy);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
 * Get when the bug did the last action.
 * time_last_action is NULL if no action was done yet.
 * @return the time.
 * @exception DBException if there is an error with the query.
 */
Time AliveBug::time_last_action() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT time_last_action\n\
FROM AliveBug\n\
WHERE bug_id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table AliveBug")
					    % this->id_));
  Time time_last_action = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return time_last_action;
}

/**
 * Set when the bug did the last action.
 * @param time_last_action the new time.
 * @exception DBException if there is an error with the update.
 */
void AliveBug::time_last_action(Time time_last_action)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE AliveBug\n\
SET time_last_action = ?\n\
WHERE bug_id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, time_last_action);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

/**
 * Get when the bug will finish the current action.
 * action_time is NULL if the bug is not doing a action.
 * @return the time.
 * @exception DBException if there is an error with the query.
 */
Time AliveBug::action_time() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT action_time\n\
FROM AliveBug\n\
WHERE bug_id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table AliveBug")
					    % this->id_));
  Time action_time = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return action_time;
}

/**
 * Set when the bug will finish the current action.
 * @param action_time the new time.
 * @exception DBException if there is an error with the update.
 */
void AliveBug::action_time(Time action_time)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE AliveBug\n\
SET action_time = ?\n\
WHERE bug_id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, action_time);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
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
