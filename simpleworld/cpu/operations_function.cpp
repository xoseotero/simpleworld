/**
 * @file simpleworld/cpu/operations_function.cpp
 * Function operations of the Simple CPU.
 *
 *  Copyright (C) 2006-2011  Xosé Otero <xoseotero@gmail.com>
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

namespace simpleworld
{
namespace cpu
{

/**
 * Push the local registers in the stack.
 *
 * @param cpu the CPU.
 */
static inline void push_locals(CPU& cpu)
{
  Word sp = cpu.get_reg(REGISTER_SP);

  for (Sint8 i = GLOBAL_REGISTERS;
       i < GLOBAL_REGISTERS + REGISTERS_PER_WINDOW;
       i++) {
    // Save the register in the top of the stack
    cpu.set_mem(sp, cpu.get_reg(i));
    sp += sizeof(Word);
  }

  // Update stack pointer
  cpu.set_reg(REGISTER_SP, sp);
}

/**
 * Pop the local registers from the stack.
 *
 * @param cpu the CPU.
 */
static inline void pop_locals(CPU& cpu)
{
  Word sp = cpu.get_reg(REGISTER_SP);

  for (Sint8 i = GLOBAL_REGISTERS + REGISTERS_PER_WINDOW - 1;
       i >= GLOBAL_REGISTERS;
       i--) {
    sp -= sizeof(Word);
    // Restore the register
    cpu.set_reg(i, cpu.get_mem(sp));
  }

  // Update stack pointer
  cpu.set_reg(REGISTER_SP, sp);
}

/**
 * Pop the global registers from the stack.
 *
 * @param cpu the CPU.
 */
static inline void pop_globals(CPU& cpu)
{
  Word sp = cpu.get_reg(REGISTER_SP);

  // Restore the registers
  sp -= sizeof(Word);
  cpu.set_reg(REGISTER_G3, cpu.get_mem(sp));
  sp -= sizeof(Word);
  cpu.set_reg(REGISTER_G2, cpu.get_mem(sp));
  sp -= sizeof(Word);
  cpu.set_reg(REGISTER_G1, cpu.get_mem(sp));
  sp -= sizeof(Word);
  cpu.set_reg(REGISTER_G0, cpu.get_mem(sp));
  sp -= sizeof(Word);
  cpu.set_reg(REGISTER_IP, cpu.get_mem(sp));

  // Update stack pointer
  cpu.set_reg(REGISTER_SP, sp);
}

/**
 * Increase the register window.
 *
 * @param cpu the CPU.
 */
static inline void increase_window(CPU& cpu)
{
  Word wc = cpu.get_reg(REGISTER_WC);
  if (wc >= (REGISTER_WINDOWS - 1))
    push_locals(cpu);
  cpu.set_reg(REGISTER_WC, wc + 1);
}

/**
 * Decrease the register window.
 *
 * @param cpu the CPU.
 */
static inline void decrease_window(CPU& cpu)
{
  Word wc = cpu.get_reg(REGISTER_WC) - 1;
  cpu.set_reg(REGISTER_WC, wc);
  if (wc >= (REGISTER_WINDOWS - 1))
    pop_locals(cpu);
}


/**
 * Call a function.
 *
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

  // Update the link register and the frame pointer
  cpu.set_reg(REGISTER_LR, cpu.get_reg(REGISTER_PC));
  cpu.set_reg(REGISTER_FP, cpu.get_reg(REGISTER_SP));

  // Execute the function
  cpu.set_reg(REGISTER_PC, address);

  return UpdateNone;
}

/**
 * Call a function using a register as address.
 *
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

  // Update the link register and the frame pointer
  cpu.set_reg(REGISTER_LR, cpu.get_reg(REGISTER_PC));
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
  // already does it if the interrupts are enabled
  cpu.interrupt(INTERRUPT_SOFTWARE, inst.data);

  return UpdateInterrupt;
}

/**
 * Return.
 *
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update ret(CPU& cpu, Instruction inst)
{
  // Restore the program counter and the stack pointer
  cpu.set_reg(REGISTER_PC, cpu.get_reg(REGISTER_LR));
  cpu.set_reg(REGISTER_SP, cpu.get_reg(REGISTER_FP));

  decrease_window(cpu);

  return UpdatePC;
}

/**
 * Return from exception.
 *
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update reti(CPU& cpu, Instruction inst)
{
  // Restore the program counter and the stack pointer
  cpu.set_reg(REGISTER_PC, cpu.get_reg(REGISTER_LR));
  cpu.set_reg(REGISTER_SP, cpu.get_reg(REGISTER_FP));

  decrease_window(cpu);

  // Restore the global registers
  pop_globals(cpu);

  // It's not needed to update the pc because it already udpated by
  // CPU::interrupt()
  return UpdateNone;
}

}
}
