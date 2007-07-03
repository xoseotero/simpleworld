/**
 * @file db/hierarchy.cpp
 * Hierarchy of a bug.
 *
 * begin:     Sat, 24 Feb 2007 17:05:27 +0100
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

#include "hierarchy.hpp"

namespace SimpleWorld
{
namespace DB
{

Hierarchy::Hierarchy(DB* db, ID bug_id)
  : Table(db, bug_id)
{
}

Hierarchy::~Hierarchy()
{
  this->db_->free_hierarchy(this->id_);
}


void Hierarchy::update()
{
  // parents
  static sqlite3x::sqlite3_command sql1(*this->db_,
					"\
SELECT father_id\n\
FROM Hierarchy\n\
WHERE son_id = ?;");
  sql1.bind(1, this->id_);

  try {
    sqlite3x::sqlite3_cursor cursor(sql1.executecursor());
    while (cursor.step())
      this->parents.push_back(cursor.getint64(0));

    if (this->parents.empty())
      throw IDNotFound(__FILE__, __LINE__);
  } catch (sqlite3x::database_error) {
    throw DBError(__FILE__, __LINE__);
  }

  // sons
  static sqlite3x::sqlite3_command sql2(*this->db_,
					"\
SELECT son_id\n\
FROM Hierarchy\n\
WHERE father_id = ?");
  sql2.bind(1, this->id_);

  try {
    sqlite3x::sqlite3_cursor cursor(sql2.executecursor());
    while (cursor.step())
      this->sons.push_back(cursor.getint64(0));

    if (this->sons.empty())
      throw IDNotFound(__FILE__, __LINE__);
  } catch (sqlite3x::database_error) {
    throw IDNotFound(__FILE__, __LINE__);
  }
}

void Hierarchy::update_db()
{
  // parents
  static sqlite3x::sqlite3_command sql(*this->db_,
				       "\
INSERT OR IGNORE INTO Hierarchy(father_id, son_id)\n\
VALUES(?, ?);");
  sql.bind(2, this->id_);

  std::vector<ID>::iterator iter;
  try {
    for (iter = this->parents.begin();
	 iter != this->parents.end();
	 ++iter) {
      sql.bind(1, *iter);
      sql.executenonquery();
    }
  } catch (sqlite3x::database_error) {
    throw DBError(__FILE__, __LINE__);
  }

  // sons
  sql.bind(1, this->id_);

  try {
    for (iter = this->sons.begin();
	 iter != this->sons.end();
	 ++iter) {
      sql.bind(2, *iter);
      sql.executenonquery();
    }
  } catch (sqlite3x::database_error) {
    throw DBError(__FILE__, __LINE__);
  }
}

}
}
