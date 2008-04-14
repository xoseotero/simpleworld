/**
 * @file simpleworld/db/environment.cpp
 * Information about the environment of the world.
 *
 * begin:     Fri, 26 Jan 2007 06:29:35 +0100
 * last:      $Date$
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

#include <boost/format.hpp>

#include "exception.hpp"
#include "environment.hpp"

namespace simpleworld
{
namespace db
{

/**
 * Constructor.
 * @param db database.
 * @param id id of the environment.
 * @exception DBException if there is a error in the database.
 * @exception DBException if the ID is not found in the table.
 */
Environment::Environment(DB* db, ID id)
  : Table(db, id)
{
  this->update();
}

/**
 * Constructor to insert data.
 * @param db database.
 * @exception DBException if there is a error in the database.
 */
Environment::Environment(DB* db)
  : Table(db)
{
}


/**
 * Update the data of the class with the database.
 * changed is set to false.
 * @exception DBException if there is a error in the database.
 * @exception DBException if the ID is not found in the table.
 */
void Environment::update()
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT time, size_x, size_y,\n\
       mutations_probability, time_birth, attack_multiplier,\n\
       energy_nothing, energy_myself, energy_detect, energy_info,\n\
       energy_move, energy_turn, energy_attack, energy_eat, energy_egg\n\
FROM Environment\n\
WHERE id = ?");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
time %1% not found in table Environment")
                                              % this->id_));

    this->time = cursor.getint(0);
    this->size.x = cursor.getint(1);
    this->size.y = cursor.getint(2);
    this->mutations_probability = cursor.getdouble(3);
    this->time_birth = cursor.getint(4);
    this->attack_multiplier = cursor.getdouble(5);
    this->energy_nothing = cursor.getint(6);
    this->energy_myself = cursor.getint(7);
    this->energy_detect = cursor.getint(8);
    this->energy_info = cursor.getint(9);
    this->energy_move = cursor.getint(10);
    this->energy_turn = cursor.getint(11);
    this->energy_attack = cursor.getint(12);
    this->energy_eat = cursor.getint(13);
    this->energy_egg = cursor.getint(14);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, e.what());
  }


  Table::update();
}

/**
 * Update the database with the data of the class in changed or force are
 * true.
 * changed is set to false.
 * @param force force the update of the database.
 * @exception DBException if there is a error in the database.
 */
void Environment::update_db(bool force)
{
  if (this->changed or force) {
    sqlite3x::sqlite3_command sql(*this->db_);

    try {
      sql.prepare("\
UPDATE Environment\n\
SET time = ?, size_x = ?, size_y = ?,\n\
    mutations_probability = ?, time_birth = ?, attack_multiplier = ?,\n\
    energy_nothing = ?, energy_myself = ?, energy_detect = ?,\n\
    energy_info = ?, energy_move = ?, energy_turn = ?,\n\
    energy_attack = ?, energy_eat = ?, energy_egg = ?\n\
WHERE id = ?;");
      sql.bind(1, static_cast<int>(this->time));
      sql.bind(2, static_cast<int>(this->size.x));
      sql.bind(3, static_cast<int>(this->size.y));
      sql.bind(4, this->mutations_probability);
      sql.bind(5, static_cast<int>(this->time_birth));
      sql.bind(6, this->attack_multiplier);
      sql.bind(7, static_cast<int>(this->energy_nothing));
      sql.bind(8, static_cast<int>(this->energy_myself));
      sql.bind(9, static_cast<int>(this->energy_detect));
      sql.bind(10, static_cast<int>(this->energy_info));
      sql.bind(11, static_cast<int>(this->energy_move));
      sql.bind(12, static_cast<int>(this->energy_turn));
      sql.bind(13, static_cast<int>(this->energy_attack));
      sql.bind(14, static_cast<int>(this->energy_eat));
      sql.bind(15, static_cast<int>(this->energy_egg));
      sql.bind(16, this->id_);

      sql.executenonquery();
    } catch (const sqlite3x::database_error& e) {
      throw EXCEPTION(DBException, e.what());
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
void Environment::insert()
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
INSERT INTO Environment(time, size_x, size_y,\n\
                        mutations_probability, time_birth, attack_multiplier,\n\
                        energy_nothing, energy_myself, energy_detect,\n\
                        energy_info, energy_move, energy_turn,\n\
                        energy_attack, energy_eat, energy_egg)\n\
VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
    sql.bind(1, static_cast<int>(this->time));
    sql.bind(2, static_cast<int>(this->size.x));
    sql.bind(3, static_cast<int>(this->size.y));
    sql.bind(4, this->mutations_probability);
    sql.bind(5, static_cast<int>(this->time_birth));
    sql.bind(6, this->attack_multiplier);
    sql.bind(7, static_cast<int>(this->energy_nothing));
    sql.bind(8, static_cast<int>(this->energy_myself));
    sql.bind(9, static_cast<int>(this->energy_detect));
    sql.bind(10, static_cast<int>(this->energy_info));
    sql.bind(11, static_cast<int>(this->energy_move));
    sql.bind(12, static_cast<int>(this->energy_turn));
    sql.bind(13, static_cast<int>(this->energy_attack));
    sql.bind(14, static_cast<int>(this->energy_eat));
    sql.bind(15, static_cast<int>(this->energy_egg));

    sql.executenonquery();
    this->id_ = this->db_->insertid();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, e.what());
  }


  Table::insert();
}

/**
 * Remove the data from the database.
 * changed is set to false.
 * @exception DBException if there is an error in the database.
 */
void Environment::remove()
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
DELETE FROM Environment\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, e.what());
  }


  Table::remove();
}

}
}
