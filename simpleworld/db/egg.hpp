/**
 * @file simpleworld/db/egg.hpp
 * Information about an egg.
 *
 *  Copyright (C) 2007-2013  Xosé Otero <xoseotero@gmail.com>
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

#ifndef SIMPLEWORLD_DB_EGG_HPP
#define SIMPLEWORLD_DB_EGG_HPP

#include <simpleworld/ints.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/table.hpp>

namespace simpleworld
{
namespace db
{

/**
 * Information about an egg.
 */
class Egg: public Table
{
public:
  /**
   * Constructor.
   * It's not checked if the id is in the table, only when accessing the data
   * the id is checked.
   * @param db database.
   * @param bug_id id of the bug.
   */
  Egg(DB* db, ID bug_id);


  /**
   * Insert a egg.
   * @param db database.
   * @param bug_id id of the bug.
   * @param world_id id of the world.
   * @param energy energy.
   * @param memory_id id of the memory of the bug.
   * @return the id of the new row (the same as bug_id).
   * @exception DBException if there is an error with the insertion.
   */
  static ID insert(DB* db, ID bug_id, ID world_id, Energy energy, ID memory_id);

  /**
   * Delete a egg.
   * @param db database.
   * @param bug_id id of the egg.
   * @exception DBException if there is an error with the deletion.
   */
  static void remove(DB* db, ID bug_id);


  /**
   * Get the id of the egg.
   * @return the id.
   * @exception DBException if there is an error with the query.
   */
  ID bug_id() const;

  /**
   * Set the id of the egg.
   * @param bug_id the new id.
   * @exception DBException if there is an error with the update.
   */
  void bug_id(ID bug_id);


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
   * Get the energy of the egg.
   * @return the energy.
   * @exception DBException if there is an error with the query.
   */
  Energy energy() const;

  /**
   * Set the energy of the egg.
   * @param energy the new energy.
   * @exception DBException if there is an error with the update.
   */
  void energy(Energy energy);


  /**
   * Get the id of the memory.
   * @return the id.
   * @exception DBException if there is an error with the query.
   */
  ID memory_id() const;
  
  /**
   * Set the id of the memory.
   * @param memory_id the new id.
   * @exception DBException if there is an error with the update.
   */
  void memory_id(ID memory_id);
};

}
}

#endif // SIMPLEWORLD_DB_EGG_HPP
