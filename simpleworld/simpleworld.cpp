/**
 * @file simpleworld/simpleworld.hpp
 * Simple World: the simulation of a world and the bugs that live on it.
 *
 * Simple World is the simulation of a world and the bugs that live on it.
 * Simple World tries to reproduce the basic elements that define a simple
 * world. The objective of the project is to observe the evolution of this
 * world and of these bugs.
 *
 *  Copyright (C) 2007-2010  Xosé Otero <xoseotero@gmail.com>
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

#if DEBUG
#include <iostream>
#endif // DEBUG

#include <algorithm>
#include <vector>
#include <list>
#include <cassert>

#include <boost/scoped_array.hpp>
#include <boost/format.hpp>

#include <sqlite3x.hpp>

#include <simpleworld/cpu/types.hpp>
#include <simpleworld/cpu/exception.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/exception.hpp>
#include <simpleworld/db/world.hpp>
#include <simpleworld/db/food.hpp>
#include <simpleworld/db/bug.hpp>
#include <simpleworld/db/egg.hpp>
#include <simpleworld/db/alivebug.hpp>
#include <simpleworld/db/deadbug.hpp>

#include "simpleworld.hpp"
#include "worlderror.hpp"
#include "actionerror.hpp"
#include "bugdeath.hpp"
#include "movement.hpp"
#include "mutation.hpp"

// Default values for the environment
#define DEFAULT_SIZE (Position) {16, 16}

#define DEFAULT_MUTATIONS_PROBABILITY 0.001
#define DEFAULT_TIME_BIRTH 32
#define DEFAULT_TIME_MUTATE (16 * 1024)

#define DEFAULT_TIME_LAZINESS 1024
#define DEFAULT_ENERGY_LAZINESS 16

#define DEFAULT_ATTACK_MULTIPLIER 2.5

#define DEFAULT_ENERGY_NOTHING 0
#define DEFAULT_ENERGY_MYSELF 1
#define DEFAULT_ENERGY_DETECT 1
#define DEFAULT_ENERGY_INFO 1
#define DEFAULT_ENERGY_MOVE 2
#define DEFAULT_ENERGY_TURN 2
#define DEFAULT_ENERGY_ATTACK 3
#define DEFAULT_ENERGY_EAT 3
#define DEFAULT_ENERGY_EGG 4

namespace simpleworld
{

/**
 * Constructor.
 * @param filename File name of the database.
 * @exception DBException if there is a error in the database.
 */
SimpleWorld::SimpleWorld(std::string filename)
  : DB(filename)
{
  this->on_open();
}

/**
 * Destructor.
 */
SimpleWorld::~SimpleWorld()
{
  // Free the food
  std::list<Food*>::iterator food = this->foods_.begin();
  while (food != this->foods_.end()) {
    delete (*food);
    ++food;
  }

  // Free the eggs
  std::list<Egg*>::iterator egg = this->eggs_.begin();
  while (egg != this->eggs_.end()) {
    delete (*egg);
    ++egg;
  }

  // Free the bugs
  std::list<Bug*>::iterator bug = this->bugs_.begin();
  while (bug != this->bugs_.end()) {
    delete (*bug);
    ++bug;
  }


  // Free the world
  delete this->world_;
  // Free the environment
  delete this->env_;
}


/**
 * Add a egg to the World.
 * @param energy Energy of the egg.
 * @param position Position of the egg.
 * @param orientation Orientation of the egg.
 * @param code Code of the egg.
 * @exception WorldError if the position is out of the limits.
 * @exception WorldError if the position is used.
 */
void SimpleWorld::add_egg(Energy energy,
                          Position position, Orientation orientation,
                          const cpu::Memory& code)
{
  // begin a transaction
  sqlite3x::sqlite3_transaction transaction(*this);

  db::ID id;
  try {
    boost::scoped_array<cpu::Word> data(new cpu::Word[code.size()]);
    for (cpu::Address i = 0; i < code.size(); i += sizeof(cpu::Word))
      data[i / sizeof(cpu::Word)] = code.get_word(i, false);

    id = db::Bug::insert(this, data.get(), code.size());
    db::ID world_id = db::World::insert(this, position.x, position.y,
                                        orientation);
    db::Egg::insert(this, id, world_id, energy, this->env_->time());
  } catch (const db::DBException& e) {
    throw EXCEPTION(WorldError, e.what);
  }

  Egg* egg = new Egg(this, id);
  this->eggs_.push_back(egg);
  this->world_->add(egg, position);

  transaction.commit();
}

/**
 * Add food to the World.
 * @param position Position of the food.
 * @param size Size of the food.
 * @exception WorldError if the position is out of the limits.
 * @exception WorldError if the position is used.
 */
void SimpleWorld::add_food(Position position, Energy size)
{
  // begin a transaction
  sqlite3x::sqlite3_transaction transaction(*this);

  db::ID id;
  try {
    db::ID world_id = db::World::insert(this, position.x, position.y);
    id = db::Food::insert(this, world_id, size);
  } catch (const db::DBException& e) {
    throw EXCEPTION(WorldError, e.what);
  }

  Food* food = new Food(this, id);
  this->foods_.push_back(food);
  this->world_->add(food, position);

  transaction.commit();
}


/**
 * Execute some cycles of the World.
 * @param cycles Cycles to be executed.
 */
void SimpleWorld::run(Time cycles)
{
  for (; cycles > 0; cycles--) {
    // execute each cycle in a transaction
    sqlite3x::sqlite3_transaction transaction(*this);

    // update the time of the environment
    Time time = this->env_->time() + 1;
    this->env_->time(time);

    this->bugs_mutate();
    if (time % 64 == 0)
      this->bugs_timer();
    this->bugs_run();
    this->bugs_laziness();
    this->eggs_birth();

    transaction.commit();
  }
}


/**
 * Do nothing.
 * @param bug Bug that executes the action.
 * @exception ActionError if something fails.
 */
void SimpleWorld::nothing(Bug* bug)
{
#ifdef DEBUG
  std::cout << boost::format("Bug %1% doing action nothing")
    % bug->id()
    << std::endl;
#endif // DEBUG

  this->substract_energy(bug, this->env_->energy_nothing());
}

/**
 * Get information about the bug itself.
 * @param bug Bug that executes the action.
 * @param info Type of information requested.
 * @param ypos Where to save the Y coord when requested the position.
 * @return The information.
 * @exception ActionError if something fails.
 */
cpu::Word SimpleWorld::myself(Bug* bug, Info info, cpu::Word* ypos)
{
#ifdef DEBUG
  std::cout << boost::format("Bug %1% doing action myself")
    % bug->id()
    << std::endl;
#endif // DEBUG

  this->substract_energy(bug, this->env_->energy_myself());

  switch (info) {
  case InfoID:
    return static_cast<cpu::Word>(bug->id());

  case InfoSize:
    return static_cast<cpu::Word>(bug->code().size());

  case InfoEnergy:
    return static_cast<cpu::Word>(bug->energy());

  case InfoPosition:
    *ypos = static_cast<cpu::Word>(bug->position_y());
    return static_cast<cpu::Word>(bug->position_x());

  case InfoOrientation:
    return static_cast<cpu::Word>(bug->orientation());

  default:
    throw EXCEPTION(ActionError, boost::str(boost::format(\
"Type of information (0x%04x) unknown")
                                            % info));
  }
}

/**
 * Get the type of element that is in front of the bug.
 * @param bug Bug that executes the action.
 * @return The type of element.
 * @exception ActionError if something fails.
 */
ElementType SimpleWorld::detect(Bug* bug)
{
#ifdef DEBUG
  std::cout << boost::format("Bug %1% doing action detect")
    % bug->id()
    << std::endl;
#endif // DEBUG

  this->substract_energy(bug, this->env_->energy_detect());

  Position front = this->front(bug);
  try {
    return this->world_->get(front)->type;
  } catch (const WorldError& e) {
    return ElementNothing;
  }
}

/**
 * Get information about the bug that is in front of the bug.
 * @param bug Bug that executes the action.
 * @param info Type of information requested.
 * @param ypos Where to save the Y coord when requested the position.
 * @return The information.
 * @exception ActionError if something fails.
 */
cpu::Word SimpleWorld::information(Bug* bug, Info info, cpu::Word* ypos)
{
#ifdef DEBUG
  std::cout << boost::format("Bug %1% doing action information")
    % bug->id()
    << std::endl;
#endif // DEBUG

  this->substract_energy(bug, this->env_->energy_info());

  Position front = this->front(bug);
  Element* target;
  try {
    target = this->world_->get(front);
  } catch (const WorldError& e) {
    throw EXCEPTION(ActionError, boost::str(boost::format("\
There is nothing in (%1%, %2%)")
                                            % front.x
                                            % front.y));
  }
  // ElementNothing is not a valid type, it's only send to bugs to inform
  // them that there is nothing in the World
  assert(target->type != ElementNothing);

  switch (info) {
  case InfoID:  // Only eggs and bugs
    if (target->type == ElementEgg or target->type == ElementBug)
      return static_cast<cpu::Word>(static_cast<Bug*>(target)->id());
    else
      throw EXCEPTION(ActionError, "Food has no id");

  case InfoSize: // Every element
    if (target->type == ElementFood)
      return static_cast<cpu::Word>(static_cast<Food*>(target)->size());
    else
      return static_cast<cpu::Word>(static_cast<Bug*>(target)->code().size());

  case InfoEnergy: // Only eggs and bugs
    if (target->type == ElementEgg or target->type == ElementBug)
      return static_cast<cpu::Word>(static_cast<Bug*>(target)->energy());
    else
      throw EXCEPTION(ActionError, "Food has no energy");

  case InfoPosition: // Every element
    *ypos = static_cast<cpu::Word>(front.y);
    return static_cast<cpu::Word>(front.x);

  case InfoOrientation: // Only eggs and bugs
    if (target->type == ElementEgg or target->type == ElementBug)
      return static_cast<cpu::Word>(static_cast<Bug*>(target)->orientation());
    else
      throw EXCEPTION(ActionError, "Food has no orientation");

  default:
    throw EXCEPTION(ActionError, boost::str(boost::format(\
"Type of information (0x%04x) unknown")
                                            % info));
  }
}

/**
 * Move a bug.
 * @param bug Bug that executes the action.
 * @param movement Type of movement.
 * @exception ActionError if something fails.
 */
void SimpleWorld::move(Bug* bug, Movement movement)
{
#ifdef DEBUG
  std::cout << boost::format("Bug %1% doing action move")
    % bug->id()
    << std::endl;
#endif // DEBUG

  this->substract_energy(bug, this->env_->energy_move());

  try {
    Position current(bug->position_x(), bug->position_y());
    Position front = this->front(bug);
    this->world_->move(current, front);
    if (current.x != front.x)
      bug->position_x(front.x);
    else
      bug->position_y(front.y);
  } catch (const Exception& e) {
    throw EXCEPTION(ActionError, e.what);
  }
}

/**
 * Turn a bug.
 * @param bug Bug that executes the action.
 * @param turn Type of turn.
 * @exception ActionError if something fails.
 */
void SimpleWorld::turn(Bug* bug, Turn turn)
{
#ifdef DEBUG
  std::cout << boost::format("Bug %1% doing action turn")
    % bug->id()
    << std::endl;
#endif // DEBUG

  this->substract_energy(bug, this->env_->energy_turn());

  try {
    bug->orientation(::simpleworld::turn(bug->orientation(), turn));
  } catch (const Exception& e) {
    throw EXCEPTION(ActionError, e.what);
  }
}

/**
 * Attack.
 * @param bug Bug that executes the action.
 * @param energy Energy used in the attack.
 * @exception ActionError if something fails.
 */
void SimpleWorld::attack(Bug* bug, Energy energy)
{
#ifdef DEBUG
  std::cout << boost::format("Bug %1% doing action attack")
    % bug->id()
    << std::endl;
#endif // DEBUG

  this->substract_energy(bug, this->env_->energy_attack());

  // search the target of the attack
  Position front = this->front(bug);
  Element* target;
  try {
    target = this->world_->get(front);
  } catch (const WorldError& e) {
    throw EXCEPTION(ActionError, boost::str(boost::format("\
There is nothing in (%1%, %2%)")
                                            % front.x
                                            % front.y));
  }
  // ElementNothing is not a valid type, it's only send to bugs to inform
  // them that there is nothing in the World
  assert(target->type != ElementNothing);

  if ((target->type != ElementBug and target->type != ElementEgg))
    throw EXCEPTION(ActionError, boost::str(boost::format("\
There is not a egg/bug in (%1%, %2%)")
                                            % front.x
                                            % front.y));

  // attack the target
  if (target->type == ElementBug) {
    Bug* bug_target = dynamic_cast<Bug*>(target);
    try {
      this->substract_energy(bug_target,
                             energy * this->env_->attack_multiplier());
      bug_target->attacked();
    } catch (const BugDeath& e) {
      // the bug is death
      this->kill(bug_target, bug->id());
    }
  } else {
    Egg* egg_target = dynamic_cast<Egg*>(target);
    try {
      this->substract_energy(egg_target,
                             energy * this->env_->attack_multiplier());
    } catch (const BugDeath& e) {
      // the egg is death
      this->kill(egg_target, bug->id());
    }
  }

  // substract the energy of the attack to the bug
  this->substract_energy(bug, energy);
}

/**
 * Eat what is in front of the bug.
 * @param bug Bug that executes the action.
 * @return The energy eaten.
 * @exception ActionError if something fails.
 */
Energy SimpleWorld::eat(Bug* bug)
{
#ifdef DEBUG
  std::cout << boost::format("Bug %1% doing action eat")
    % bug->id()
    << std::endl;
#endif // DEBUG

  this->substract_energy(bug, this->env_->energy_eat());

  Position front = this->front(bug);
  Element* target;
  try {
    target = this->world_->get(front);
  } catch (const WorldError& e) {
    throw EXCEPTION(ActionError, boost::str(boost::format("\
There is nothing in (%1%, %2%)")
                                            % front.x
                                            % front.y));
  }
  // ElementNothing is not a valid type, it's only send to bugs to inform
  // them that there is nothing in the World
  assert(target->type != ElementNothing);

  if (target->type != ElementFood)
    throw EXCEPTION(ActionError, boost::str(boost::format("\
There is nothing to eat in (%1%, %2%")
                                            % front.x
                                            % front.y));

  Food* food_target = dynamic_cast<Food*>(target);
  Energy energy = food_target->size();
  bug->energy(bug->energy() + energy);

  db::World::remove(this, food_target->world_id());
  db::Food::remove(this, food_target->id());
  this->world_->remove(front);
  this->foods_.remove(food_target);
  delete food_target;

  return energy;
}

/**
 * Create a egg in front of the bug.
 * @param bug Bug that executes the action.
 * @param energy Energy transfered to the egg.
 * @exception ActionError if something fails.
 */
void SimpleWorld::egg(Bug* bug, Energy energy)
{
#ifdef DEBUG
  std::cout << boost::format("Bug %1% doing action egg")
    % bug->id()
    << std::endl;
#endif // DEBUG

  this->substract_energy(bug, this->env_->energy_egg());

  if (energy == 0)
    throw EXCEPTION(ActionError, "Trying to create a egg without energy");

  Position front = this->front(bug);
  if (this->world_->used(front))
    throw EXCEPTION(ActionError, boost::str(boost::format("\
Position used (%1%, %2%)")
                                            % front.x
                                            % front.y));


  // savepoint
  sqlite3x::sqlite3_command sql(*this);

  try {
    sql.prepare("SAVEPOINT egg;");

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(db::DBException, std::string(e.what()) +
                    " (" + this->errormsg() + ")");
  }

  // the egg is looking to the father
  db::ID world_id = db::World::insert(this, front.x, front.y,
                                      ::simpleworld::turn(::simpleworld::turn(bug->orientation(),
                                                                              TurnLeft),
                                                          TurnLeft));
  Uint32 size;
  boost::shared_array<Uint8> code = bug->code().read(&size);
  db::ID egg_id = db::Bug::insert(this, bug->id(), code.get(), size);
  db::Egg::insert(this, egg_id, world_id, std::min(bug->energy(), energy),
                  this->env_->time());
  Egg* ptr = new Egg(this, egg_id);
  mutate(ptr, this->env_->mutations_probability(), this->env_->time());

  try {
    // Substracts the size of the egg
    this->substract_energy(bug, db::Bug(this, egg_id).code().size());
  } catch (const BugDeath& e) {
    try {
      sql.prepare("ROLLBACK TO egg;");
      sql.executenonquery();
      sql.prepare("RELEASE egg;");
      sql.executenonquery();
    } catch (const sqlite3x::database_error& e) {
      throw EXCEPTION(db::DBException, std::string(e.what()) +
                      " (" + this->errormsg() + ")");
    }

    throw;
  }

  this->eggs_.push_back(ptr);
  this->world_->add(ptr, Position(ptr->position_x(), ptr->position_y()));

  // Substracts the energy of the egg
  this->substract_energy(bug, energy);

  // savepoint
  try {
    sql.prepare("RELEASE egg;");

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(db::DBException, std::string(e.what()) +
                    " (" + this->errormsg() + ")");
  }
}


/**
 * This function is called when open() succeeds. Subclasses
 * which wish to do custom db initialization or sanity checks
 * may do them here.
 */
void SimpleWorld::on_open()
{
  DB::on_open();

  // Load the environment
  if (this->environments().empty()) {
    db::Environment::insert(this, 0, DEFAULT_SIZE.x, DEFAULT_SIZE.y,
                            DEFAULT_MUTATIONS_PROBABILITY, DEFAULT_TIME_BIRTH,
                            DEFAULT_TIME_MUTATE, DEFAULT_TIME_LAZINESS,
                            DEFAULT_ENERGY_LAZINESS, DEFAULT_ATTACK_MULTIPLIER,
                            DEFAULT_ENERGY_NOTHING, DEFAULT_ENERGY_MYSELF,
                            DEFAULT_ENERGY_DETECT, DEFAULT_ENERGY_INFO,
                            DEFAULT_ENERGY_MOVE, DEFAULT_ENERGY_TURN,
                            DEFAULT_ENERGY_ATTACK, DEFAULT_ENERGY_EAT,
                            DEFAULT_ENERGY_EGG);
  }

  this->env_ = new db::Environment(this, this->last_environment());

  this->world_ = new World(Position(this->env_->size_x(), this->env_->size_y()));


  // Load the elements of the world
  std::vector< db::ID > ids;

  ids = this->food();
  for (std::vector< db::ID >::const_iterator iter = ids.begin();
       iter != ids.end();
       ++iter) {
    Food* ptr = new Food(this, *iter);
    this->foods_.push_back(ptr);
    this->world_->add(ptr, Position(ptr->position_x(), ptr->position_y()));
  }

  ids = this->eggs();
  for (std::vector< db::ID >::const_iterator iter = ids.begin();
       iter != ids.end();
       ++iter) {
    Egg* ptr = new Egg(this, *iter);
    this->eggs_.push_back(ptr);
    this->world_->add(ptr, Position(ptr->position_x(), ptr->position_y()));
  }

  ids = this->alive_bugs();
  for (std::vector< db::ID >::const_iterator iter = ids.begin();
       iter != ids.end();
       ++iter) {
    Bug* ptr = new Bug(this, *iter);
    this->bugs_.push_back(ptr);
    this->world_->add(ptr, Position(ptr->position_x(), ptr->position_y()));
  }
}

/**
 * Position in front of a given bug.
 * @param bug The bug.
 * @return Position in front of the bug.
 */
Position SimpleWorld::front(Bug* bug)
{
  return ::simpleworld::move(Position(bug->position_x(), bug->position_y()),
                             bug->orientation(), MoveForward,
                             this->world_->size());
}


/**
 * Convert in bugs all the eggs that are old enough.
 */
void SimpleWorld::eggs_birth()
{
  // check for the birthday of each egg
  std::list<Egg*> eggs = this->eggs_;
  for (std::list<Egg*>::iterator egg = eggs.begin();
       egg != eggs.end();
       ++egg)
    if (((*egg)->conception() + this->env_->time_birth()) <= this->env_->time())
      this->birth(*egg);
}

/**
 * Mutate the code of the old bugs.
 */
void SimpleWorld::bugs_mutate()
{
  // check that time_mutate is not zero to prevent a float point exception
  if (this->env_->time_mutate() == 0)
    return;

  // check if the bug is old enough
  for (std::list<Bug*>::iterator bug = this->bugs_.begin();
       bug != this->bugs_.end();
       ++bug) {
    Time age = this->env_->time() - (*bug)->birth();
    if ((age > 0) and (age % this->env_->time_mutate() == 0))
      if (mutate((*bug), this->env_->mutations_probability(),
          this->env_->time())) {
        (*bug)->mutated();

#ifdef DEBUG
  std::cout << boost::format("The code of bug %1% has been mutated")
    % (*bug)->id()
    << std::endl;
#endif // DEBUG
        }
  }
}

/**
 * Throw the Timer Interrupt in all the alive bugs.
 */
void SimpleWorld::bugs_timer()
{
  // throw the Timer Interrupt in each bug
  std::list<Bug*> bugs = this->bugs_;
  for (std::list<Bug*>::iterator bug = bugs.begin();
       bug != bugs.end();
       ++bug) {
    try {
      // throw the interrupt
      (*bug)->cpu.timer_interrupt();
    } catch (const cpu::CPUException& e) {
      // some critical error
      this->kill(*bug);
    }
  }
}

/**
 * Substract some energy to the egg.
 * If the egg has not enough energy, it's converted in food.
 * @param egg The egg.
 * @param energy The energy.
 * @exception BugDeath If the bug die.
 */
void SimpleWorld::substract_energy(Egg* egg, Energy energy)
{
  if (egg->energy() <= energy)
    throw EXCEPTION(BugDeath, boost::str(boost::format("\
Egg %1% is death")
                                         % egg->id()));

  egg->energy(egg->energy() - energy);
}

/**
 * Substract some energy to the bug.
 * If the bug has not enough energy, it's converted in food.
 * @param bug The bug.
 * @param energy The energy.
 * @exception BugDeath If the bug die.
 */
void SimpleWorld::substract_energy(Bug* bug, Energy energy)
{
  if (bug->energy() <= energy)
    throw EXCEPTION(BugDeath, boost::str(boost::format("\
Bug %1% is death")
                                         % bug->id()));

  bug->energy(bug->energy() - energy);
}

/**
 * Convert a egg into a bug.
 * @param egg The egg.
 */
void SimpleWorld::birth(Egg* egg)
{
  // Convert the egg into a bug
  db::ID id = db::AliveBug::insert(this, egg, this->env_->time());
  Bug* bug = new Bug(this, id);

  Position position(bug->position_x(), bug->position_y());
  this->world_->remove(position);
  this->world_->add(bug, position);
  this->bugs_.push_back(bug);
#ifdef DEBUG
  std::cout << boost::str(boost::format("\
Bug[%1%] born")
                          % bug->id())
            << std::endl;
#endif // DEBUG
  this->eggs_.remove(egg);
  delete egg;
}

/**
 * Kill the egg and convert it into food.
 * @param egg The egg.
 */
void SimpleWorld::kill(Egg* egg)
{
  // Convert the egg in food
  db::ID id = db::Food::insert(this, egg->world_id(), egg->code().size());
  db::DeadBug::insert(this, egg, this->env_->time());

  Food* food = new Food(this, id);
  Position position(food->position_x(), food->position_y());
  this->foods_.push_back(food);
  this->world_->remove(position);
  this->world_->add(food, position);
#ifdef DEBUG
  std::cout << boost::format("\
Food[%1%] added at (%2%, %3%) with a size of %4%")
    % food->id()
    % food->position_x()
    % food->position_y()
    % food->size()
            << std::endl;
#endif // DEBUG

  // Remove the dead bug
  this->eggs_.remove(egg);
#ifdef DEBUG
  std::cout << boost::str(boost::format("\
Egg[%1%] died")
                          % egg->id())
            << std::endl;
#endif // DEBUG
  delete egg;
}

/**
 * Kill the egg and convert it into food.
 * @param egg The egg.
 * @param killer_id Who killed it.
 */
void SimpleWorld::kill(Egg* egg, db::ID killer_id)
{
  // Convert the egg in food
  db::ID id = db::Food::insert(this, egg->world_id(), egg->code().size());
  db::DeadBug::insert(this, egg, this->env_->time(), killer_id);

  Food* food = new Food(this, id);
  Position position(food->position_x(), food->position_y());
  this->foods_.push_back(food);
  this->world_->remove(position);
  this->world_->add(food, position);
#ifdef DEBUG
  std::cout << boost::format("\
Food[%1%] added at (%2%, %3%) with a size of %4%")
    % food->id()
    % food->position_x()
    % food->position_y()
    % food->size()
            << std::endl;
#endif // DEBUG

  // Remove the dead bug
  this->eggs_.remove(egg);
#ifdef DEBUG
  std::cout << boost::str(boost::format("\
Egg[%1%] died")
                          % egg->id())
            << std::endl;
#endif // DEBUG
  delete egg;
}

/**
 * Kill the bug and convert it into food.
 * @param bug The bug.
 */
void SimpleWorld::kill(Bug* bug)
{
  // Convert the bug in food
  db::ID id = db::Food::insert(this, bug->world_id(), bug->code().size());
  db::DeadBug::insert(this, bug, this->env_->time());

  Food* food = new Food(this, id);
  Position position(food->position_x(), food->position_y());
  this->foods_.push_back(food);
  this->world_->remove(position);
  this->world_->add(food, position);
#ifdef DEBUG
  std::cout << boost::format("\
Food[%1%] added at (%2%, %3%) with a size of %4%")
    % food->id()
    % food->position_x()
    % food->position_y()
    % food->size()
            << std::endl;
#endif // DEBUG

  // Remove the dead bug
  this->bugs_.remove(bug);
#ifdef DEBUG
  std::cout << boost::str(boost::format("\
Bug[%1%] died")
                          % bug->id())
            << std::endl;
#endif // DEBUG
  delete bug;
}

/**
 * Kill the bug and convert it into food.
 * @param bug The bug.
 * @param killer_id Who killed it.
 */
void SimpleWorld::kill(Bug* bug, db::ID killer_id)
{
  // Convert the bug in food
  db::ID id = db::Food::insert(this, bug->world_id(), bug->code().size());
  db::DeadBug::insert(this, bug, this->env_->time(), killer_id);

  Food* food = new Food(this, id);
  Position position(food->position_x(), food->position_y());
  this->foods_.push_back(food);
  this->world_->remove(position);
  this->world_->add(food, position);
#ifdef DEBUG
  std::cout << boost::format("\
Food[%1%] added at (%2%, %3%) with a size of %4%")
    % food->id()
    % food->position_x()
    % food->position_y()
    % food->size()
            << std::endl;
#endif // DEBUG

  // Remove the dead bug
  this->bugs_.remove(bug);
#ifdef DEBUG
  std::cout << boost::str(boost::format("\
Bug[%1%] died")
                          % bug->id())
            << std::endl;
#endif // DEBUG
  delete bug;
}

/**
 * Execute a cycle in all the alive bugs.
 */
void SimpleWorld::bugs_run()
{
  // execute a instruction in each bug
  std::list<Bug*> bugs = this->bugs_;
  for (std::list<Bug*>::iterator bug = bugs.begin();
       bug != bugs.end();
       ++bug) {
    // check if the bug is alive, a bug could have killed it
    if (std::find(this->bugs_.begin(), this->bugs_.end(), *bug) ==
        this->bugs_.end())
      continue;

    try {
      // execute a instruction
      (*bug)->cpu.next();
    } catch (const cpu::CPUException& e) {
      // some uncaught error in the CPU (CPU stopped)
      this->kill(*bug);
    } catch (const BugDeath& e) {
      // the bug is death
      this->kill(*bug);
    }
  }
}

/**
 * Penalize the bugs that don't do any action.
 */
void SimpleWorld::bugs_laziness()
{
  // check the laziness of each bug
  std::list<Bug*> bugs = this->bugs_;
  for (std::list<Bug*>::iterator bug = bugs.begin();
       bug != bugs.end();
       ++bug) {
    // calculate the time since the last action
    Time time;
    if ((*bug)->is_null("time_last_action"))
      time = this->env_->time() - (*bug)->birth();
    else
      time = this->env_->time() - (*bug)->time_last_action();

    if (time > 0 and time % this->env_->time_laziness() == 0) {
#ifdef DEBUG
  std::cout << boost::format("Bug %1% is lazy")
    % (*bug)->id()
    << std::endl;
#endif // DEBUG

      try {
        this->substract_energy((*bug), this->env_->energy_laziness());
      } catch (const BugDeath& e) {
        // the bug is death
        this->kill(*bug);
      }
    }
  }
}

}
