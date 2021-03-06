/**
 * @file simpleworld/db/code.hpp
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

#ifndef SIMPLEWORLD_DB_CODE_HPP
#define SIMPLEWORLD_DB_CODE_HPP

#include <simpleworld/ints.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/table.hpp>
#include <simpleworld/db/blob.hpp>

namespace simpleworld
{
namespace db
{

/**
 * Information about the code of a bug.
 */
class Code: public Table
{
public:
  /**
   * Constructor.
   * It's not checked if the id is in the table, only when accessing the data
   * the id is checked.
   * @param db database.
   * @param id id of the code.
   */
  Code(DB* db, ID id);


  /**
   * Insert the code of a bug.
   * @param db database.
   * @param data code of the bug.
   * @param size size of the code.
   * @return the id of the new row.
   * @exception DBException if there is an error with the insertion.
   */
  static ID insert(DB* db, const void* data, Uint32 size);

  /**
   * Copy the code of a bug.
   * @param db database.
   * @param code_id the id of the original code.
   * @return the id of the new row.
   * @exception DBException if there is an error with the insertion.
   */
  static ID insert(DB* db, ID code_id);

  /**
   * Delete the code of a bug.
   * @param db database.
   * @param id id of the code.
   * @exception DBException if there is an error with the deletion.
   */
  static void remove(DB* db, ID id);


  /**
   * Get the id of the code.
   * @return the id of the code.
   * @exception DBException if there is an error with the query.
   */
  ID id() const { return db::Table::id(); }

  /**
   * Set the id of the code.
   * @param id the new id.
   * @exception DBException if there is an error with the update.
   */
  void id(ID id);


  /**
   * Get the code of the bug.
   * @return the code.
   * @exception DBException if there is an error with the query.
   */
  Blob data() const;
};

}
}

#endif // SIMPLEWORLD_DB_CODE_HPP
