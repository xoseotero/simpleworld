/**
 * @file simpleworld/cpu/operations_function.cpp
 * Function operations of the Simple CPU.
 *
 * begin:     Sat, 11 Nov 2006 19:15:19 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2006-2007  Xosé Otero <xoseotero@users.sourceforge.net>
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
#include "interrupt.hpp"

namespace SimpleWorld
{
namespace CPU
{

/**
 * Call a function.
 *
 * PUSH(PC) and PC += OFFSET
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update call(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  // Check if the address is valid.
  // If the addres is out of range, a Invalid Memory location is raised
  // giving this instruction as data.
  Address address = regs[REGISTER_PC] + inst.offset;
  mem[address];

  // Save the program counter (pc) in the top of the stack
  mem.set_word(regs[REGISTER_STP], regs[REGISTER_PC]);
  // Update stack pointer
  regs.set_word(REGISTER_STP, regs[REGISTER_STP] + 4);
  // Execute the function
  regs.set_word(REGISTER_PC, address);

  return UpdateNone;
}

/**
 * Software interrupt.
 *
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update interrupt(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
                 Instruction inst)
{
  Word code = static_cast<Word>(isa.interrupt_code("SoftwareInterrupt"));
  interrupt.code = code;
  interrupt.r0 = code;
  interrupt.r1 = inst.data;

  return UpdateInterrupt;
}

/**
 * Return.
 *
 * POP(PC)
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update ret(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst)
{
  // Update stack pointer
  regs.set_word(REGISTER_STP, regs[REGISTER_STP] - 4);
  // Restore the program counter
  regs.set_word(REGISTER_PC, mem[regs[REGISTER_STP]]);

  return UpdatePC;
}

/**
 * Return from exception.
 *
 * POP(ALL REGISTERS)
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update reti(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  Sint8 i;
  for (i = 15; i >= 0; i--) {
    // Restore a register:
    // Update stack pointer
    regs.set_word(REGISTER_STP, regs[REGISTER_STP] - 4);
    // Restore the register
    regs.set_word(REGISTER(i), mem[regs[REGISTER_STP]]);
  }

  return UpdatePC;
}

}
}
