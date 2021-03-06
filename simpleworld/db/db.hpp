/**
 * @file simpleworld/db/db.hpp
 * Simple World Database management.
 *
 *  Copyright (C) 2007-2011  Xosé Otero <xoseotero@gmail.com>
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

#ifndef SIMPLEWORLD_DB_DB_HPP
#define SIMPLEWORLD_DB_DB_HPP

#include <string>
#include <vector>

#include <sqlite3.h>

#include <simpleworld/ints.hpp>
#include <simpleworld/types.hpp>
#include <simpleworld/db/types.hpp>

namespace simpleworld
{
/**
 * Namespace for Simple World Data Base.
 */
namespace db
{

/**
 * Simple World Data Base management.
 */
class DB
{
public:
  /**
   * Constructor.
   * @param filename File name of the database.
   * @exception DBException if the database can't be opened.
   * @exception DBException if there is a error in the database.
   * @exception WrongVersion if the database version is not supported.
   */
  DB(std::string filename);

  /**
   * Destructor.
   */
  ~DB();


  /**
  * Create the database with the default Environment.
  * @param filename File name of the database.
  * @exception DBException if there is an error with the creation.
  */
  static void create(std::string filename);

  /**
   * Create the database.
   * @param filename File name of the database.
   * @param time time passed since the creation of the World.
   * @param size_x size of the World (x coord).
   * @param size_y size of the World (y coord).
   * @param time_rot time needed to rot the food.
   * @param size_rot size that is substracted to the food.
   * @param mutations_probability probability (0.0-1.0) that the code mutates.
   * @param time_birth time needed to convert a egg into a bug.
   * @param time_mutate time to mutate the code of a old bug.
   * @param time_laziness time without doing a action to consider a bug lazy.
   * @param energy_laziness energy substracted for laziness.
   * @param attack_multiplier multiplier for the energy of a attack.
   * @param time_nothing time needed to do the action nothing.
   * @param time_myself time needed to do the action myself.
   * @param time_detect time needed to do the action detect.
   * @param time_info time needed to do the action info.
   * @param time_move time needed to do the action move.
   * @param time_turn time needed to do the action turn.
   * @param time_attack time needed to do the action attack.
   * @param time_eat time needed to do the action eat.
   * @param time_egg time needed to do the action egg.
   * @param energy_nothing energy used to do the action nothing.
   * @param energy_myself energy used to do the action myself.
   * @param energy_detect energy used to do the action detect.
   * @param energy_info energy used to do the action info.
   * @param energy_move energy used to do the action move.
   * @param energy_turn energy used to do the action turn.
   * @param energy_attack energy used to do the action attack.
   * @param energy_eat energy used to do the action eat.
   * @param energy_egg energy used to do egg.
   * @exception DBException if there is an error with the creation.
   */
  static void create(std::string filename,
                     Time time, Coord size_x, Coord size_y,
                     Time time_rot, Energy size_rot,
                     double mutations_probability, Time time_birth,
                     Time time_mutate, Time time_laziness,
                     Energy energy_laziness, double attack_multiplier,
                     Time time_nothing, Time time_myself, Time time_detect,
                     Time time_info, Time time_move, Time time_turn,
                     Time time_attack, Time time_eat, Time time_egg,
                     Energy energy_nothing, Energy energy_myself,
                     Energy energy_detect, Energy energy_info,
                     Energy energy_move, Energy energy_turn,
                     Energy energy_attack, Energy energy_eat,
                     Energy energy_egg);


  /**
   * Get the sqlite3 database connection handler.
   */
  sqlite3* db() const { return this->db_; }

  /**
   * Version of the data base.
   * @return The version.
   */
  Uint8 version() const { return this->version_; }


  /**
   * List of all the environments (changes), ordered by it's time.
   * @return the list of environments.
   * @exception DBException if there is a error in the database.
   */
  std::vector<ID> environments();

  /**
   * Get the last environment (change).
   * @return the environment.
   * @exception DBException if there is a error in the database.
   * @exception DBException if there isn't any environments.
   */
  ID last_environment();


  /**
   * List of spawns.
   * @return the list of spawns.
   * @exception DBException if there is a error in the database.
   */
  std::vector<ID> spawns();

  /**
   * List of resources.
   * @return the list of resources.
   * @exception DBException if there is a error in the database.
   */
  std::vector<ID> resources();


  /**
   * List of eggs, ordered by its conception time.
   * @return the list of eggs.
   * @exception DBException if there is a error in the database.
   */
  std::vector<ID> eggs();

  /**
   * List of the alive bugs, ordered by its birth.
   * @return the list of bugs.
   * @exception DBException if there is a error in the database.
   */
  std::vector<ID> alive_bugs();

  /**
   * List of the dead bugs, ordered by its death.
   * @return the list of bugs.
   * @exception DBException if there is a error in the database.
   */
  std::vector<ID> dead_bugs();


  /**
   * List of the food.
   * @return the list of food.
   * @exception DBException if there is a error in the database.
   */
  std::vector<ID> food();

  /**
   * List of the stats.
   * @return the list of stats.
   * @exception DBException if there is a error in the database.
   */
  std::vector<ID> stats();

private:
  sqlite3* db_;                 /**< Database connection */
  Uint8 version_;               /**< Version of the database */
};

}
}

#endif // SIMPLEWORLD_DB_DB_HPP
