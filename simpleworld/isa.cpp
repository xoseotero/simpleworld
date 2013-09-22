/**
 * @file simpleworld/instruction.cpp
 * Instruction set architecture.
 *
 *  Copyright (C) 2013  Xosé Otero <xoseotero@gmail.com>
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

#include "operations.hpp"
#include "isa.hpp"

namespace simpleworld
{

/**
 * Constructor.
 */
ISA::ISA()
  : cpu::ISA()
{
  this->add_interrupt(INTERRUPT_WORLDACTION, "InvalidWorldCommand", true);
  this->add_interrupt(INTERRUPT_WORLDEVENT, "WorldEvent", false);

  this->add_instruction(0x58, "world", 0, true, ::simpleworld::world);
}


/**
 * Global variable with the ISA.
 */
const ISA isa;

}
