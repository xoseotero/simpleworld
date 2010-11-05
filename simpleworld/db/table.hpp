/**
 * @file simpleworld/db/table.hpp
 * Base clase for the tables.
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

#ifndef SIMPLEWORLD_DB_TABLE_HPP
#define SIMPLEWORLD_DB_TABLE_HPP

#include <vector>
#include <string>
#include <algorithm>

#include <simpleworld/db/types.hpp>
#include <simpleworld/db/db.hpp>

namespace simpleworld
{
namespace db
{

/**
 * Base clase for the tables.
 */
class Table
{
public:
  /**
   * Constructor.
   * @param name name of the table.
   * @param db database.
   * @param id id of the row.
   */
  Table(const std::string& name, DB* db, ID id);


  /**
   * Name of the table.
   */
  std::string name() const { return this->name_; }

  /**
   * The database of the table.
   * @return the database object.
   */
  DB* db() const { return this->db_; }

  /**
   * The id of the table.
   * @return the ID.
   */
  ID id() const { return this->id_; }


  /**
   * Check if colname is NULL.
   * @param colname name of the column.
   * @return true if colname is NULL, else false.
   */
  bool is_null(const std::string& colname) const;

  /**
   * Set colname as NULL.
   * @param colname name of the column.
   */
  void set_null(const std::string& colname);

protected:
  std::string name_;            /**< Name of the table */
  DB* db_;                      /**< DB connection */
  ID id_;                       /**< ID of the row */
};

}
}

#endif // SIMPLEWORLD_DB_TABLE_HPP
