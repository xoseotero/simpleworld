/**
 * @file simpleworld/bug.cpp
 * A bug in Simple World.
 *
 * begin:     Sun, 15 Jul 2007 07:51:44 +0200
 * last:      $Date$
 *
 *  Copyright (C) 2007  Xosé Otero <xoseotero@gmail.com>
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

#include "config.hpp"

#ifdef DEBUG
#include <iostream>

#include <boost/format.hpp>
#endif // DEBUG

#include "simpleworld.hpp"
#include "operations.hpp"
#include "types.hpp"
#include "bug.hpp"

namespace simpleworld
{

/**
 * Constructor.
 * @param sw world where the bug lives.
 * @param id id of the bug.
 * @exception DBException if there is a error in the database.
 */
Bug::Bug(SimpleWorld* sw, db::ID id)
  : db::Bug(sw, id), CPU(&this->cpu.registers, &this->code.code), world(sw)
{
  this->isa_.add_interrupt(INTERRUPT_WORLDACTION, "InvalidWorldCommand", true);
  this->isa_.add_interrupt(INTERRUPT_WORLDEVENT, "WorldEvent", false);

  this->isa_.add_instruction(0x38, "world", 0, true, ::simpleworld::world);
}


/**
 * The bug has been attacked.
 */
void Bug::attacked()
{
#ifdef DEBUG
  std::cout << boost::str(boost::format("\
Bug[%1%] attacked")
                         % this->id_)
    << std::endl;
#endif // DEBUG

  this->interrupt(INTERRUPT_WORLDEVENT, EventAttack);
}

}
