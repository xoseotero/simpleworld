/**
 * @file simpleworld/db/bug.hpp
 * Information about a Bug.
 *
 *  Copyright (C) 2007-2011  Xosé Otero <xoseotero@gmail.com>
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

#ifndef SIMPLEWORLD_DB_BUG_HPP
#define SIMPLEWORLD_DB_BUG_HPP

#include <vector>

#include <simpleworld/ints.hpp>
#include <simpleworld/types.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/db.hpp>
#include <simpleworld/db/table.hpp>
#include <simpleworld/db/blob.hpp>

namespace simpleworld
{
namespace db
{

/**
 * Information about a bug.
 */
class Bug: public Table
{
public:
  /**
   * Constructor.
   * It's not checked if the id is in the table, only when accessing the data
   * the id is checked.
   * @param db database.
   * @param id id of the bug.
   */
  Bug(DB* db, ID id);


  /**
   * Insert a bug with father.
   * @param db database.
   * @param creation when the egg was created.
   * @param father_id id of the father.
   * @param code code of the bug.
   * @param size size of the code.
   * @return the id of the new row.
   * @exception DBException if there is an error with the insertion.
   */
  static ID insert(DB* db, Time creation, ID father_id,
                   const void* code, Uint32 size);

  /**
   * Insert a bug without father.
   * @param db database.
   * @param creation when the egg was created.
   * @param code code of the bug.
   * @param size size of the code.
   * @return the id of the new row.
   * @exception DBException if there is an error with the insertion.
   */
  static ID insert(DB* db, Time creation, const void* code, Uint32 size);

  /**
   * Delete a bug.
   * @param db database.
   * @param id id of the bug.
   * @exception DBException if there is an error with the deletion.
   */
  static void remove(DB* db, ID id);


  /**
   * Get the id of the bug.
   * @return the id of the food.
   * @exception DBException if there is an error with the query.
   */
  ID id() const { return db::Table::id(); }

  /**
   * Set the id of the bug.
   * @param id the new id.
   * @exception DBException if there is an error with the update.
   */
  void id(ID id);


  /**
  * Get when the egg was created.
  * @return the time.
  * @exception DBException if there is an error with the query.
  */
  Time creation() const;

  /**
  * Set when the egg was created.
  * @param creation the new time.
  * @exception DBException if there is an error with the update.
  */
  void creation(Time creation);


  /**
   * Get the id of the father.
   * father_id is NULL if the bug hasn't got a father.
   * @return the id of the father.
   * @exception DBException if there is an error with the query.
   */
  ID father_id() const;

  /**
   * Set the id of the father.
   * @param father_id the id of the father.
   * @exception DBException if there is an error with the query.
   */
  void father_id(ID father_id);

  /**
  * Get the ancestors of the bug.
  * @return the ancestors.
  * @exception DBException if there is an error with the query.
  */
  std::vector<ID> ancestors() const;


  /**
   * Get the code of the bug.
   * @return the code of the bug.
   * @exception DBException if there is an error with the query.
   */
  Blob code() const;


  /**
   * Get the mutations of the bug.
   * @return the mutations.
   * @exception DBException if there is an error with the query.
   */
  std::vector<ID> mutations() const;

  /**
  * Get the mutations of the ancestors of the bug and the bug.
  * @return the mutations.
  * @exception DBException if there is an error with the query.
  */
  std::vector<ID> all_mutations() const;
};

}
}

#endif // SIMPLEWORLD_DB_BUG_HPP
