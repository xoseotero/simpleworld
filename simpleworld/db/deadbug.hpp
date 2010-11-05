/**
 * @file simpleworld/db/deadbug.hpp
 * Information about a dead bug.
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

#ifndef SIMPLEWORLD_DB_DEADBUG_HPP
#define SIMPLEWORLD_DB_DEADBUG_HPP

#include <simpleworld/db/types.hpp>
#include <simpleworld/db/db.hpp>
#include <simpleworld/db/table.hpp>
#include <simpleworld/db/egg.hpp>
#include <simpleworld/db/alivebug.hpp>

namespace simpleworld
{
namespace db
{

/**
 * Information about a dead bug.
 */
class DeadBug: public Table
{
public:
  /**
   * Constructor.
   * It's not checked if the id is in the table, only when accessing the data
   * the id is checked.
   * @param db database.
   * @param bug_id id of the bug.
   */
  DeadBug(DB* db, ID bug_id);


  /**
   * Insert a dead bug without birth nor killer_id.
   * @param db database.
   * @param bug_id id of the bug.
   * @param death when the bug had dead.
   * @return the id of the new row (the same as bug_id).
   * @exception DBException if there is an error with the insertion.
   */
  static ID insert(DB* db, ID bug_id, Time death);

  /**
   * Insert a dead bug with birth but withoutr killer_id.
   * @param db database.
   * @param bug_id id of the bug.
   * @param birth when the bug had born.
   * @param death when the bug had dead.
   * @return the id of the new row (the same as bug_id).
   * @exception DBException if there is an error with the insertion.
   */
  static ID insert(DB* db, ID bug_id, Time birth, Time death);

  /**
   * Insert a dead bug with killer_id but without birth.
   * @param db database.
   * @param bug_id id of the bug.
   * @param death when the bug had dead.
   * @param killer_id who had killed the bug.
   * @return the id of the new row (the same as bug_id).
   * @exception DBException if there is an error with the insertion.
   */
  static ID insert(DB* db, ID bug_id, Time death, ID killer_id);

  /**
   * Insert a dead bug with birth and killer_id.
   * @param db database.
   * @param bug_id id of the bug.
   * @param birth when the bug had born.
   * @param death when the bug had dead.
   * @param killer_id who had killed the bug.
   * @return the id of the new row (the same as bug_id).
   * @exception DBException if there is an error with the insertion.
   */
  static ID insert(DB* db, ID bug_id, Time birth, Time death, ID killer_id);

  /**
   * Insert a dead bug from a egg without killer_id.
   * The egg is deleted.
   * @param db database.
   * @param egg egg to kill.
   * @param death when the egg had dead.
   * @return the id of the new row (the same as the egg id).
   * @exception DBException if there is an error with the insertion.
   */
  static ID insert(DB* db, Egg* egg, Time death);

  /**
   * Insert a dead bug from a egg with killer_id.
   * The egg is deleted.
   * @param db database.
   * @param egg egg to kill.
   * @param death when the egg had dead.
   * @param killer_id who had killed the egg.
   * @return the id of the new row (the same as the egg id).
   * @exception DBException if there is an error with the insertion.
   */
  static ID insert(DB* db, Egg* egg, Time death, ID killer_id);

  /**
   * Insert a dead bug from a alive bug without killer_id.
   * The alive bug is deleted.
   * @param db database.
   * @param alivebug bug to kill.
   * @param death when the bug had dead.
   * @return the id of the new row (the same as the bug id).
   * @exception DBException if there is an error with the insertion.
   */
  static ID insert(DB* db, AliveBug* alivebug, Time death);

  /**
   * Insert a dead bug from a alive bug with killer_id.
   * The alive bug is deleted.
   * @param db database.
   * @param alivebug bug to kill.
   * @param death when the bug had dead.
   * @param killer_id who had killed the bug.
   * @return the id of the new row (the same as the bug id).
   * @exception DBException if there is an error with the insertion.
   */
  static ID insert(DB* db, AliveBug* alivebug, Time death, ID killer_id);

  /**
   * Delete a dead bug.
   * @param db database.
   * @param bug_id id of the bug..
   * @exception DBException if there is an error with the deletion.
   */
  static void remove(DB* db, ID bug_id);


  /**
   * Get the id of the bug.
   * @return the id.
   * @exception DBException if there is an error with the query.
   */
  ID bug_id() const;

  /**
   * Set the id of the bug.
   * @param id the new id.
   * @exception DBException if there is an error with the update.
   */
  void bug_id(ID bug_id);


  /**
   * Get when the bug had born.
   * It's NULL if the bug haven't born (it was a egg when died).
   * @return the time.
   * @exception DBException if there is an error with the query.
   */
  Time birth() const;

  /**
   * Set when the bug had born.
   * @param birth the new time.
   * @exception DBException if there is an error with the update.
   */
  void birth(Time birth);

  /**
   * Get when the bug had dead.
   * @return the time.
   * @exception DBException if there is an error with the query.
   */
  Time death() const;

  /**
   * Set when the bug had dead.
   * @param death the new time.
   * @exception DBException if there is an error with the update.
   */
  void death(Time death);

  /**
   * Get who had killed the bug.
   * It's NULL if the bug haven't been killed by a bug.
   * @return the id.
   * @exception DBException if there is an error with the query.
   */
  ID killer_id() const;

  /**
   * Set who had killed the bug.
   * @param killer_id the id.
   * @exception DBException if there is an error with the update.
   */
  void killer_id(ID killer_id);
};

}
}

#endif // SIMPLEWORLD_DB_DEADBUG_HPP
