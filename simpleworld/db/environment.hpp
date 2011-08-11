/**
 * @file simpleworld/db/environment.hpp
 * Information about the environment of the world.
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

#ifndef SIMPLEWORLD_DB_ENVIRONMENT_HPP
#define SIMPLEWORLD_DB_ENVIRONMENT_HPP

#include <simpleworld/db/types.hpp>
#include <simpleworld/db/db.hpp>
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
   * It's not checked if the id is in the table, only when accessing the data
   * the id is checked.
   * @param db database.
   * @param id id of the environment.
   */
  Environment(DB* db, ID id);


  /**
   * Insert a environment.
   * @param db database.
   * @param time cycles since the creation of the World.
   * @param size_x size of the World (x coord).
   * @param size_y size of the World (y coord).
   * @param time_rot cycles needed to rot the food.
   * @param size_rot size that is substracted to the food.
   * @param mutations_probability probability (0.0-1.0) that the code mutates.
   * @param time_birth cycles to convert a egg into a bug.
   * @param time_mutate cycles to mutate the code of a old bug.
   * @param time_laziness cycles without doing a action to consider a bug lazy.
   * @param energy_laziness energy substracted for laziness.
   * @param attack_multiplier multiplier for the energy of a attack.
   * @param energy_nothing energy used to do the action nothing.
   * @param energy_myself energy used to do the action myself.
   * @param energy_detect energy used to do the action detect.
   * @param energy_info energy used to do the action info.
   * @param energy_move energy used to do the action move.
   * @param energy_turn energy used to do the action turn.
   * @param energy_attack energy used to do the action attack.
   * @param energy_eat energy used to do the action eat.
   * @param energy_egg energy used to do egg 
   * @return the id of the new row.
   * @exception DBException if there is an error with the insertion.
   */
  static ID insert(DB* db, Time time, Coord size_x, Coord size_y,
                   Time time_rot, Energy size_rot,
                   double mutations_probability, Time time_birth,
                   Time time_mutate, Time time_laziness,
                   Energy energy_laziness, double attack_multiplier,
                   Energy energy_nothing, Energy energy_myself,
                   Energy energy_detect, Energy energy_info,
                   Energy energy_move, Energy energy_turn,
                   Energy energy_attack, Energy energy_eat,
                   Energy energy_egg);

  /**
   * Delete a environment.
   * @param db database.
   * @param id id of the environment.
   * @exception DBException if there is an error with the deletion.
   */
  static void remove(DB* db, ID id);


  /**
   * Get the id of the environment.
   * @return the id of the environment.
   * @exception DBException if there is an error with the query.
   */
  ID id() const { return db::Table::id(); }

  /**
   * Set the id of the environment.
   * @param id the id.
   * @exception DBException if there is an error with the update.
   */
  void id(ID id);


  /**
   * Get cycles since the creation of the World.
   * @return the cycles.
   * @exception DBException if there is an error with the query.
   */
  Time time() const;

  /**
   * Set cycles since the creation of the World.
   * @param time the new cycles.
   * @exception DBException if there is an error with the update.
   */
  void time(Time time);


  /**
   * Get size of the World (x coord).
   * @return the size.
   * @exception DBException if there is an error with the query.
   */
  Coord size_x() const;

  /**
   * Get size of the World (y coord).
   * @return the size.
   * @exception DBException if there is an error with the query.
   */
  Coord size_y() const;

  /**
   * Set size of the World (x coord).
   * @param size_x the new size.
   * @exception DBException if there is an error with the update.
   */
  void size_x(Coord size_x);

  /**
   * Set size of the World (x coord).
   * @param size_y the new size.
   * @exception DBException if there is an error with the update.
   */
  void size_y(Coord size_y);


  /**
   * Get the cycles needed to rot the food.
   * @return the cycles.
   * @exception DBException if there is an error with the update.
   */
  Time time_rot() const;

  /**
   * Get the cycles needed to rot the food.
   * @param time_rot the new cycles.
   * @exception DBException if there is an error with the update.
   */
  void time_rot(Time time_rot);

  /**
   * Get the size that is substracted to the food.
   * @return the size.
   * @exception DBException if there is an error with the update.
   */
  Energy size_rot() const;

  /**
   * Set the size that is substracted to the food.
   * @param size_rot the new size.
   * @exception DBException if there is an error with the update.
   */
  void size_rot(Energy size_rot);


  /**
   * Get the probability (0.0-1.0) that the code mutates.
   * @return the probability.
   * @exception DBException if there is an error with the query.
   */
  double mutations_probability() const;

  /**
   * Set the probability (0.0-1.0) that the code mutates.
   * @param mutations_probability the new probability.
   * @exception DBException if there is an error with the update.
   */
  void mutations_probability(double mutations_probability);


  /**
   * Get the cycles to convert a egg into a bug.
   * @return the cycles.
   * @exception DBException if there is an error with the query.
   */
  Time time_birth() const;

  /**
   * Set the cycles to convert a egg into a bug.
   * @param time_birth the new cycles.
   * @exception DBException if there is an error with the update.
   */
  void time_birth(Time time_birth);


  /**
   * Get the cycles to mutate the code of a old bug.
   * @return the cycles.
   * @exception DBException if there is an error with the query.
   */
  Time time_mutate() const;

  /**
   * Set the cycles to mutate the code of a old bug.
   * @param time_mutate the new time.
   * @exception DBException if there is an error with the update.
   */
  void time_mutate(Time time_mutate);


  /**
   * Get the cycles without doing a action to be considered lazy.
   * @return the time.
   * @exception DBException if there is an error with the query.
   */
  Time time_laziness() const;

  /**
   * Set the cyles without doing a action to be considered lazy.
   * @param time_laziness the new time.
   * @exception DBException if there is an error with the update.
   */
  void time_laziness(Time time_laziness);


  /**
   * Get the energy substracted for laziness.
   * @return the energy.
   * @exception DBException if there is an error with the query.
   */
  Energy energy_laziness() const;

  /**
   * Set the energy substracted for laziness.
   * @param energy_laziness the new energy.
   * @exception DBException if there is an error with the update.
   */
  void energy_laziness(Energy energy_laziness);


  /**
   * Get the multiplier for the energy of a attack.
   * @return the multiplier.
   * @exception DBException if there is an error with the query.
   */
  double attack_multiplier() const;

  /**
   * Set the multiplier for the energy of a attack.
   * @param attack_multiplier the new multiplier.
   * @exception DBException if there is an error with the update.
   */
  void attack_multiplier(double attack_multiplier);


  /**
   * Get the energy used to do the action nothing.
   * @return the energy.
   * @exception DBException if there is an error with the query.
   */
  Energy energy_nothing() const;

  /**
   * Set the energy used to do the action nothing.
   * @param energy_nothing the new energy.
   * @exception DBException if there is an error with the update.
   */
  void energy_nothing(Energy energy_nothing);


  /**
   * Get the energy used to do the action myself.
   * @return the energy.
   * @exception DBException if there is an error with the query.
   */
  Energy energy_myself() const;

  /**
   * Set the energy used to do the action myself.
   * @param energy_myself the new energy.
   * @exception DBException if there is an error with the update.
   */
  void energy_myself(Energy energy_myself);


  /**
   * Get the energy used to do the action detect.
   * @return the energy.
   * @exception DBException if there is an error with the query.
   */
  Energy energy_detect() const;

  /**
   * Set the energy used to do the action detect.
   * @param energy_detect the new energy.
   * @exception DBException if there is an error with the update.
   */
  void energy_detect(Energy energy_detect);


  /**
   * Get the energy used to do the action info.
   * @return the energy.
   * @exception DBException if there is an error with the query.
   */
  Energy energy_info() const;

  /**
   * Set the energy used to do the action info.
   * @param energy_info the new energy.
   * @exception DBException if there is an error with the update.
   */
  void energy_info(Energy energy_info);


  /**
   * Get the energy used to do the action move.
   * @return the energy.
   * @exception DBException if there is an error with the query.
   */
  Energy energy_move() const;

  /**
   * Set the energy used to do the action move.
   * @param energy_move the new energy.
   * @exception DBException if there is an error with the update.
   */
  void energy_move(Energy energy_move);


  /**
   * Get the energy used to do the action turn.
   * @return the energy.
   * @exception DBException if there is an error with the query.
   */
  Energy energy_turn() const;

  /**
   * Set the energy used to do the action turn.
   * @param energy_turn the new energy.
   * @exception DBException if there is an error with the update.
   */
  void energy_turn(Energy energy_turn);


  /**
   * Get the energy used to do the action attack.
   * @return the energy.
   * @exception DBException if there is an error with the query.
   */
  Energy energy_attack() const;

  /**
   * Set the energy used to do the action attack.
   * @param energy_attack the new energy.
   * @exception DBException if there is an error with the update.
   */
  void energy_attack(Energy energy_attack);


  /**
   * Get the energy used to do the action eat.
   * @return the energy.
   * @exception DBException if there is an error with the query.
   */
  Energy energy_eat() const;

  /**
   * Set the energy used to do the action eat.
   * @param energy_eat the new energy.
   * @exception DBException if there is an error with the update.
   */
  void energy_eat(Energy energy_eat);


  /**
   * Get the energy used to do the action egg.
   * @return the energy.
   * @exception DBException if there is an error with the query.
   */
  Energy energy_egg() const;

  /**
   * Set the energy used to do the action egg.
   * @param energy_egg the new energy.
   * @exception DBException if there is an error with the update.
   */
  void energy_egg(Energy energy_egg);
};

}
}

#endif // SIMPLEWORLD_DB_ENVIRONMENT_HPP
