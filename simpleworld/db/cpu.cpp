/**
 * @file simpleworld/db/cpu.cpp
 * CPU of a bug.
 *
 * begin:     Mon, 01 Jan 2007 09:02:47 +0100
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

#include <boost/format.hpp>

#include "exception.hpp"
#include "cpu.hpp"
#include "common.hpp"

namespace SimpleWorld
{
namespace DB
{

CPU::CPU(DB* db, ID bug_id)
  : Table(db, bug_id), registers(&this->changed)
{
  this->update();
}

CPU::CPU(DB* db)
  : Table(db), registers(&this->changed)
{
}


void CPU::update()
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT registers\n\
FROM CPU\n\
WHERE bug_id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw DBException(__FILE__, __LINE__,
                        boost::str(boost::format("\
bug_id %1% not found in table CPU")
                                   % this->id_));

    this->registers = get_memory(&cursor, 0);
  } catch (const sqlite3x::database_error& e) {
    throw DBException(__FILE__, __LINE__, e.what());
  }


  Table::update();
}

void CPU::update_db(bool force)
{
  if (this->changed or force) {
    sqlite3x::sqlite3_command sql(*this->db_);

    try {
      sql.prepare("\
UPDATE CPU\n\
SET registers = ?\n\
WHERE bug_id = ?;");
      bind_memory(&sql, 1, this->registers);
      sql.bind(2, this->id_);

      sql.executenonquery();
    } catch (const sqlite3x::database_error& e) {
      throw DBException(__FILE__, __LINE__, e.what());
    }
  }


  Table::update_db(force);
}

void CPU::insert(ID bug_id)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
INSERT INTO CPU(bug_id, registers)\n\
VALUES(?, ?);");
    sql.bind(1, bug_id);
    bind_memory(&sql, 2, this->registers);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw DBException(__FILE__, __LINE__, e.what());
  }


  Table::insert(bug_id);
}

void CPU::remove()
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
DELETE FROM CPU\n\
WHERE bug_id = ?;");
    sql.bind(1, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw DBException(__FILE__, __LINE__, e.what());
  }


  Table::remove();
}

}
}
