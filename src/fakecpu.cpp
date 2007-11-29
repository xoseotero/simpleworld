/**
 * @file src/fakecpu.cpp
 * Fake CPU that add a "world" operation that does nothing.
 *
 * begin:     Sun, 04 Nov 2007 20:26:14 +0100
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

#include "fakecpu.hpp"

#include <simpleworld/types.hpp>
#include <simpleworld/cpu/memory.hpp>
#include <simpleworld/cpu/interrupt.hpp>
#include <simpleworld/cpu/instruction.hpp>
#include <simpleworld/cpu/isa.hpp>

#define REGISTER(NUMBER) (NUMBER * sizeof(cpu::Word))
#define REGISTER_PC REGISTER(0xd)
#define REGISTER_STP REGISTER(0xe)
#define REGISTER_ITP REGISTER(0xf)

/**
 * Fake World operation that does nothing.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
cpu::Update fakeworld(cpu::ISA& isa, cpu::Memory& regs, cpu::Memory& mem,
                      cpu::Interrupt& interrupt, cpu::Instruction inst)
{
  // The operation can be done always.
  // The actual values of the registers are used to return the false values of
  // the operation.
  regs.set_word(REGISTER(0), static_cast<cpu::Word>(sw::ActionSuccess));

  return cpu::UpdatePC;
}


FakeCPU::FakeCPU(cpu::Memory* registers, cpu::Memory* memory)
  : cpu::CPU(registers, memory)
{
  this->isa_.add_instruction(0x38, "world", 0, true, fakeworld);
}
