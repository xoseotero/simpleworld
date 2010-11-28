/**
 * @file simpleworld/db/alivebug.hpp
 * Information about an alive bug.
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

#ifndef SIMPLEWORLD_DB_ALIVEBUG_HPP
#define SIMPLEWORLD_DB_ALIVEBUG_HPP

#include <simpleworld/db/types.hpp>
#include <simpleworld/db/db.hpp>
#include <simpleworld/db/table.hpp>
#include <simpleworld/db/blob.hpp>
#include <simpleworld/db/egg.hpp>

namespace simpleworld
{
namespace db
{

/**
 * Information about an alive bug.
 */
class AliveBug: public Table
{
public:
  /**
   * Constructor.
   * It's not checked if the id is in the table, only when accessing the data
   * the id is checked.
   * @param db database.
   * @param bug_id id of the bug.
   */
  AliveBug(DB* db, ID bug_id);


  /**
   * Insert a alive bug.
   * @param db database.
   * @param bug_id id of the bug.
   * @param world_id id of the world.
   * @param birth birth time.
   * @param energy energy.
   * @param registers data stored in the registers.
   * @param size size of the data.
   * @return the id of the new row (the same as bug_id).
   * @exception DBException if there is an error with the insertion.
   */
  static ID insert(DB* db, ID bug_id, ID world_id, Time birth, Energy energy,
		   const void* registers, Uint32 size);

  /**
   * Insert a alive bug from a egg.
   * The egg is deleted.
   * @param db database.
   * @param egg egg to convert in a alive bug.
   * @param birth birth time.
   * @return the id of the new row (the same as the bug_id of the egg).
   * @exception DBException if there is an error with the insertion.
   */
  static ID insert(DB* db, Egg* egg, Time birth);

  /**
   * Delete a alive bug.
   * @param db database.
   * @param id id of the bug.
   * @exception DBException if there is an error with the deletion.
   */
  static void remove(DB* db, ID id);


  /**
   * Get the id of the bug.
   * @return the id.
   * @exception DBException if there is an error with the query.
   */
  ID bug_id() const;

  /**
   * Set the id of the bug.
   * @param bug_id the new id.
   * @exception DBException if there is an error with the update.
   */
  void bug_id(ID bug_id);


  /**
   * Get the id of the world..
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
   * Get the birth of the bug.
   * @return the birth of the bug.
   * @exception DBException if there is an error with the query.
   */
  Time birth() const;

  /**
   * Set the birth of the bug.
   * @param birth the new birth of the bug.
   * @exception DBException if there is an error with the update.
   */
  void birth(Time birth);

  /**
   * Get the energy of the bug.
   * @return the energy of the bug.
   * @exception DBException if there is an error with the query.
   */
  Energy energy() const;

  /**
   * Set the energy of the bug.
   * @param energy the new energy of the bug.
   * @exception DBException if there is an error with the update.
   */
  void energy(Energy energy);


  /**
   * Get when the bug did the last action.
   * time_last_action is NULL if no action was done yet.
   * @return the time.
   * @exception DBException if there is an error with the query.
   */
  Time time_last_action() const;

  /**
   * Set when the bug did the last action.
   * @param time_last_action the new time.
   * @exception DBException if there is an error with the update.
   */
  void time_last_action(Time time_last_action);

  /**
   * Get when the bug will finish the current action.
   * action_time is NULL if the bug is not doing a action.
   * @return the time.
   * @exception DBException if there is an error with the query.
   */
  Time action_time() const;

  /**
   * Set when the bug will finish the current action.
   * @param action_time the new time.
   * @exception DBException if there is an error with the update.
   */
  void action_time(Time action_time);


  /**
   * Get the registers of the bug.
   * @return the registers.
   * @exception DBException if there is an error with the query.
   */
  Blob registers() const;
};

}
}

#endif // SIMPLEWORLD_DB_ALIVEBUG_HPP
