/**
 * @file simpleworld/db/alivebug.cpp
 * Information about a alive bug.
 *
 * begin:     Wed, 15 Aug 2007 13:56:46 +0200
 * last:      $Date$
 *
 *  Copyright (C) 2007  Xosé Otero <xoseotero@users.sourceforge.net>
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

#include "exception.hpp"
#include "food.hpp"
#include "deadbug.hpp"
#include "alivebug.hpp"

namespace SimpleWorld
{
namespace DB
{

AliveBug::AliveBug(DB* db, ID id, ElementType type, Position position)
  : BugElement(db, id, type, position)
{
}

AliveBug::AliveBug(DB* db, ElementType type)
  : BugElement(db, type)
{
}


ID AliveBug::die(Time dead)
{
  this->energy = 0;
  this->update_db(true);

  // Convert the alive bug to a dead bug
  DeadBug deadbug(this->db_, this->id_);
  deadbug.dead = dead;
  deadbug.update_db(true);

  // Create the food in the position of the dead bug
  Food food(this->db_);
  food.position = this->position;
  food.size = this->code.size;
  food.insert();

  return food.id();
}

ID AliveBug::die(Time dead, ID killer_id)
{
  this->energy = 0;
  this->update_db(true);

  // Convert the alive bug to a dead bug
  DeadBug deadbug(this->db_, this->id_);
  deadbug.dead = dead;
  deadbug.killer_id = killer_id;
  deadbug.update_db(true);

  // Create the food in the position of the dead bug
  Food food(this->db_);
  food.position = this->position;
  food.size = this->code.size;
  food.insert();

  return food.id();
}

}
}
