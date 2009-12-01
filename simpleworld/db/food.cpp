/**
 * @file simpleworld/db/food.cpp
 * Information about the food
 *
 *  Copyright (C) 2007  Xosé Otero <xoseotero@gmail.com>
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

#include "exception.hpp"
#include "food.hpp"

namespace simpleworld
{
namespace db
{

/**
 * Constructor.
 * @param db database.
 * @param id id of the food.
 * @exception DBException if there is a error in the database.
 * @exception DBException if the ID is not found in the table.
 */
Food::Food(DB* db, ID id)
  // The position at this moment is unknown
  : Table(db, id), Element(ElementFood, this->position)
{
  this->update();
}

/**
 * Constructor to insert data.
 * @param db database.
 * @exception DBException if there is a error in the database.
 */
Food::Food(DB* db)
  // The position at this moment is unknown
  : Table(db), Element(ElementFood, this->position)
{
}


/**
 * Update the data of the class with the database.
 * changed is set to false.
 * @exception DBException if there is an error in the database.
 * @exception DBException if the ID is not found in the table.
 */
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
      throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Food")
                                              % this->id_));

    this->position.x = cursor.getint(0);
    this->position.y = cursor.getint(1);
    this->size = cursor.getint(2);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }


  Table::update();
}

/**
 * Update the database with the data of the class in changed or force are
 * true.
 * changed is set to false.
 * @param force force the update of the database.
 * @exception DBException if there is an error in the database.
 */
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
      throw EXCEPTION(DBException, std::string(e.what()) +
                      " (" + this->db()->errormsg() + ")");
    }
  }


  Table::update_db(force);
}

/**
 * Insert the data in the database.
 * The ID is updated.
 * changed is set to false.
 * @exception DBException if there is an error in the database.
 */
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
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }


  Table::insert();
}

/**
 * Remove the data from the database.
 * changed is set to false.
 * @exception DBException if there is an error in the database.
 */
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
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }


  Table::remove();
}

}
}
