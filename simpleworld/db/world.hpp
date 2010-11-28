/**
 * @file simpleworld/db/world.hpp
 * Information about the world.
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

#ifndef SIMPLEWORLD_DB_WORLD_HPP
#define SIMPLEWORLD_DB_WORLD_HPP

#include <simpleworld/ints.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/table.hpp>

namespace simpleworld
{
namespace db
{

/**
 * Information about the world.
 */
class World: public Table
{
public:
  /**
   * Constructor.
   * It's not checked if the id is in the table, only when accessing the data
   * the id is checked.
   * @param db database.
   * @param id id of the world.
   */
  World(DB* db, ID id);


  /**
   * Insert a element in the world with orientation.
   * @param db database.
   * @param position_x position in the x coordinate of the element.
   * @param position_y position in the y coordinate of the element.
   * @param orientation orientation of the element.
   * @return the id of the new row.
   * @exception DBException if there is an error with the insertion.
   */
  static ID insert(DB* db, Coord position_x, Coord position_y,
                   Orientation orientation);

  /**
   * Insert a element in the world without orientation.
   * @param db database.
   * @param position_x position in the x coordinate of the element.
   * @param position_y position in the y coordinate of the element.
   * @return the id of the new row.
   * @exception DBException if there is an error with the insertion.
   */
  static ID insert(DB* db, Coord position_x, Coord position_y);

  /**
   * Delete a element from the world.
   * @param db database.
   * @param id id of the bug.
   * @exception DBException if there is an error with the deletion.
   */
  static void remove(DB* db, ID id);


  /**
   * Get the id of the world.
   * @return the id.
   * @exception DBException if there is an error with the query.
   */
  ID id() const { return db::Table::id(); }

  /**
   * Set the id of the world.
   * @param id the new id.
   * @exception DBException if there is an error with the update.
   */
  void id(ID id);


  /**
   * Get position in the x coordinate.
   * @return the position.
   * @exception DBException if there is an error with the query.
   */
  Coord position_x() const;

  /**
   * Get position in the y coordinate.
   * @return the position.
   * @exception DBException if there is an error with the query.
   */
  Coord position_y() const;

  /**
   * Set position in the x coordinate.
   * @param position_x the new position.
   * @exception DBException if there is an error with the update.
   */
  void position_x(Coord position_x);

  /**
   * Set position in the y coordinate.
   * @param position_y the new position.
   * @exception DBException if there is an error with the update.
   */
  void position_y(Coord position_y);


  /**
   * Get the orientation.
   * @return the orientation.
   * @exception DBException if there is an error with the query.
   */
  Orientation orientation() const;

  /**
   * Set the orientation.
   * @param orientation the new orientation.
   * @exception DBException if there is an error with the update.
   */
  void orientation(Orientation orientation);
};

}
}

#endif // SIMPLEWORLD_DB_WORLD_HPP
