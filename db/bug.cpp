/**
 * @file db/bug.cpp
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

#include "bug.hpp"

#include <sqlite3x.hpp>

namespace SimpleWorld
{
namespace DB
{

Bug::Bug(DB* db, ID id)
  : Table(db, id)
{
}

Bug::~Bug()
{
  this->db_->free_bug(this->id_);
}


void Bug::update()
{
  static sqlite3x::sqlite3_command sql(*this->db_,
				       "\
SELECT order_world, energy, position_x, position_y, orientation,\n\
       birth, dead, killer_id\n\
FROM Bug\n\
WHERE id = ?;");
  sql.bind(1, this->id_);

  try {
    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw IDNotFound(__FILE__, __LINE__);

    this->order_world = cursor.getint(0);
    this->energy = cursor.getint(1);
    this->position_x = cursor.getint(2);
    this->position_y = cursor.getint(3);
    this->orientation = cursor.getint(4);
    this->birth = cursor.getint(5);
    this->dead = cursor.getint(6);
    this->killer = cursor.getint64(7);

    if (cursor.isnull(0))
      this->add_null("order_world");
    if (cursor.isnull(6))
      this->add_null("dead");
    if (cursor.isnull(7))
      this->add_null("killer_id");
  } catch (sqlite3x::database_error) {
    throw DBError(__FILE__, __LINE__);
  }
}

void Bug::update_db()
{
  static sqlite3x::sqlite3_command sql(*this->db_,
				       "\
UPDATE bug\n\
SET order_world = ?, energy = ?,\n\
    position_x = ?, position_y = ?, orientation = ?,\n\
    birth = ? , dead = ?, killer_id = ?\n\
WHERE id = ?;");
  if (this->is_null("order_world"))
    sql.bind(1);
  else
    sql.bind(1, static_cast<int>(this->order_world));
  sql.bind(2, static_cast<int>(this->energy));
  sql.bind(3, static_cast<int>(this->position_x));
  sql.bind(4, static_cast<int>(this->position_y));
  sql.bind(5, static_cast<int>(this->orientation));
  sql.bind(6, static_cast<int>(this->birth));
  if (this->is_null("dead"))
    sql.bind(7);
  else
    sql.bind(7, static_cast<int>(this->dead));
  if (this->is_null("killer_id"))
    sql.bind(8);
  else
    sql.bind(8, static_cast<sqlite3x::int64_t>(this->killer));
  sql.bind(9, this->id_);

  try {
    sql.executenonquery();
  } catch (sqlite3x::database_error) {
    throw DBError(__FILE__, __LINE__);
  }
}

}
}
