/**
 * @file simpleworld/simpleworld.hpp
 * Simple World: the simulation of a world and the bugs that live on it.
 *
 * Simple World is the simulation of a world and the bugs that live on it.
 * Simple World tries to reproduce the basic elements that define a simple
 * world. The objective of the project is to observe the evolution of this
 * world and of these bugs.
 *
 * begin:     Wed, 22 Aug 2007 14:59:02 +0200
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

#if DEBUG
#include <iostream>
#endif // DEBUG

#include <vector>
#include <list>
#include <cassert>

#include <boost/format.hpp>

#include <sqlite3x.hpp>

#include <simpleworld/cpu/types.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/food.hpp>
#include <simpleworld/db/egg.hpp>
#include <simpleworld/db/bug.hpp>

#include "simpleworld.hpp"
#include "exception.hpp"
#include "actionerror.hpp"
#include "bugdeath.hpp"
#include "movement.hpp"

// Default values for the environment
#define DEFAULT_SIZE (Position) {16, 16}
#define DEFAULT_ENERGY_DEVELOPED 64
#define DEFAULT_MUTATIONS_PROBABILITY 0.001
#define DEFAULT_TIME_BIRTH 32
#define DEFAULT_ENERGY_NOTHING 0
#define DEFAULT_ENERGY_MYSELF 1
#define DEFAULT_ENERGY_DETECT 1
#define DEFAULT_ENERGY_INFO 1
#define DEFAULT_ENERGY_MOVE 2
#define DEFAULT_ENERGY_TURN 2
#define DEFAULT_ENERGY_ATTACK 3
#define DEFAULT_ENERGY_EAT 3
#define DEFAULT_ENERGY_EGG 4

namespace SimpleWorld
{

SimpleWorld::SimpleWorld(std::string filename)
  : DB(filename)
{
  this->on_open();
}

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


void SimpleWorld::add_egg(Energy energy,
                          Position position, Orientation orientation,
                          const CPU::Memory& code)
{
  // begin a transaction
  sqlite3x::sqlite3_transaction transaction(*this);

  ::SimpleWorld::DB::Egg egg(this);
  egg.position = position;
  egg.orientation = orientation;
  egg.birth = this->env_->time + this->env_->time_birth;
  egg.add_null("father_id");
  egg.energy = energy;
  egg.code.size = code.size();
  egg.code.code = code;
  egg.code.update_md5();
  egg.insert();

  Egg* ptr = new Egg(this, egg.id());
  this->eggs_.push_back(ptr);
  this->world_->add(ptr, ptr->position);

  transaction.commit();
}

void SimpleWorld::add_food(Position position, Energy size)
{
  // begin a transaction
  sqlite3x::sqlite3_transaction transaction(*this);

  ::SimpleWorld::DB::Food food(this);
  food.position = position;
  food.size = size;
  food.insert();

  Food* ptr = new Food(this, food.id());
  this->foods_.push_back(ptr);
  this->world_->add(ptr, ptr->position);

  transaction.commit();
}


void SimpleWorld::run(Time cycles)
{
  for (; cycles > 0; cycles--) {
    this->env_->time++;

    // execute each cycle in a transaction
    sqlite3x::sqlite3_transaction transaction(*this);

    if (this->env_->time % 64 == 0)
      this->bugs_timer();
    this->bugs_run();
    this->eggs_birth();

    // update the time of the environment
    this->env_->update_db(true);

    // update the database
    this->update_db();

    transaction.commit();
  }
}


void SimpleWorld::nothing(Bug* bug)
{
#ifdef DEBUG
  std::cout << boost::format("Bug %1% doing action nothing")
    % bug->id()
    << std::endl;
#endif // DEBUG

  this->substract_energy(bug, this->env_->energy_nothing);
  bug->changed = true;
}

CPU::Word SimpleWorld::myself(Bug* bug, Info info, CPU::Word* ypos)
{
#ifdef DEBUG
  std::cout << boost::format("Bug %1% doing action myself")
    % bug->id()
    << std::endl;
#endif // DEBUG

  this->substract_energy(bug, this->env_->energy_myself);
  bug->changed = true;

  switch (info) {
  case InfoID:
    return static_cast<CPU::Word>(bug->id());

  case InfoSize:
    return static_cast<CPU::Word>(bug->code.size);

  case InfoEnergy:
    return static_cast<CPU::Word>(bug->energy);

  case InfoPosition:
    *ypos = static_cast<CPU::Word>(bug->position.y);
    return static_cast<CPU::Word>(bug->position.x);

  case InfoOrientation:
    return static_cast<CPU::Word>(bug->orientation);

  default:
    throw EXCEPTION(ActionError, boost::str(boost::format(\
"Type of information (%04x) unknown")
                                            % info));
  }
}

ElementType SimpleWorld::detect(Bug* bug)
{
#ifdef DEBUG
  std::cout << boost::format("Bug %1% doing action detect")
    % bug->id()
    << std::endl;
#endif // DEBUG

  this->substract_energy(bug, this->env_->energy_detect);
  bug->changed = true;

  Position front = this->front(bug);
  try {
    return this->world_->get(front)->type;
  } catch (const WorldError& e) {
    return ElementNothing;
  }
}

CPU::Word SimpleWorld::information(Bug* bug, Info info, CPU::Word* ypos)
{
#ifdef DEBUG
  std::cout << boost::format("Bug %1% doing action information")
    % bug->id()
    << std::endl;
#endif // DEBUG

  this->substract_energy(bug, this->env_->energy_info);
  bug->changed = true;

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
    return static_cast<CPU::Word>(static_cast< ::SimpleWorld::DB::BugElement* >(target)->id());

  case InfoSize: // Every element
    if (target->type == ElementFood)
      return static_cast<CPU::Word>(static_cast<Food*>(target)->size);
    else
      return static_cast<CPU::Word>(static_cast< ::SimpleWorld::DB::BugElement* >(target)->code.size);

  case InfoEnergy: // Only eggs and bugs
    return static_cast<CPU::Word>(static_cast< ::SimpleWorld::DB::AliveBug* >(target)->energy);

  case InfoPosition: // Every element
    *ypos = static_cast<CPU::Word>(target->position.y);
    return static_cast<CPU::Word>(target->position.x);

  case InfoOrientation: // Only eggs and bugs
    return static_cast<CPU::Word>(static_cast< ::SimpleWorld::DB::BugElement* >(target)->orientation);

  default:
    throw EXCEPTION(ActionError, boost::str(boost::format(\
"Type of information (%04x) unknown")
                                            % info));
  }
}

void SimpleWorld::move(Bug* bug, Movement movement)
{
#ifdef DEBUG
  std::cout << boost::format("Bug %1% doing action move")
    % bug->id()
    << std::endl;
#endif // DEBUG

  this->substract_energy(bug, this->env_->energy_move);
  bug->changed = true;

  try {
    this->world_->move(bug->position, this->front(bug));
  } catch (const Exception& e) {
    throw EXCEPTION(ActionError, e.what);
  }
}

void SimpleWorld::turn(Bug* bug, Turn turn)
{
#ifdef DEBUG
  std::cout << boost::format("Bug %1% doing action turn")
    % bug->id()
    << std::endl;
#endif // DEBUG

  this->substract_energy(bug, this->env_->energy_turn);
  bug->changed = true;

  try {
    bug->orientation = ::SimpleWorld::turn(bug->orientation, turn);
  } catch (const Exception& e) {
    throw EXCEPTION(ActionError, e.what);
  }
}

void SimpleWorld::attack(Bug* bug, Energy energy)
{
#ifdef DEBUG
  std::cout << boost::format("Bug %1% doing action attack")
    % bug->id()
    << std::endl;
#endif // DEBUG

  this->substract_energy(bug, this->env_->energy_attack);
  bug->changed = true;

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
      this->substract_energy(bug_target, energy);
      bug_target->attacked();
      bug_target->changed = true;
    } catch (const BugDeath& e) {
      // the bug is death
      this->kill(bug_target);
    }
  } else {
    Egg* egg_target = dynamic_cast<Egg*>(target);
    try {
      this->substract_energy(egg_target, energy);
      egg_target->changed = true;
    } catch (const BugDeath& e) {
      // the egg is death
      this->kill(egg_target);
    }
  }

  // substract the energy of the attack to the bug
  this->substract_energy(bug, energy);
}

Energy SimpleWorld::eat(Bug* bug)
{
#ifdef DEBUG
  std::cout << boost::format("Bug %1% doing action eat")
    % bug->id()
    << std::endl;
#endif // DEBUG

  this->substract_energy(bug, this->env_->energy_eat);
  bug->changed = true;

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
  Energy energy = food_target->size;
  bug->energy += energy;
  bug->changed = true;

  food_target->remove();
  this->world_->remove(food_target->position);
  this->foods_.remove(food_target);
  delete food_target;

  return energy;
}

void SimpleWorld::egg(Bug* bug, Energy energy)
{
#ifdef DEBUG
  std::cout << boost::format("Bug %1% doing action egg")
    % bug->id()
    << std::endl;
#endif // DEBUG

  this->substract_energy(bug, this->env_->energy_egg);
  bug->changed = true;

  // TODO: Do something :)
}


void SimpleWorld::on_open()
{
  DB::on_open();

  // Load the environment
  if (this->environments().empty()) {
    this->env_ = new ::SimpleWorld::DB::Environment(this);
    this->env_->time = 0;
    this->env_->size = DEFAULT_SIZE;
    this->env_->energy_developed = DEFAULT_ENERGY_DEVELOPED;
    this->env_->mutations_probability = DEFAULT_MUTATIONS_PROBABILITY;
    this->env_->time_birth = DEFAULT_TIME_BIRTH;
    this->env_->energy_nothing = DEFAULT_ENERGY_NOTHING;
    this->env_->energy_myself = DEFAULT_ENERGY_MYSELF;
    this->env_->energy_detect = DEFAULT_ENERGY_DETECT;
    this->env_->energy_info = DEFAULT_ENERGY_INFO;
    this->env_->energy_move = DEFAULT_ENERGY_MOVE;
    this->env_->energy_turn = DEFAULT_ENERGY_TURN;
    this->env_->energy_attack = DEFAULT_ENERGY_ATTACK;
    this->env_->energy_eat = DEFAULT_ENERGY_EAT;
    this->env_->energy_egg = DEFAULT_ENERGY_EGG;
    this->env_->insert();
  } else {
    this->env_ = new ::SimpleWorld::DB::Environment(this,
      this->last_environment());
  }

  this->world_ = new World(this->env_->size);


  // Load the elements of the world
  std::vector< ::SimpleWorld::DB::ID > ids;
  std::vector< ::SimpleWorld::DB::ID >::const_iterator iter;

  ids = this->food();
  for (iter = ids.begin(); iter != ids.end(); ++iter) {
    Food* ptr = new Food(this, *iter);
    this->foods_.push_back(ptr);
    this->world_->add(ptr, ptr->position);
  }

  ids = this->eggs();
  for (iter = ids.begin(); iter != ids.end(); ++iter) {
    Egg* ptr = new Egg(this, *iter);
    this->eggs_.push_back(ptr);
    this->world_->add(ptr, ptr->position);
  }

  ids = this->alive_bugs();
  for (iter = ids.begin(); iter != ids.end(); ++iter) {
    Bug* ptr = new Bug(this, *iter);
    this->bugs_.push_back(ptr);
    this->world_->add(ptr, ptr->position);
  }
}


Position SimpleWorld::front(Bug* bug)
{
  return ::SimpleWorld::move(bug->position, bug->orientation,
                             MoveForward, this->world_->size());
}


void SimpleWorld::eggs_birth()
{
  // check for the birthday of each egg
  std::list<Egg*> eggs = this->eggs_;
  std::list<Egg*>::iterator egg;
  for (egg = eggs.begin(); egg != eggs.end(); ++egg) {
    if ((*egg)->birth <= this->env_->time) {
      // convert the egg to a bug
      Bug* bug = new Bug(this, (*egg)->be_born());

      this->world_->remove((*egg)->position);
      this->world_->add(bug, bug->position);
      this->bugs_.push_back(bug);
      this->eggs_.remove(*egg);
      delete (*egg);
    }
  }
}

void SimpleWorld::bugs_timer()
{
  // throw the Timer Interrupt in each bug
  std::list<Bug*>::iterator current;
  for (current = this->bugs_.begin();
       current != this->bugs_.end();
       ++current)
    // throw the interrupt
    (*current)->timer_interrupt();
}

void SimpleWorld::substract_energy(Egg* egg, Energy energy)
{
  if (egg->energy <= energy)
    throw EXCEPTION(BugDeath, boost::str(boost::format("\
Egg %1% is death")
                                         % egg->id()));

  egg->energy -= energy;
}

void SimpleWorld::substract_energy(Bug* bug, Energy energy)
{
  if (bug->energy <= energy)
    throw EXCEPTION(BugDeath, boost::str(boost::format("\
Bug %1% is death")
                                         % bug->id()));

  bug->energy -= energy;
}

void SimpleWorld::kill(Egg* egg)
{
  // Convert the egg in food
  Food* food = new Food(this, egg->die(this->env_->time));
  this->foods_.push_back(food);
  this->world_->remove(egg->position);
  this->world_->add(food, food->position);
#ifdef DEBUG
  std::cout << boost::format("\
Food[%1%] added at (%2%, %3%) with a size of %4%")
    % food->id()
    % food->position.x
    % food->position.y
    % food->size
            << std::endl;
#endif // DEBUG

  // Remove the dead bug
  this->eggs_.remove(egg);
  delete egg;

#ifdef DEBUG
  std::cout << boost::str(boost::format("\
Egg[%1%] died")
                          % egg->id())
            << std::endl;
#endif // DEBUG
}

void SimpleWorld::kill(Bug* bug)
{
  // Convert the bug in food
  Food* food = new Food(this, bug->die(this->env_->time));
  this->foods_.push_back(food);
  this->world_->remove(bug->position);
  this->world_->add(food, food->position);
#ifdef DEBUG
  std::cout << boost::format("\
Food[%1%] added at (%2%, %3%) with a size of %4%")
    % food->id()
    % food->position.x
    % food->position.y
    % food->size
            << std::endl;
#endif // DEBUG

  // Remove the dead bug
  this->bugs_.remove(bug);
  delete bug;

#ifdef DEBUG
  std::cout << boost::str(boost::format("\
Bug[%1%] died")
                          % bug->id())
            << std::endl;
#endif // DEBUG
}

void SimpleWorld::bugs_run()
{
  // execute a instruction in each bug
  std::list<Bug*> bugs = this->bugs_;
  std::list<Bug*>::iterator bug;
  for (bug = bugs.begin(); bug != bugs.end(); ++bug) {
    // check if the bug is alive, a bug could have killed it
    if (std::find(this->bugs_.begin(), this->bugs_.end(), *bug) ==
        this->bugs_.end())
      continue;

    try {
      // execute a instruction
      (*bug)->next();
    } catch (const BugDeath& e) {
      // the bug is death
      this->kill(*bug);
    }
  }
}

void SimpleWorld::update_db()
{
  // update the eggs
  std::list<Food*>::iterator food;
  for (food = this->foods_.begin();
       food != this->foods_.end();
       ++food)
    (*food)->update_db();

  // update the eggs
  std::list<Egg*>::iterator egg;
  for (egg = this->eggs_.begin();
       egg != this->eggs_.end();
       ++egg)
    (*egg)->update_db();

  // update the bugs
  std::list<Bug*>::iterator bug;
  for (bug = this->bugs_.begin();
       bug != this->bugs_.end();
       ++bug)
    (*bug)->update_db();
}

}
