/**
 * @file simpleworld/db/deadbug.cpp
 * Information about a dead bug.
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

#include <string>

#include <boost/format.hpp>

#include <sqlite3x.hpp>

#include "exception.hpp"
#include "deadbug.hpp"

namespace simpleworld
{
namespace db
{

/**
 * Constructor.
 * @param db database.
 * @param id id of the bug.
 * @exception DBException if there is a error in the database.
 * @exception DBException if the ID is not found in the table.
 */
DeadBug::DeadBug(DB* db, ID id)
  // The position at this moment is unknown
  : BugElement(db, id, ElementNothing, this->position)
{
  this->update();
}


/**
 * Update the data of the class with the database.
 * changed is set to false.
 * @exception DBException if there is an error in the database.
 * @exception DBException if the ID is not found in the table.
 */
void DeadBug::update()
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT position_x, position_y, orientation,\n\
       birth, dead, father_id, killer_id\n\
FROM Bug\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Bug")
                                              % this->id_));

    this->position.x = cursor.getint(0);
    this->position.y = cursor.getint(1);
    this->orientation = static_cast<Orientation>(cursor.getint(2));
    this->birth = cursor.getint(3);
    this->dead = cursor.getint(4);
    this->father_id = cursor.getint64(5);
    this->killer_id = cursor.getint64(6);

    //assert(not cursor.isnull(4));
    if (cursor.isnull(5))
      this->add_null("father_id");
    if (cursor.isnull(6))
      this->add_null("killer_id");
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }


  BugElement::update();
}

/**
 * Update the database with the data of the class in changed or force are
 * true.
 * changed is set to false.
 * @param force force the update of the database.
 * @exception DBException if there is an error in the database.
 */
void DeadBug::update_db(bool force)
{
  if (this->changed or force) {
    sqlite3x::sqlite3_command sql(*this->db_);

    try {
      sql.prepare("\
UPDATE Bug\n\
SET position_x = ?, position_y = ?, orientation = ?,\n\
    birth = ?, dead = ?, father_id = ?, killer_id = ?\n\
WHERE id = ?;");
      sql.bind(1, static_cast<int>(this->position.x));
      sql.bind(2, static_cast<int>(this->position.y));
      sql.bind(3, static_cast<int>(this->orientation));
      sql.bind(4, static_cast<int>(this->birth));
      //assert(not this->is_null("dead"));
      sql.bind(5, static_cast<int>(this->dead));
      if (this->is_null("father_id"))
        sql.bind(6);
      else
        sql.bind(6, static_cast<sqlite3x::int64_t>(this->father_id));
      if (this->is_null("killer_id"))
        sql.bind(7);
      else
        sql.bind(7, static_cast<sqlite3x::int64_t>(this->killer_id));
      sql.bind(8, this->id_);

      sql.executenonquery();
    } catch (const sqlite3x::database_error& e) {
      throw EXCEPTION(DBException, std::string(e.what()) +
                      " (" + this->db()->errormsg() + ")");
    }
  }


  BugElement::update_db(force);
}

/**
 * Remove the data from the database.
 * changed is set to false.
 * @exception DBException if there is an error in the database.
 */
void DeadBug::remove()
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
DELETE FROM Bug\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }


  BugElement::remove();
}

}
}
