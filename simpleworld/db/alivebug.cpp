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
  // Convert the alive bug to a dead bug
  this->type = ElementNothing;
  this->energy = 0;

  sqlite3x::sqlite3_command sql(*this->db_);
  try {
    sql.prepare("\
UPDATE Bug\n\
SET energy = ?, dead = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<int>(this->energy));
    sql.bind(2, static_cast<int>(dead));
    sql.bind(3, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, e.what());
  }


  // Create the food in the position of the dead bug
  Food food(this->db_);
  food.position = this->position;
  food.size = this->code.size;
  food.insert();

  return food.id();
}

ID AliveBug::die(Time dead, ID killer_id)
{
  // Convert the alive bug to a dead bug
  this->type = ElementNothing;
  this->energy = 0;

  sqlite3x::sqlite3_command sql(*this->db_);
  try {
    sql.prepare("\
UPDATE Bug\n\
SET energy = ?, dead = ?, killer_id = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<int>(this->energy));
    sql.bind(2, static_cast<int>(dead));
    sql.bind(3, static_cast<sqlite3x::int64_t>(killer_id));
    sql.bind(4, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, e.what());
  }


  // Create the food in the position of the dead bug
  Food food(this->db_);
  food.position = this->position;
  food.size = this->code.size;
  food.insert();

  return food.id();
}

}
}
