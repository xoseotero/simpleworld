/**
 * @file simpleworld/db/registers.cpp
 * Information about the registers of a bug.
 *
 *  Copyright (C) 2013  Xosé Otero <xoseotero@gmail.com>
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

#include <sqlite3.h>

#include <simpleworld/cpu/isa.hpp>
#include "exception.hpp"
#include "registers.hpp"

namespace simpleworld
{
namespace db
{

/**
 * Constructor.
 * It's not checked if the id is in the table, only when accessing the data
 * the id is checked.
 * @param db database.
 * @param id id of the registers.
 */
Registers::Registers(DB* db, ID id)
  : Table("Registers", db, id)
{
}


/**
 * Insert the registers of a bug.
 * @param db database.
 * @param data registers of the bug.
 * @param size size of the registers.
 * @return the id of the new row.
 * @exception DBException if there is an error with the insertion.
 */
ID Registers::insert(DB* db, const void* data, Uint32 size)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
INSERT INTO Registers(data)\n\
VALUES(?);", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_bind_blob(stmt, 1, data, size, SQLITE_TRANSIENT);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_finalize(stmt);

  return sqlite3_last_insert_rowid(db->db());
}

/**
 * Insert a row with all the registers set to 0.
 * @param db database.
 * @return the id of the new row.
 * @exception DBException if there is an error with the insertion.
 */
ID Registers::insert(DB* db)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
    INSERT INTO Registers(data)\n\
    VALUES(?);", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_bind_zeroblob(stmt, 1, TOTAL_REGISTERS * sizeof(cpu::Word));
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_finalize(stmt);

  return sqlite3_last_insert_rowid(db->db());
}

/**
 * Delete the registers of a bug.
 * @param db database.
 * @param id id of the registers.
 * @exception DBException if there is an error with the deletion.
 */
void Registers::remove(DB* db, ID id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
DELETE FROM Registers\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_bind_int64(stmt, 1, id);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_finalize(stmt);
}


/**
 * Set the id of the registers.
 * @param id the new id.
 * @exception DBException if there is an error with the update.
 */
void Registers::id(ID id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Registers\n\
SET id = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, id);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);

  this->id_ = id;
}


/**
 * Get the registers of the bug.
 * @return the registers.
 * @exception DBException if there is an error with the query.
 */
Blob Registers::data() const
{
  return Blob(this->db_, "Registers", "data", this->id_);
}

}
}
