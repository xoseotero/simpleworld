/**
 * @file simpleworld/db/environment.hpp
 * Information about the environment of the world.
 *
 * begin:     Thu, 25 Jan 2007 03:53:57 +0100
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

#ifndef SIMPLEWORLD_DB_ENVIRONMENT_HPP
#define SIMPLEWORLD_DB_ENVIRONMENT_HPP

#include <simpleworld/ints.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/table.hpp>

namespace simpleworld
{
namespace db
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
   * @param id id of the environment.
   * @exception DBException if there is a error in the database.
   * @exception DBException if the ID is not found in the table.
   */
  Environment(DB* db, ID id);

  /**
   * Constructor to insert data.
   * @param db database.
   * @exception DBException if there is a error in the database.
   */
  Environment(DB* db);


  /**
   * Update the data of the class with the database.
   * changed is set to false.
   * @exception DBException if there is a error in the database.
   * @exception DBException if the ID is not found in the table.
   */
  void update();

  /**
   * Update the database with the data of the class in changed or force are
   * true.
   * changed is set to false.
   * @param force force the update of the database.
   * @exception DBException if there is a error in the database.
   */
  void update_db(bool force = false);

  /**
   * Insert the data in the database.
   * The ID is updated.
   * changed is set to false.
   * @exception DBException if there is an error in the database.
   */
  void insert();

  /**
   * Remove the data from the database.
   * changed is set to false.
   * @exception DBException if there is an error in the database.
   */
  void remove();


  // Data
  Time time;                    /**< Cycles since the creation of the World. */

  Position size;                /**< Size of the World. */

  double mutations_probability; /**< Probability (0.0-1.0) that the code
                                     mutates. */
  Time time_birth;              /**< Cycles needed to convert a egg into a
                                     bug. */
  Time time_mutate;             /**< Cycles needed to mutate the code of a
                                     old bug. */
  double attack_multiplier;     /**< Multiplier for the energy of a attack */

  Energy energy_nothing;        /**< Energy needed to do action nothing. */
  Energy energy_myself;         /**< Energy needed to do action myself. */
  Energy energy_detect;         /**< Energy needed to do action detect. */
  Energy energy_info;           /**< Energy needed to do action info. */
  Energy energy_move;           /**< Energy needed to do action move. */
  Energy energy_turn;           /**< Energy needed to do action turn. */
  Energy energy_attack;         /**< Energy needed to do action attack. */
  Energy energy_eat;            /**< Energy needed to do action eat. */
  Energy energy_egg;            /**< Energy needed to do action egg. */
};

}
}

#endif // SIMPLEWORLD_DB_ENVIRONMENT_HPP
