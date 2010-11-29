/**
 * @file simpleworld/db/blob.cpp
 * A binary large object in a table.
 *
 *  Copyright (C) 2010  Xosé Otero <xoseotero@gmail.com>
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

#include "exception.hpp"
#include "blob.hpp"

namespace simpleworld
{
namespace db
{

/**
 * Constructor.
 * It's not checked if the id is in the table, only when accessing the data
 * the id is checked.
 * @param db database.
 * @param table name of the table.
 * @param column name of the column.
 * @param id id of the row.
 */
Blob::Blob(DB* db, const std::string& table, const std::string& column, ID id)
  : db_(db), table_(table), column_(column), id_(id)
{
}


/**
 * Get the size of the data.
 * @return the size.
 * @exception DBException if there is an error with the query.
 */
Uint32 Blob::size() const
{
  // In sqlite3 prepared statments ? can only be used in parameters not
  // instead of tables/columns
  std::string query(boost::str(boost::format("\
SELECT length(%1%)\n\
FROM %2%\n\
WHERE _ROWID_ = ?;")
                               % this->column_
                               % this->table_));
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), query.c_str(), query.size(), &stmt,
                         NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table %2%")
                                            % this->id_
                                            % this->column_));
  Uint32 size = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return size;
}

/**
 * Get the data and its size.
 * @param size where to store the size of the data.
 * @return the data.
 * @exception DBException if there is an error with the query.
 */
boost::shared_array<Uint8> Blob::read(Uint32* size) const
{
  // In sqlite3 prepared statments ? can only be used in parameters not
  // instead of tables/columns
  std::string query(boost::str(boost::format("\
SELECT %1%\n\
FROM %2%\n\
WHERE _ROWID_ = ?;")
                               % this->column_
                               % this->table_));
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), query.c_str(), query.size(), &stmt,
                         NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table %2%")
                                              % this->id_
                                              % this->column_));
  const void* blob = sqlite3_column_blob(stmt, 0);
  int s = sqlite3_column_bytes(stmt, 0);
  boost::shared_array<Uint8> data(new Uint8[s]);
  std::memcpy(data.get(), blob, s);
  *size = s;
  return data;
}

/**
 * Get part of the data.
 * @param n bytes to get.
 * @param offset offset of the data.
 * @return the data.
 * @exception DBException if there is an error with the query.
 */
boost::shared_array<Uint8> Blob::read(Uint32 n, Uint32 offset) const
{
  sqlite3_blob* blob;
  if (sqlite3_blob_open(this->db_->db(), "main", this->table_.c_str(),
                        this->column_.c_str(), this->id_, 0, &blob))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  boost::shared_array<Uint8> data(new Uint8[n]);
  if (sqlite3_blob_read(blob, data.get(), n, offset)) {
    // sqlite3_errmsg() must be called right after the error
    std::string error(sqlite3_errmsg(this->db_->db()));
    sqlite3_blob_close(blob);
    throw EXCEPTION(DBException, error);
  }
  if (sqlite3_blob_close(blob))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));

  return data;
}


/**
 * Replace the data.
 * @param data the new data.
 * @param size the size of the data.
 * @exception DBException if there is an error with the query.
 */
void Blob::write(const void* data, Uint32 size)
{
  // In sqlite3 prepared statments ? can only be used in parameters not
  // instead of tables/columns
  std::string query(boost::str(boost::format("\
UPDATE %1%\n\
SET %2% = ?\n\
WHERE _ROWID_ = ?;")
                               % this->table_
                               % this->column_));
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), query.c_str(), query.size(), &stmt,
                         NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_blob(stmt, 1, data, size, SQLITE_TRANSIENT);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

/**
 * Replace part of the data.
 * @param data the new data.
 * @param n size of the data.
 * @param offset offset of the data.
 * @exception DBException if there is an error with the query.
 */
void Blob::write(const void* data, Uint32 n, Uint32 offset)
{
  sqlite3_blob* blob;
  if (sqlite3_blob_open(this->db_->db(), "main", this->table_.c_str(),
                        this->column_.c_str(), this->id_, 1, &blob))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  if (sqlite3_blob_write(blob, data, n, offset)) {
    // sqlite3_errmsg() must be called just after the error
    std::string error(sqlite3_errmsg(this->db_->db()));
    sqlite3_blob_close(blob);
    throw EXCEPTION(DBException, error);
  }
  if (sqlite3_blob_close(blob))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
}

}
}
