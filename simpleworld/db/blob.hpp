/**
 * @file simpleworld/db/blob.hpp
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

#ifndef SIMPLEWORLD_DB_BLOB_HPP
#define SIMPLEWORLD_DB_BLOB_HPP

#include <string>

#include <boost/shared_array.hpp>

#include <simpleworld/ints.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/db.hpp>

namespace simpleworld
{
namespace db
{

/**
 * A binary large object in a table.
 */
class Blob
{
public:
  /**
   * Constructor.
   * It's not checked if the id is in the table, only when accessing the data
   * the id is checked.
   * @param db database.
   * @param table name of the table.
   * @param column name of the column.
   * @param id id of the row.
   */
  Blob(DB* db, const std::string& table, const std::string& column, ID id);


  /**
   * Get the size of the data.
   * @return the size.
   * @exception DBException if there is an error with the query.
   */
  Uint32 size() const;

  /**
   * Get the data and its size.
   * @param size where to store the size of the data.
   * @return the data.
   * @exception DBException if there is an error with the query.
   */
  boost::shared_array<Uint8> read(Uint32* size) const;

  /**
   * Get part of the data.
   * @param n bytes to get.
   * @param offset offset of the data.
   * @return the data.
   * @exception DBException if there is an error with the query.
   */
  boost::shared_array<Uint8> read(Uint32 n, Uint32 offset) const;


  /**
   * Replace the data.
   * @param data the new data.
   * @param size the size of the data.
   * @exception DBException if there is an error with the query.
   */
  void write(const void* data, Uint32 size);

  /**
   * Replace part of the data.
   * @param data the new data.
   * @param n size of the data.
   * @param offset offset of the data.
   * @exception DBException if there is an error with the query.
   */
  void write(const void* data, Uint32 n, Uint32 offset);

private:
  DB* db_;                      /**< DB connection */
  std::string table_;           /**< Name of the table */
  std::string column_;          /**< Name of the column */
  ID id_;                       /**< ID of the row */
};

}
}

#endif // SIMPLEWORLD_DB_BLOB_HPP
