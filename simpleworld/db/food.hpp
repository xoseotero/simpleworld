/**
 * @file simpleworld/db/food.hpp
 * Information about the food
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

#ifndef SIMPLEWORLD_DB_FOOD_HPP
#define SIMPLEWORLD_DB_FOOD_HPP

#include <simpleworld/types.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/db.hpp>
#include <simpleworld/db/table.hpp>

namespace simpleworld
{
namespace db
{

/**
 * Information about the food
 */
class Food: public Table
{
public:
  /**
   * Constructor.
   * It's not checked if the id is in the table, only when accessing the data
   * the id is checked.
   * @param db database.
   * @param id id of the food.
   */
  Food(DB* db, ID id);


  /**
   * Insert a food.
   * @param db database.
   * @param time when the food was added.
   * @param world_id id of the world.
   * @param size size.
   * @return the id of the new row.
   * @exception DBException if there is an error with the insertion.
   */
  static ID insert(DB* db, Time time, ID world_id, Energy size);

  /**
   * Delete a food.
   * @param db database.
   * @param id id of the food.
   * @exception DBException if there is an error with the deletion.
   */
  static void remove(DB* db, ID id);


  /**
   * Get the id of the food.
   * @return the id.
   * @exception DBException if there is an error with the query.
   */
  ID id() const { return db::Table::id(); }

  /**
   * Set the id of the food.
   * @param id the new id.
   * @exception DBException if there is an error with the update.
   */
  void id(ID id);


  /**
   * Get when the food was added.
   * @return the time.
   * @exception DBException if there is an error with the query.
   */
  Time time() const;

  /**
   * Set when the food was added.
   * @param time the new time.
   * @exception DBException if there is an error with the update.
   */
  void time(Time time);


  /**
   * Get the id of the world.
   * @return the id.
   * @exception DBException if there is an error with the query.
   */
  ID world_id() const;

  /**
   * Set the id of the world.
   * @param world_id the new id.
   * @exception DBException if there is an error with the update.
   */
  void world_id(ID world_id);


  /**
   * Get the size of the food.
   * @return the size.
   * @exception DBException if there is an error with the query.
   */
  Energy size() const;

  /**
   * Set the size of the food.
   * @param size the new size.
   * @exception DBException if there is an error with the update.
   */
  void size(Energy size);
};

}
}

#endif // SIMPLEWORLD_DB_FOOD_HPP
