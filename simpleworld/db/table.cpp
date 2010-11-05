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
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    // In sqlite3 prepared statments ? can only be used in parameters not
    // instead of tables/columns
    std::string query(boost::str(boost::format("\
SELECT %1%\n\
FROM %2%\n\
WHERE _ROWID_ = ?;")
				 % colname
				 % this->name_));
    sql.prepare(query);
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table AliveBug")
                                              % this->id_));

    return cursor.isnull(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set colname as NULL.
 * @param colname name of the column.
 */
void Table::set_null(const std::string& colname)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    // In sqlite3 prepared statments ? can only be used in parameters not
    // instead of tables/columns
    std::string query(boost::str(boost::format("\
UPDATE %1%\n\
SET %2% = ?\n\
WHERE _ROWID_ = ?;")
				 % this->name_
				 % colname));
    sql.prepare(query);
    sql.bind(1);
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

}
}
