/**
 * @file simpleworld/cpu.cpp
 * A CPU in Simple World.
 *
 *  Copyright (C) 2007-2013  Xosé Otero <xoseotero@gmail.com>
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

#include <simpleworld/cpu/instruction.hpp>

#include "types.hpp"
#include "isa.hpp"
#include "cpu.hpp"
#include "bug.hpp"
#include "actionerror.hpp"

namespace simpleworld
{

/**
 * Constructor.
 * @param isa instruction set architecture of the CPU.
 * @param registers registers of the CPU.
 * @param memory memory of the CPU.
 * @param bug bug owner of the CPU.
 */
CPU::CPU(const cpu::ISA& isa, cpu::Memory* registers, cpu::Memory* memory,
	 Bug* bug)
  : cpu::CPU(isa, registers, memory), bug(bug)
{
}


/**
* Execute the next instruction.
* @exception CPUException A stop instruction was found
*/
void CPU::next()
{
  try {
    cpu::CPU::next();
  } catch (const ActionError& e) {
    // invalid world command
    cpu::Address addr = this->get_reg(REGISTER_PC);
    this->interrupt(INTERRUPT_WORLDACTION, addr,
                    cpu::Instruction(this->get_mem(addr, false)).data);
  }
}

/**
 * Throw a interrupt.
 * @param code the code of the interrupt.
 * @param g1 the word stored in g1.
 * @param g2 the word stored in g2.
 * @exception MemoryError if the ip is not valid.
 */
void CPU::interrupt(Uint8 code, cpu::Word g1, cpu::Word g2)
{
  // interrupt the current action
  if (this->interrupt_enabled(code) and not this->bug->is_null("action_time")) {
    this->bug->set_null("action_time");
    this->set_reg(REGISTER_G0, static_cast<cpu::Word>(ActionInterrupted));

    return;
  }

  cpu::CPU::interrupt(code, g1, g2);
}

}
