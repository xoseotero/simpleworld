/**
 * @file simpleworld/db/alivebug.cpp
 * Information about a alive bug.
 *
 * begin:     Wed, 15 Aug 2007 13:56:46 +0200
 * last:      $Date$
 *
 *  Copyright (C) 2007-2008  Xosé Otero <xoseotero@users.sourceforge.net>
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

#include <sqlite3x.hpp>

#include <simpleworld/types.hpp>
#include "exception.hpp"
#include "food.hpp"
#include "deadbug.hpp"
#include "alivebug.hpp"

namespace simpleworld
{
namespace db
{

/**
 * Constructor.
 * @param db database.
 * @param id id of the bug.
 * @param type type of element
 * @param position position of the bug.
 * @exception DBException if there is a error in the database.
 * @exception DBException if the ID is not found in the table.
 */
AliveBug::AliveBug(DB* db, ID id, ElementType type, Position position)
  : BugElement(db, id, type, position)
{
}

/**
 * Constructor to insert data.
 * @param db database.
 * @param type type of element
 * @exception DBException if there is a error in the database.
 */
AliveBug::AliveBug(DB* db, ElementType type)
  : BugElement(db, type)
{
}


/**
 * Convert the alive bug in a dead bug.
 * @param dead When the bug dead.
 * @return The id of the new food.
 */
ID AliveBug::die(Time dead)
{
  Energy energy = this->energy;

  this->energy = 0;
  this->update_db(true);

  // Convert the alive bug to a dead bug
  DeadBug deadbug(this->db_, this->id_);
  deadbug.dead = dead;
  deadbug.add_null("killer_id");
  deadbug.update_db(true);

  // Create the food in the position of the dead bug
  Food food(this->db_);
  food.position = this->position;
  food.size = this->code.size + energy;
  food.insert();

  return food.id();
}

/**
 * Convert the alive bug in a dead bug.
 * @param dead When the bug dead.
 * @param killer_id Who killed it.
 * @return The id of the new food.
 */
ID AliveBug::die(Time dead, ID killer_id)
{
  Energy energy = this->energy;

  this->energy = 0;
  this->update_db(true);

  // Convert the alive bug to a dead bug
  DeadBug deadbug(this->db_, this->id_);
  deadbug.dead = dead;
  deadbug.remove_null("killer_id");
  deadbug.killer_id = killer_id;
  deadbug.update_db(true);

  // Create the food in the position of the dead bug
  Food food(this->db_);
  food.position = this->position;
  food.size = this->code.size + energy;
  food.insert();

  return food.id();
}

}
}
