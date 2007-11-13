/**
 * @file simpleworld/db/food.cpp
 * Information about the food
 *
 * begin:     Sun, 29 Jul 2007 23:21:13 +0200
 * last:      $Date$
 *
 *  Copyright (C) 2007  Xosé Antón Otero Ferreira <xoseotero@gmail.com>
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

#include "food.hpp"

namespace SimpleWorld
{
namespace DB
{

Food::Food(DB* db, ID id)
  // The position at this moment is unknown
  : Table(db, id), Element(ElementFood, this->position)
{
  this->update();
}

Food::Food(DB* db)
  // The position at this moment is unknown
  : Table(db), Element(ElementFood, this->position)
{
}


void Food::update()
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT position_x, position_y, size\n\
FROM Food\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw IDNotFound(__FILE__, __LINE__);

    this->position.x = cursor.getint(0);
    this->position.y = cursor.getint(1);
    this->size = cursor.getint(2);
  } catch (const sqlite3x::database_error& e) {
    throw DBError(__FILE__, __LINE__, e.what());
  }


  Table::update();
}

void Food::update_db(bool force)
{
  if (this->changed or force) {
    sqlite3x::sqlite3_command sql(*this->db_);

    try {
      sql.prepare("\
UPDATE Food\n\
SET position_x = ?, position_y = ?, size = ?\n\
WHERE id = ?;");
      sql.bind(1, static_cast<int>(this->position.x));
      sql.bind(2, static_cast<int>(this->position.y));
      sql.bind(3, static_cast<int>(this->size));
      sql.bind(4, this->id_);

      sql.executenonquery();
    } catch (const sqlite3x::database_error& e) {
      throw DBError(__FILE__, __LINE__, e.what());
    }
  }


  Table::update_db(force);
}

void Food::insert()
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
INSERT INTO Food(position_x, position_y, size)\n\
VALUES(?, ?, ?);");
    sql.bind(1, static_cast<int>(this->position.x));
    sql.bind(2, static_cast<int>(this->position.y));
    sql.bind(3, static_cast<int>(this->size));

    sql.executenonquery();
    this->id_ = this->db_->insertid();
  } catch (const sqlite3x::database_error& e) {
    throw DBError(__FILE__, __LINE__, e.what());
  }


  Table::insert();
}

void Food::remove()
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
DELETE FROM Food\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw DBError(__FILE__, __LINE__, e.what());
  }


  Table::remove();
}

}
}
