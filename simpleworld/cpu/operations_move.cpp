/**
 * @file simpleworld/cpu/operations_move.cpp
 * Load/store/move/stack operations of the Simple CPU.
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

#include <simpleworld/ints.hpp>
#include "operations.hpp"

namespace SimpleWorld
{
namespace CPU
{

/* Move operations */

/**
 * Move the content of a register to a register.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update move(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  regs.set_word(REGISTER(inst.first), regs[REGISTER(inst.second)]);

  return UpdatePC;
}

/**
 * Swap the high half-word and the low half-word of a word.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update swap(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] << 16 |
                regs[REGISTER(inst.second)] >> 16);

  return UpdatePC;
}


/* Load operations */

/**
 * Load a word from memory.
 *
 * REGISTERS[FIRST] = MEMORY[PC + OFFSET]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update load(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  regs.set_word(REGISTER(inst.first), mem[regs[REGISTER_PC] + inst.offset]);

  return UpdatePC;
}

/**
 * Load the data.
 * The higher 16 bits are set to 0.
 *
 * REGISTERS[FIRST] = DATA (the upper 32bits are cleared)
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loadi(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
             Instruction inst)
{
  regs.set_word(REGISTER(inst.first), inst.data);

  return UpdatePC;
}

/**
 * Load the absolute address from the offset.
 *
 * REGISTERS[FIRST] = PC + OFFSET
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loada(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
             Instruction inst)
{
  regs.set_word(REGISTER(inst.first), regs[REGISTER_PC] + inst.offset);

  return UpdatePC;
}

/**
 * Load a word from memory using two base registers.
 *
 * REGISTERS[FIRST] = MEMORY[REGISTERS[SECOND] + REGISTERS[DATA]]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loadrr(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
              Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                mem[regs[REGISTER(inst.second)] +
                    regs[REGISTER(inst.data)]]);

  return UpdatePC;
}

/**
 * Load a word from memory using a base register and the data.
 *
 * REGISTERS[FIRST] = MEMORY[REGISTERS[SECOND] + OFFSET]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loadri(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
              Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                mem[regs[REGISTER(inst.second)] + inst.offset]);

  return UpdatePC;
}


/* Store operations */

/**
 * Store a word to memory.
 *
 * MEMORY[PC + OFFSET] = REGISTERS[FIRST]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update store(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
             Instruction inst)
{
  mem.set_word(regs[REGISTER_PC] + inst.offset, regs[REGISTER(inst.first)]);

  return UpdatePC;
}

/**
 * Store a word to memory using two base registers.
 *
 * MEMORY[REGISTERS[FIRST] + REGISTERS[DATA]] = REGISTERS[SECOND]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update storerr(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
               Instruction inst)
{
  mem.set_word(regs[REGISTER(inst.second)] + regs[REGISTER(inst.data)],
               regs[REGISTER(inst.first)]);

  return UpdatePC;
}

/**
 * Store a word to memory using a base register and the data.
 *
 * MEMORY[REGISTERS[FIRST] + OFFSET] = REGISTERS[SECOND]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update storeri(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
               Instruction inst)
{
  mem.set_word(regs[REGISTER(inst.second)] + inst.offset,
               regs[REGISTER(inst.first)]);

  return UpdatePC;
}


/* Stack operations */

/**
 * Move the content of the register to the top of the stack.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update push(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  // Save the register in the top of the stack
  mem.set_word(regs[REGISTER_STP], regs[REGISTER(inst.first)]);
  // Update stack pointer
  regs.set_word(REGISTER_STP, regs[REGISTER_STP] + 4);

  return UpdatePC;
}

/**
 * Move the top of the stack to a register.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update pop(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst)
{
  // Update stack pointer
  regs.set_word(REGISTER_STP, regs[REGISTER_STP] - 4);
  // Restore the register
  regs.set_word(REGISTER(inst.first), mem[regs[REGISTER_STP]]);

  return UpdatePC;
}

}
}
