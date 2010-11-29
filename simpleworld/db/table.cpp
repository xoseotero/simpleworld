/**
 * @file simpleworld/db/table.cpp
 * Base class for the tables.
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

#include <boost/format.hpp>

#include <sqlite3.h>

#include "exception.hpp"
#include "table.hpp"

namespace simpleworld
{
namespace db
{

/**
 * Constructor.
 * @param name name of the table.
 * @param db database.
 * @param id id of the row.
 */
Table::Table(const std::string& name, DB* db, ID id)
  : name_(name), db_(db), id_(id)
{
}


/**
 * Check if colname is NULL.
 * @param colname name of the column.
 * @return true if colname is NULL, else false.
 */
bool Table::is_null(const std::string& colname) const
{
  // In sqlite3 prepared statments ? can only be used in parameters not
  // instead of tables/columns
  std::string query(boost::str(boost::format("\
SELECT %1%\n\
FROM %2%\n\
WHERE _ROWID_ = ?;")
                               % colname
                               % this->name_));
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), query.c_str(), query.size(), &stmt,
                         NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table %2%")
                                            % this->id_
                                            % colname));
  bool is_null = sqlite3_column_type(stmt, 0) == SQLITE_NULL;
  sqlite3_finalize(stmt);

  return is_null;
}

/**
 * Set colname as NULL.
 * @param colname name of the column.
 */
void Table::set_null(const std::string& colname)
{
  // In sqlite3 prepared statments ? can only be used in parameters not
  // instead of tables/columns
  std::string query(boost::str(boost::format("\
UPDATE %1%\n\
SET %2% = ?\n\
WHERE _ROWID_ = ?;")
                               % this->name_
                               % colname));
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), query.c_str(), query.size(), &stmt,
                         NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_null(stmt, 1);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table %2%")
                                            % this->id_
                                            % colname));
  sqlite3_finalize(stmt);
}

}
}
