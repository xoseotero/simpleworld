/**
 * @file simpleworld/simpleworld.hpp
 * Simple World: the simulation of a world and the bugs that live on it.
 *
 * Simple World is the simulation of a world and the bugs that live on it.
 * Simple World tries to reproduce the basic elements that define a simple
 * world. The objective of the project is to observe the evolution of this
 * world and of these bugs.
 *
 * begin:     Sat, 21 Jul 2007 12:08:38 +0200
 * last:      $Date$
 *
 *  Copyright (C) 2007  Xosé Antón Otero Ferreira <xoseotero@gmail.com>
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

#ifndef __SIMPLEWORLD_SIMPLEWORLD_HPP__
#define __SIMPLEWORLD_SIMPLEWORLD_HPP__

#include <list>
#include <string>

#include <simpleworld/types.hpp>
#include <simpleworld/world.hpp>
#include <simpleworld/food.hpp>
#include <simpleworld/egg.hpp>
#include <simpleworld/bug.hpp>
#include <simpleworld/db/db.hpp>
#include <simpleworld/db/environment.hpp>
#include <simpleworld/cpu/types.hpp>
#include <simpleworld/cpu/memory.hpp>

namespace SimpleWorld
{

/**
 * Simple World: the simulation of a world and the bugs that live on it.
 *
 * Simple World is the simulation of a world and the beings that inhabit it.
 * Simple World tries to reproduce the basic elements that define a simple
 * world. The objective of the project is to observe the evolution of this
 * world and of these beings.
 */
class SimpleWorld: public DB::DB
{
public:
  /**
   * Constructor.
   * @param filename File name of the database.
   * @exception DBException if there is a error in the database.
   */
  SimpleWorld(std::string filename);

  /**
   * Destructor.
   */
  ~SimpleWorld();


  const World& world() const { return *this->world_; }
  const ::SimpleWorld::DB::Environment& env() const { return *this->env_; }

  /**
   * Add a egg to the World.
   * @param energy Energy of the egg.
   * @param position Position of the egg.
   * @param orientation Orientation of the egg.
   * @param code Code of the egg.
   * @exception Exception if the position is out of the limits.
   * @exception Exception if the position is used.
   */
  void add_egg(Energy energy, Position position, Orientation orientation,
               const CPU::Memory& code);

  /**
   * Add food to the World.
   * @param position Position of the food.
   * @param size Size of the food.
   * @exception Exception if the position is out of the limits.
   * @exception Exception if the position is used.
   */
  void add_food(Position position, Energy size);


  /**
   * Execute some cycles of the World.
   * @param cycles Cycles to be executed.
   */
  void run(Time cycles);


  /**
   * Do nothing.
   * @param bug Bug that executes the action.
   * @exception ActionError if something fails.
   */
  virtual void nothing(Bug* bug);

  /**
   * Get information about the bug itself.
   * @param bug Bug that executes the action.
   * @param info Type of information requested.
   * @param ypos Where to save the Y coord when requested the position.
   * @return The information.
   * @exception ActionError if something fails.
   */
  virtual CPU::Word myself(Bug* bug, Info info, CPU::Word* ypos);

  /**
   * Get the type of element that is in front of the bug.
   * @param bug Bug that executes the action.
   * @return The type of element.
   * @exception ActionError if something fails.
   */
  virtual ElementType detect(Bug* bug);

  /**
   * Get information about the bug that is in front of the bug.
   * @param bug Bug that executes the action.
   * @param info Type of information requested.
   * @param ypos Where to save the Y coord when requested the position.
   * @return The information.
   * @exception ActionError if something fails.
   */
  virtual CPU::Word information(Bug* bug, Info info, CPU::Word* ypos);

  /**
   * Move a bug.
   * @param bug Bug that executes the action.
   * @param movement Type of movement.
   * @exception ActionError if something fails.
   */
  virtual void move(Bug* bug, Movement movement);

  /**
   * Turn a bug.
   * @param bug Bug that executes the action.
   * @param turn Type of turn.
   * @exception ActionError if something fails.
   */
  virtual void turn(Bug* bug, Turn turn);

  /**
   * Attack.
   * @param bug Bug that executes the action.
   * @param energy Energy used in the attack.
   * @exception ActionError if something fails.
   */
  virtual void attack(Bug* bug, Energy energy);

  /**
   * Eat what is in front of the bug.
   * @param bug Bug that executes the action.
   * @return The energy eaten.
   * @exception ActionError if something fails.
   */
  virtual Energy eat(Bug* bug);

  /**
   * Create a egg in front of the bug.
   * @param bug Bug that executes the action.
   * @param energy Energy transfered to the egg.
   * @exception ActionError if something fails.
   */
  virtual void egg(Bug* bug, Energy energy);

protected:
  /**
   * This function is called when open() succeeds. Subclasses
   * which wish to do custom db initialization or sanity checks
   * may do them here.
   */
  virtual void on_open();

  /**
   * Position in front of a given bug.
   * @param bug The bug.
   * @return Position in front of the bug.
   */
  Position front(Bug* bug);


  /**
   * Convert in bugs all the eggs that are old enough.
   */
  void eggs_birth();

  /**
   * Execute a cycle in all the alive bugs.
   */
  void bugs_run();

  /**
   * Update the elements of the World in the database.
   */
  void update_db();

  World* world_;
  ::SimpleWorld::DB::Environment* env_;

private:
  std::list<Food*> foods_;
  std::list<Egg*> eggs_;
  std::list<Bug*> bugs_;
};

}

#endif // __SIMPLEWORLD_SIMPLEWORLD_HPP__
