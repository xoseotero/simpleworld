/**
 * @file simpleworld/db/mutation.cpp
 * A mutation of a bug.
 *
 * begin:     Thu, 01 Mar 2007 16:59:31 +0100
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

#include "mutation.hpp"

namespace SimpleWorld
{
namespace DB
{

Mutation::Mutation(DB* db, ID bug_id)
  : Table(db, bug_id)
{
  this->update();
}

Mutation::Mutation(DB* db)
  : Table(db, this->id_)
{
}


void Mutation::update()
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT position, original, mutated, bug_id\n\
FROM Mutation\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw IDNotFound(__FILE__, __LINE__);

    this->position = cursor.getint(0);
    this->original = cursor.getint(1);
    this->mutated = cursor.getint(2);
    if (cursor.isnull(1)) {
      this->add_null("original");
      this->type = Mutation::addition;
    } else if (cursor.isnull(2)) {
      this->add_null("mutated");
      this->type = Mutation::deletion;
    } else
      this->type = Mutation::mutation;
  } catch (const sqlite3x::database_error& e) {
    throw DBError(__FILE__, __LINE__, e.what());
  }


  Table::update();
}

void Mutation::update_db(bool force)
{
  if (this->changed or force) {
    sqlite3x::sqlite3_command sql(*this->db_);

    try {
      sql.prepare("\
UPDATE Mutation\n\
SET position = ?, original = ?, mutated = ?, bug_id\n\
WHERE id = ?;");
      sql.bind(1, static_cast<int>(this->position));
      if (this->type == Mutation::addition)
	sql.bind(2);
      else
	sql.bind(2, static_cast<int>(this->original));
      if (this->type == Mutation::deletion)
	sql.bind(3);
      else
	sql.bind(3, static_cast<int>(this->mutated));
      sql.bind(4, this->id_);

      sql.executenonquery();
    } catch (const sqlite3x::database_error& e) {
      throw DBError(__FILE__, __LINE__, e.what());
    }
  }


  Table::update_db(force);
}

void Mutation::insert(ID bug_id)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
INSERT INTO Mutation(bug_id, position, original, mutated, bug_id)\n\
VALUES(?, ?, ?, ?, ?);");
    sql.bind(1, bug_id);
    sql.bind(2, static_cast<int>(this->position));
    if (this->type == Mutation::addition)
      sql.bind(3);
    else
      sql.bind(3, static_cast<int>(this->original));
    if (this->type == Mutation::deletion)
      sql.bind(4);
    else
      sql.bind(4, static_cast<int>(this->mutated));
    sql.bind(5, static_cast<int>(this->bug_id));

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw DBError(__FILE__, __LINE__, e.what());
  }


  Table::insert(bug_id);
}

void Mutation::remove()
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
DELETE FROM Mutation\n\
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
