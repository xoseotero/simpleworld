/**
 * @file simpleworld/cpu/operations_function.cpp
 * Function operations of the Simple CPU.
 *
 * begin:     Sat, 11 Nov 2006 19:15:19 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2006-2008  Xosé Otero <xoseotero@users.sourceforge.net>
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

#include "types.hpp"
#include "operations.hpp"

namespace SimpleWorld
{
namespace CPU
{

/**
 * Call a function.
 *
 * PUSH(PC) and PC += OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update call(CPU& cpu, Instruction inst)
{
  // Check if the address is valid.
  // If the addres is out of range, a Invalid Memory location is raised
  // giving this instruction as data.
  Address address = cpu.get_reg(REGISTER_PC) + inst.offset;
  cpu.get_mem(address);

  // Save the program counter (pc) in the top of the stack
  cpu.set_mem(cpu.get_reg(REGISTER_SP), cpu.get_reg(REGISTER_PC));
  // Update stack pointer
  cpu.set_reg(REGISTER_SP, cpu.get_reg(REGISTER_SP) + 4);
  // Execute the function
  cpu.set_reg(REGISTER_PC, address);

  return UpdateNone;
}

/**
 * Software interrupt.
 *
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update interrupt(CPU& cpu, Instruction inst)
{
  cpu.interrupt(INTERRUPT_SOFTWARE, inst.data);

  return UpdateInterrupt;
}

/**
 * Return.
 *
 * POP(PC)
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update ret(CPU& cpu, Instruction inst)
{
  // Update stack pointer
  cpu.set_reg(REGISTER_SP, cpu.get_reg(REGISTER_SP) - 4);
  // Restore the program counter
  cpu.set_reg(REGISTER_PC, cpu.get_mem(cpu.get_reg(REGISTER_SP)));

  return UpdatePC;
}

/**
 * Return from exception.
 *
 * POP(ALL REGISTERS)
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update reti(CPU& cpu, Instruction inst)
{
  // Restore all the registers
  Sint8 i;
  for (i = 15; i >= 0; i--) {
    // Update stack pointer
    cpu.set_reg(REGISTER_SP, cpu.get_reg(REGISTER_SP) - 4);
    // Restore the register
    cpu.set_reg(i, cpu.get_mem(cpu.get_reg(REGISTER_SP)));
  }

  // It's not needed to update the cs register because the correct values
  // were stored in the stack

  // It's not needed to update the pc because it already udpated by
  // CPU::interrupt()
  return UpdateNone;
}

}
}
