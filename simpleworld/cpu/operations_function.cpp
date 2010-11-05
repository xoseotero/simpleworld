/**
 * @file simpleworld/cpu/operations_function.cpp
 * Function operations of the Simple CPU.
 *
 *  Copyright (C) 2006-2010  Xosé Otero <xoseotero@gmail.com>
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
#include "cs.hpp"

namespace simpleworld
{
namespace cpu
{

/**
 * Increase the register window.
 *
 * @param cpu the CPU.
 */
static void increase_window(CPU& cpu)
{
  CS cs(cpu.get_reg(REGISTER_CS, false));
  cs.cw++;
  cpu.set_reg(REGISTER_CS, cs.encode(), false);
}

/**
 * Decrease the register window.
 *
 * @param cpu the CPU.
 */
static void decrease_window(CPU& cpu)
{
  CS cs(cpu.get_reg(REGISTER_CS, false));
  cs.cw--;
  cpu.set_reg(REGISTER_CS, cs.encode(), false);
}


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
  // If the address is out of range, a Invalid Memory location is raised
  // giving this instruction as data.
  Address address = cpu.get_reg(REGISTER_PC) + inst.offset;
  cpu.get_mem(address);

  increase_window(cpu);

  // Save the frame pointer and the program counter in the top of the stack
  cpu.set_mem(cpu.get_reg(REGISTER_SP), cpu.get_reg(REGISTER_FP));
  cpu.set_mem(cpu.get_reg(REGISTER_SP) + sizeof(Word),
              cpu.get_reg(REGISTER_PC));
  // Update the stack pointer and the frame pointer
  cpu.set_reg(REGISTER_SP, cpu.get_reg(REGISTER_SP) + 8);
  cpu.set_reg(REGISTER_FP, cpu.get_reg(REGISTER_SP));
  // Execute the function
  cpu.set_reg(REGISTER_PC, address);

  return UpdateNone;
}

/**
 * Call a function using a register as address.
 *
 * PUSH(PC) and PC = REGISTER[FIRST]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update callr(CPU& cpu, Instruction inst)
{
  // Check if the address is valid.
  // If the address is out of range, a Invalid Memory location is raised
  // giving this instruction as data.
  Address address = cpu.get_reg(inst.first);
  cpu.get_mem(address);

  increase_window(cpu);

  // Save the frame pointer and the program counter in the top of the stack
  cpu.set_mem(cpu.get_reg(REGISTER_SP), cpu.get_reg(REGISTER_FP));
  cpu.set_mem(cpu.get_reg(REGISTER_SP) + sizeof(Word),
              cpu.get_reg(REGISTER_PC));
  // Update the stack pointer and the frame pointer
  cpu.set_reg(REGISTER_SP, cpu.get_reg(REGISTER_SP) + 8);
  cpu.set_reg(REGISTER_FP, cpu.get_reg(REGISTER_SP));
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
  // It's not needed to increase the register window because cpu.interrupt()
  // already does it if the interrupts are enabled'
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
  decrease_window(cpu);

  // Update stack pointer
  cpu.set_reg(REGISTER_SP, cpu.get_reg(REGISTER_FP) - 8);
  // Restore the program counter and the frame pointer
  cpu.set_reg(REGISTER_PC,
              cpu.get_mem(cpu.get_reg(REGISTER_SP) + sizeof(Word)));
  cpu.set_reg(REGISTER_FP, cpu.get_mem(cpu.get_reg(REGISTER_SP)));

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
  decrease_window(cpu);

  // Update stack pointer
  cpu.set_reg(REGISTER_SP, cpu.get_reg(REGISTER_FP));

  // Restore all the global registers
  for (Sint8 i = (GLOBAL_REGISTERS - 1); i >= 0; i--) {
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
