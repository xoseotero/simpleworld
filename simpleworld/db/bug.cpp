/**
 * @file simpleworld/db/bug.cpp
 * Information about a bug.
 *
 * begin:     Sat, 24 Feb 2007 13:32:56 +0100
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

#include <simpleworld/cpu/types.hpp>
#include <simpleworld/cpu/memory.hpp>

#include "bug.hpp"

namespace SimpleWorld
{
namespace DB
{

Bug::Bug(DB* db, ID id)
  // The position at this moment is unknown
  : AliveBug(db, id, ElementBug, this->position), cpu(db, id)
{
  this->update();
}

Bug::Bug(DB* db, Egg* egg)
  : AliveBug(db, egg->id(), ElementBug, egg->position), cpu(db)
{
  this->cpu.insert(egg->id());

  this->update();
  this->type = ElementBug;
}


void Bug::update()
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try{
    sql.prepare("\
SELECT energy, position_x, position_y, orientation,\n\
       birth, dead, father_id, killer_id\n\
FROM Bug\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw IDNotFound(__FILE__, __LINE__);

    this->energy = cursor.getint(0);
    this->position.x = cursor.getint(1);
    this->position.y = cursor.getint(2);
    this->orientation = static_cast<Orientation>(cursor.getint(3));
    this->birth = cursor.getint(4);
    this->father_id = cursor.getint64(5);

    if (cursor.isnull(5))
      this->add_null("father_id");
  } catch (const sqlite3x::database_error& e) {
    throw DBError(__FILE__, __LINE__, e.what());
  }


  this->cpu.update();

  AliveBug::update();
}

void Bug::update_db(bool force)
{
  if (this->changed or force) {
    sqlite3x::sqlite3_command sql(*this->db_);

    try {
      sql.prepare("\
UPDATE Bug\n\
SET energy = ?, position_x = ?, position_y = ?, orientation = ?,\n\
    birth = ? , father_id = ?\n\
WHERE id = ?;");
      sql.bind(1, static_cast<int>(this->energy));
      sql.bind(2, static_cast<int>(this->position.x));
      sql.bind(3, static_cast<int>(this->position.y));
      sql.bind(4, static_cast<int>(this->orientation));
      sql.bind(5, static_cast<int>(this->birth));
      if (this->is_null("father_id"))
	sql.bind(6);
      else
	sql.bind(6, static_cast<sqlite3x::int64_t>(this->father_id));
      sql.bind(7, this->id_);

      sql.executenonquery();
    } catch (const sqlite3x::database_error& e) {
      throw DBError(__FILE__, __LINE__, e.what());
    }
  }

  this->cpu.update_db();

  AliveBug::update_db(force);
}

void Bug::remove()
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
DELETE FROM Bug\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw DBError(__FILE__, __LINE__, e.what());
  }


  // It's not needed to propagate the remove because the database do it
  // this->cpu.remove();

  AliveBug::remove();
}

}
}