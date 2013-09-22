/**
 * @file src/common/fakeisa.cpp
 * Fake ISA that adds a "world" operation that does nothing.
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

#include "fakeisa.hpp"

#include <simpleworld/types.hpp>
#include <simpleworld/cpu/instruction.hpp>
#include <simpleworld/cpu/cpu.hpp>

/**
 * Fake World operation that does nothing.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
cpu::Update fakeworld(cpu::CPU& cpu, cpu::Instruction inst)
{
  // The operation can be done always.
  // The actual values of the registers are used to return the false values of
  // the operation.
  cpu.set_reg(REGISTER_G0, static_cast<cpu::Word>(sw::ActionSuccess));

  return cpu::UpdatePC;
}


/**
 * Constructor.
 */
FakeISA::FakeISA()
  : cpu::ISA()
{
  this->add_interrupt(5, "InvalidWorldCommand", true);
  this->add_interrupt(6, "WorldEvent", false);

  this->add_instruction(0x58, "world", 0, true, fakeworld);
}


/**
 * Global variable with the fake ISA.
 */
const FakeISA fakeisa;
