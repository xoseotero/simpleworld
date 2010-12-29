/**
 * @file simpleworld/db/resource.hpp
 * Information about a resource.
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

#ifndef SIMPLEWORLD_DB_RESOURCE_HPP
#define SIMPLEWORLD_DB_RESOURCE_HPP

#include <simpleworld/ints.hpp>
#include <simpleworld/types.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/table.hpp>

namespace simpleworld
{
namespace db
{

/**
 * Information about a resource.
 */
class Resource: public Table
{
public:
  /**
   * Constructor.
   * It's not checked if the id is in the table, only when accessing the data
   * the id is checked.
   * @param db database.
   * @param id id of the resource.
   */
  Resource(DB* db, ID id);


  /**
   * Insert a resource.
   * @param db database.
   * @param frequency the frequency of the resource.
   * @param max maximum number of elements in the region.
   * @param start_x x coord of the start of the region.
   * @param start_y y coord of the start of the region.
   * @param end_x x coord of the end of the region.
   * @param end_y y coord of the end of the region.
   * @param size size of new food.
   * @return the id of the new row (the same as bug_id).
   * @exception DBException if there is an error with the insertion.
   */
  static ID insert(DB* db, Time frequency, Uint16 max,
                   Coord start_x, Coord start_y, Coord end_x, Coord end_y,
                   Energy size);

  /**
   * Delete a resource.
   * @param db database.
   * @param id id of the resource.
   * @exception DBException if there is an error with the deletion.
   */
  static void remove(DB* db, ID id);


  /**
   * Get the id of the resource.
   * @return the id of the resource.
   * @exception DBException if there is an error with the query.
   */
  ID id() const { return db::Table::id(); }

  /**
   * Set the id of the resource.
   * @param id the new id.
   * @exception DBException if there is an error with the update.
   */
  void id(ID id);


  /**
   * Get the frequency of the resource.
   * @return the frequency.
   * @exception DBException if there is an error with the query.
   */
  Time frequency() const;

  /**
   * Set the frequency of the resource.
   * @param frequency the new frequency.
   * @exception DBException if there is an error with the update.
   */
  void frequency(Time frequency);


  /**
   * Get the maximum number of elements in the region.
   * @return the maximum.
   * @exception DBException if there is an error with the query.
   */
  Uint16 max() const;

  /**
   * Set the maximum number of elements in the region.
   * @param max the new maximum.
   * @exception DBException if there is an error with the update.
   */
  void max(Uint16 max);

  /**
   * Get the x coord of the start of the region.
   * @return the x coord.
   * @exception DBException if there is an error with the query.
   */
  Coord start_x() const;

  /**
   * Set the x coord of the start of the region.
   * @param start_x the new x coord.
   * @exception DBException if there is an error with the update.
   */
  void start_x(Coord start_x);

  /**
   * Get the y coord of the start of the region.
   * @return the y coord.
   * @exception DBException if there is an error with the query.
   */
  Coord start_y() const;

  /**
   * Set the y coord of the start of the region.
   * @param start_y the new x coord.
   * @exception DBException if there is an error with the update.
   */
  void start_y(Coord start_y);

  /**
   * Get the x coord of the end of the region.
   * @return the x coord.
   * @exception DBException if there is an error with the query.
   */
  Coord end_x() const;

  /**
   * Set the x coord of the end of the region.
   * @param end_x the new x coord.
   * @exception DBException if there is an error with the update.
   */
  void end_x(Coord end_x);

  /**
   * Get the y coord of the end of the region.
   * @return the y coord.
   * @exception DBException if there is an error with the query.
   */
  Coord end_y() const;

  /**
   * Set the y coord of the end of the region.
   * @param end_y the new y coord.
   * @exception DBException if there is an error with the update.
   */
  void end_y(Coord end_y);


  /**
   * Get the size of new food.
   * @return the size.
   * @exception DBException if there is an error with the query.
   */
  Energy size() const;

  /**
   * Set the size of new food.
   * @param size the new size.
   * @exception DBException if there is an error with the update.
   */
  void size(Energy size);
};

}
}

#endif // SIMPLEWORLD_DB_RESOURCE_HPP
