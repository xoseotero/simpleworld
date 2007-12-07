/**
 * @file simpleworld/bug.cpp
 * A bug in Simple World.
 *
 * begin:     Sun, 15 Jul 2007 07:51:44 +0200
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

#include <cassert>

#include <simpleworld/simpleworld.hpp>
#include <simpleworld/operations.hpp>
#include "bug.hpp"

namespace SimpleWorld
{

Bug::Bug(SimpleWorld* sw, DB::ID id)
  : DB::Bug(sw, id), CPU(&this->cpu.registers, &this->code.code), world(sw)
{
  Bug::bugs[this->registers_] = this;

  this->isa_.add_interrupt(0x5, "InvalidWorldCommand");
  this->isa_.add_interrupt(0x6, "WorldEvent");

  this->isa_.add_instruction(0x38, "world", 0, true, ::SimpleWorld::world);
}

Bug::~Bug()
{
  assert(Bug::bugs.find(this->registers_) != Bug::bugs.end());

  Bug::bugs.erase(this->registers_);
}


void Bug::attacked()
{
  // do something
}


std::map< ::SimpleWorld::CPU::Memory*, Bug*> Bug::bugs;

}
