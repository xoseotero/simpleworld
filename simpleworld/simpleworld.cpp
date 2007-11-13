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
#include <boost/format.hpp>
#endif // DEBUG

#include <vector>
#include <cassert>

#include <sqlite3x.hpp>

#include <simpleworld/db/food.hpp>
#include <simpleworld/db/egg.hpp>
#include <simpleworld/db/bug.hpp>
#include <simpleworld/db/cpu.hpp>
#include <simpleworld/db/code.hpp>

#include "movement.hpp"
#include "simpleworld.hpp"

// Default values for the environment
#define DEFAULT_SIZE (Position) {16, 16}
#define DEFAULT_ENERGY_DEVELOPED 64
#define DEFAULT_MUTATIONS_PERCENT 0.001
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
  // this can throw a InvalidPosition
  if (this->world_->used(position))
    throw UsedPosition(__FILE__, __LINE__, position);


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
  // this can throw a InvalidPosition
  if (this->world_->used(position))
    throw UsedPosition(__FILE__, __LINE__, position);


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

    // @todo send the timer event
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
  std::cerr << boost::format("Bug %1% doing action nothing")
    % bug->id()
    << std::endl;
#endif // DEBUG

  bug->energy -= this->env_->energy_nothing;
  bug->changed = true;
}

CPU::Word SimpleWorld::myself(Bug* bug, Info info, CPU::Word* ypos)
{
#ifdef DEBUG
  std::cerr << boost::format("Bug %1% doing action myself")
    % bug->id()
    << std::endl;
#endif // DEBUG

  bug->energy -= this->env_->energy_myself;
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
    throw ActionException(__FILE__, __LINE__, "Unknown type of information");
  }
}

ElementType SimpleWorld::detect(Bug* bug)
{
#ifdef DEBUG
  std::cerr << boost::format("Bug %1% doing action detect")
    % bug->id()
    << std::endl;
#endif // DEBUG

  bug->energy -= this->env_->energy_detect;
  bug->changed = true;

  return bug->type;
}

CPU::Word SimpleWorld::information(Bug* bug, Info info, CPU::Word* ypos)
{
#ifdef DEBUG
  std::cerr << boost::format("Bug %1% doing action information")
    % bug->id()
    << std::endl;
#endif // DEBUG

  Element* target = this->world_->get(this->front(bug));
  assert(target != NULL and target->type != ElementNothing);

  switch (info) {
  case InfoID:  // Only eggs and bugs
    if (target->type == ElementFood)
      throw ActionException(__FILE__, __LINE__, "The target is food");

    return static_cast<CPU::Word>(static_cast< ::SimpleWorld::DB::BugElement* >(target)->id());

  case InfoSize: // Every element
    if (target->type == ElementFood)
      return static_cast<CPU::Word>(static_cast<Food*>(target)->size);
    else
      return static_cast<CPU::Word>(static_cast< ::SimpleWorld::DB::BugElement* >(target)->code.size);

  case InfoEnergy: // Only eggs and bugs
    if (target->type == ElementFood)
      throw ActionException(__FILE__, __LINE__, "The target is food");

    return static_cast<CPU::Word>(static_cast< ::SimpleWorld::DB::AliveBug* >(target)->energy);

  case InfoPosition: // Every element
    *ypos = static_cast<CPU::Word>(target->position.y);
    return static_cast<CPU::Word>(target->position.x);

  case InfoOrientation: // Only eggs and bugs
    if (target->type == ElementFood)
      throw ActionException(__FILE__, __LINE__, "The target is food");

    return static_cast<CPU::Word>(static_cast< ::SimpleWorld::DB::BugElement* >(target)->orientation);

  default:
    throw ActionException(__FILE__, __LINE__, "Unknown type of information");
  }
}

void SimpleWorld::move(Bug* bug, Movement movement)
{
#ifdef DEBUG
  std::cerr << boost::format("Bug %1% doing action move")
    % bug->id()
    << std::endl;
#endif // DEBUG

  this->world_->move(bug->position, this->front(bug));
  bug->changed = true;
}

void SimpleWorld::turn(Bug* bug, Turn turn)
{
#ifdef DEBUG
  std::cerr << boost::format("Bug %1% doing action turn")
    % bug->id()
    << std::endl;
#endif // DEBUG

  assert(turn == TurnLeft or turn == TurnRight);

  bug->energy -= this->env_->energy_turn;
  bug->orientation = ::SimpleWorld::turn(bug->orientation, turn);
  bug->changed = true;
}

/**
 * Attack the bug that is in front of the bug.
 * @param bug Bug that executes the action.
 * @param energy Energy used in the attack.
 */
void SimpleWorld::attack(Bug* bug, Energy energy)
{
#ifdef DEBUG
  std::cerr << boost::format("Bug %1% doing action attack")
    % bug->id()
    << std::endl;
#endif // DEBUG

  // TODO: Do something :)
  //bug->energy -= this->env_->energy_attack;
  //bug->changed = true;
}

/**
 * Eat what is in front of the bug.
 * @param bug Bug that executes the action.
 * @return The energy eaten.
 */
Energy SimpleWorld::eat(Bug* bug)
{
#ifdef DEBUG
  std::cerr << boost::format("Bug %1% doing action eat")
    % bug->id()
    << std::endl;
#endif // DEBUG

  // TODO: Do something :)
  //bug->energy -= this->env_->energy_eat;
  //bug->changed = true;

  return 0;
}

/**
 * Create a egg in front of the bug.
 * @param bug Bug that executes the action.
 * @param energy Energy transfered to the egg.
 */
void SimpleWorld::egg(Bug* bug, Energy energy)
{
#ifdef DEBUG
  std::cerr << boost::format("Bug %1% doing action egg")
    % bug->id()
    << std::endl;
#endif // DEBUG

  // TODO: Do something :)
  //bug->energy -= this->env_->energy_egg;
  //bug->changed = true;
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
    this->env_->mutations_probability = DEFAULT_MUTATIONS_PERCENT;
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
  std::list<Egg*>::iterator egg;
  std::vector<Egg*> to_remove;
  for (egg = this->eggs_.begin();
       egg != this->eggs_.end();
       ++egg) {
    if ((*egg)->birth <= this->env_->time) {
      // convert the egg to a bug
      Bug* bug = new Bug(this, (*egg));
      bug->update_db(true);

      this->bugs_.push_back(bug);
      to_remove.push_back(*egg);
    }
  }

  std::vector<Egg*>::iterator remove;
  for (remove = to_remove.begin();
       remove != to_remove.end();
       ++remove)
    this->eggs_.remove(*remove);

}

void SimpleWorld::bugs_run()
{
  // execute a instruction in each bug
  std::list<Bug*>::iterator current;
  for (current = this->bugs_.begin();
       current != this->bugs_.end();
       ++current)
    // execute a instruction
    (*current)->next();
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
