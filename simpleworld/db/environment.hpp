/**
 * @file simpleworld/db/environment.hpp
 * Information about the environment of the world.
 *
 * begin:     Thu, 25 Jan 2007 03:53:57 +0100
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

#ifndef __SIMPLEWORLD_DB_ENVIRONMENT_HPP__
#define __SIMPLEWORLD_DB_ENVIRONMENT_HPP__

#include <simpleworld/ints.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/table.hpp>

namespace SimpleWorld
{
namespace DB
{

/**
 * Information about the environment of the world.
 */
class Environment: public Table
{
public:
  /**
   * Constructor.
   * @param db database.
   * @param time time of the environment (the real time can be >= than this).
   * @exception DBError if there is a error in the database.
   * @exception IDNotFound if the time is not found in the table.
   */
  Environment(DB* db, Time time);

  /**
   * Constructor to insert data.
   * @param db database.
   * @exception DBError if there is a error in the database.
   */
  Environment(DB* db);


  /**
   * Update the data of the class with the database.
   * changed is set to false.
   * @exception DBError if there is a error in the database.
   * @execption IDNotFound if the ID is not found in the table.
   */
  void update();

  /**
   * Update the database with the data of the class in changed or force are
   * true.
   * changed is set to false.
   * @param force force the update of the database.
   * @exception DBError if there is a error in the database.
   */
  void update_db(bool force = false);

  /**
   * Insert the data in the database.
   * The ID is updated.
   * changed is set to false.
   * @exception DBError if there is an error in the database.
   */
  void insert();

  /**
   * Remove the data from the database.
   * changed is set to false.
   * @exception DBError if there is an error in the database.
   */
  void remove();


  // Data
  Time time;

  Position size;

  Energy energy_developed;

  float mutations_probability;

  Time time_birth;

  Energy energy_nothing;
  Energy energy_myself;
  Energy energy_detect;
  Energy energy_info;
  Energy energy_move;
  Energy energy_turn;
  Energy energy_attack;
  Energy energy_eat;
  Energy energy_egg;
};

}
}

#endif // __SIMPLEWORLD_DB_ENVIRONMENT_HPP__
