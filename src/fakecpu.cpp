/**
 * @file src/fakecpu.cpp
 * Fake CPU that add a "world" operation that does nothing.
 *
 *  Copyright (C) 2007-2008  Xosé Otero <xoseotero@gmail.com>
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
#include <simpleworld/cpu/instruction.hpp>

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
  cpu.set_reg(0, static_cast<cpu::Word>(sw::ActionSuccess));

  return cpu::UpdatePC;
}


FakeCPU::FakeCPU(cpu::Memory* registers, cpu::Memory* memory)
  : cpu::CPU(registers, memory)
{
  this->isa_.add_instruction(0x58, "world", 0, true, fakeworld);
}
