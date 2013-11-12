/**
 * @file simpleworld/db/egg.cpp
 * Information about an egg.
 *
 *  Copyright (C) 2007-2013  Xosé Otero <xoseotero@gmail.com>
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

#include <string>

#include <boost/format.hpp>

#include <sqlite3.h>

#include "exception.hpp"
#include "egg.hpp"

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
Egg::Egg(DB* db, ID bug_id)
  : Table("Egg", db, bug_id)
{
}


/**
 * Insert a egg.
 * @param db database.
 * @param bug_id id of the bug.
 * @param world_id id of the world.
 * @param energy energy.
 * @param memory_id id of the memory of the bug.
 * @return the id of the new row (the same as bug_id).
 * @exception DBException if there is an error with the insertion.
 */
ID Egg::insert(DB* db, ID bug_id, ID world_id, Energy energy, ID memory_id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
INSERT INTO Egg(bug_id, world_id, energy, memory_id)\n\
VALUES(?, ?, ?, ?);", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_bind_int64(stmt, 1, bug_id);
  sqlite3_bind_int64(stmt, 2, world_id);
  sqlite3_bind_int(stmt, 3, energy);
  sqlite3_bind_int64(stmt, 4, memory_id);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_finalize(stmt);

  return sqlite3_last_insert_rowid(db->db());
}

/**
 * Delete a egg.
 * @param db database.
 * @param bug_id id of the egg.
 * @exception DBException if there is an error with the deletion.
 */
void Egg::remove(DB* db, ID id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
DELETE FROM Egg\n\
WHERE bug_id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_bind_int64(stmt, 1, id);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_finalize(stmt);
}


/**
 * Get the id of the egg.
 * @return the id.
 * @exception DBException if there is an error with the query.
 */
ID Egg::bug_id() const
{
  return this->id_;
}

/**
 * Set the id of the egg.
 * @param bug_id the new id.
 * @exception DBException if there is an error with the update.
 */
void Egg::bug_id(ID bug_id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Egg\n\
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
ID Egg::world_id() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT world_id\n\
FROM Egg\n\
WHERE bug_id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Egg")
                                            % this->id_));
  ID world_id = sqlite3_column_int64(stmt, 0);
  sqlite3_finalize(stmt);

  return world_id;
}

/**
 * Set the id of the world.
 * @param world_id the new id.
 * @exception DBException if there is an error with the update.
 */
void Egg::world_id(ID world_id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Egg\n\
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
 * Get the energy of the egg.
 * @return the energy.
 * @exception DBException if there is an error with the query.
 */
Energy Egg::energy() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT energy\n\
FROM Egg\n\
WHERE bug_id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Egg")
                                            % this->id_));
  Energy energy = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return energy;
}

/**
 * Set the energy of the egg.
 * @param energy the new energy.
 * @exception DBException if there is an error with the update.
 */
void Egg::energy(Energy energy)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Egg\n\
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
 * Get the id of the memory.
 * @return the id.
 * @exception DBException if there is an error with the query.
 */
ID Egg::memory_id() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT memory_id\n\
FROM Egg\n\
WHERE bug_id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table AliveBug")
    % this->id_));
  ID memory_id = sqlite3_column_int64(stmt, 0);
  sqlite3_finalize(stmt);

  return memory_id;
}

/**
 * Set the id of the memory.
 * @param memory_id the new id.
 * @exception DBException if there is an error with the update.
 */
void Egg::memory_id(ID memory_id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Egg\n\
SET memory_id = ?\n\
WHERE bug_id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, memory_id);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

}
}
