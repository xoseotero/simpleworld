/**
 * @file simpleworld/db/code.cpp
 * Information about the code of a bug.
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

#include "exception.hpp"
#include "code.hpp"

namespace simpleworld
{
namespace db
{

/**
 * Constructor.
 * It's not checked if the id is in the table, only when accessing the data
 * the id is checked.
 * @param db database.
 * @param id id of the code.
 */
Code::Code(DB* db, ID id)
  : Table("Code", db, id)
{
}


/**
 * Insert the code of a bug.
 * @param db database.
 * @param data code of the bug.
 * @param size size of the code.
 * @return the id of the new row.
 * @exception DBException if there is an error with the insertion.
 */
ID Code::insert(DB* db, const void* data, Uint32 size)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
INSERT INTO Code(data)\n\
VALUES(?);", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_bind_blob(stmt, 1, data, size, SQLITE_TRANSIENT);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_finalize(stmt);

  return sqlite3_last_insert_rowid(db->db());
}

/**
 * Copy the code of a bug.
 * @param db database.
 * @param code_id the id of the original code.
 * @return the id of the new row.
 * @exception DBException if there is an error with the insertion.
 */
ID Code::insert(DB* db, ID code_id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
INSERT INTO Code(data)\n\
SELECT data\n\
FROM Code\n\
WHERE id = ?", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_bind_int64(stmt, 1, code_id);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_finalize(stmt);

  return sqlite3_last_insert_rowid(db->db());
}

/**
 * Delete the code of a bug.
 * @param db database.
 * @param id id of the code.
 * @exception DBException if there is an error with the deletion.
 */
void Code::remove(DB* db, ID id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
DELETE FROM Code\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_bind_int64(stmt, 1, id);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_finalize(stmt);
}


/**
 * Set the id of the code.
 * @param id the new id.
 * @exception DBException if there is an error with the update.
 */
void Code::id(ID id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Code\n\
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
 * Get the code of the bug.
 * @return the code.
 * @exception DBException if there is an error with the query.
 */
Blob Code::data() const
{
  return Blob(this->db_, "Code", "data", this->id_);
}

}
}
