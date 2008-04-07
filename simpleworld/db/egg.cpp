/**
 * @file simpleworld/db/egg.cpp
 * Information about a egg.
 *
 * begin:     Wed, 15 Aug 2007 14:58:35 +0200
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

#include <sqlite3x.hpp>

#include "exception.hpp"
#include "cpu.hpp"
#include "egg.hpp"

namespace simpleworld
{
namespace db
{

/**
 * Constructor.
 * @param db database.
 * @param id id of the egg.
 * @exception DBException if there is a error in the database.
 * @exception DBException if the ID is not found in the table.
 */
Egg::Egg(DB* db, ID id)
  // The position at this moment is unknown
  : AliveBug(db, id, ElementEgg, this->position)
{
  this->update();
}

/**
 * Constructor to insert data.
 * @param db database.
 * @exception DBException if there is a error in the database.
 */
Egg::Egg(DB* db)
  : AliveBug(db, ElementEgg)
{
}


/**
 * Convert the egg in a bug.
 * @return The ID of the new bug.
 */
ID Egg::be_born()
{
  // Convert the egg to a bug
  CPU(this->db_).insert(this->id_);

  return this->id_;
}


/**
 * Update the data of the class with the database.
 * changed is set to false.
 * @exception DBException if there is an error in the database.
 * @exception DBException if the ID is not found in the table.
 */
void Egg::update()
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT energy, position_x, position_y, orientation,\n\
       birth, father_id\n\
FROM Bug\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Bug")
                                              % this->id_));

    this->energy = cursor.getint(0);
    this->position.x = cursor.getint(1);
    this->position.y = cursor.getint(2);
    this->orientation = static_cast<Orientation>(cursor.getint(3));
    this->birth = cursor.getint(4);
    this->father_id = cursor.getint64(5);

    if (cursor.isnull(5))
      this->add_null("father_id");
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, e.what());
  }


  AliveBug::update();
}

/**
 * Update the database with the data of the class in changed or force are
 * true.
 * changed is set to false.
 * @param force force the update of the database.
 * @exception DBException if there is an error in the database.
 */
void Egg::update_db(bool force)
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
      throw EXCEPTION(DBException, e.what());
    }
  }


  AliveBug::update_db(force);
}

/**
 * Insert the data in the database.
 * The ID is updated.
 * changed is set to false.
 * @exception DBException if there is an error in the database.
 */
void Egg::insert()
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
INSERT INTO Bug(energy, position_x, position_y, orientation,\n\
                birth, father_id)\n\
VALUES(?, ?, ?, ?, ?, ?);");
    sql.bind(1, static_cast<int>(this->energy));
    sql.bind(2, static_cast<int>(this->position.x));
    sql.bind(3, static_cast<int>(this->position.y));
    sql.bind(4, static_cast<int>(this->orientation));
    sql.bind(5, static_cast<int>(this->birth));
    if (this->is_null("father_id"))
      sql.bind(6);
    else
      sql.bind(6, static_cast<sqlite3x::int64_t>(this->father_id));

    sql.executenonquery();
    this->id_ = this->db_->insertid();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, e.what());
  }


  AliveBug::insert();
}

/**
 * Remove the data from the database.
 * changed is set to false.
 * @exception DBException if there is an error in the database.
 */
void Egg::remove()
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
DELETE FROM Bug\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, e.what());
  }


  AliveBug::remove();
}

}
}
