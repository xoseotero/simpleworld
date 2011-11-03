/**
 * @file simpleworld/cpu/operations_move.cpp
 * Load/store/move/stack operations of the Simple CPU.
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

#include <simpleworld/config.hpp>

#include "types.hpp"
#include "word.hpp"
#include "operations.hpp"

namespace simpleworld
{
namespace cpu
{

/* Move operations */

/**
 * Move the content of a register to a register.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update move(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first, cpu.get_reg(inst.second));

  return UpdatePC;
}

/**
 * Swap the high half-word and the low half-word of a word.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update swap(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first,
              cpu.get_reg(inst.second) << 16 | cpu.get_reg(inst.second) >> 16);

  return UpdatePC;
}


/* Load operations */

/**
 * Load a word from memory.
 *
 * REGISTERS[FIRST] = MEMORY[PC + OFFSET]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update load(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first, cpu.get_mem(cpu.get_reg(REGISTER_PC) + inst.offset));

  return UpdatePC;
}

/**
 * Load a word from memory using two base registers.
 *
 * REGISTERS[FIRST] = MEMORY[REGISTERS[SECOND] + REGISTERS[DATA]]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loadrr(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first,
              cpu.get_mem(cpu.get_reg(inst.second) + cpu.get_reg(inst.data)));

  return UpdatePC;
}

/**
 * Load a word from memory using a base register and the data.
 *
 * REGISTERS[FIRST] = MEMORY[REGISTERS[SECOND] + OFFSET]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loadri(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first, cpu.get_mem(cpu.get_reg(inst.second) + inst.offset));

  return UpdatePC;
}

/**
 * Load a half word (16 bits) from memory.
 *
 * REGISTERS[FIRST] = MEMORY[PC + OFFSET]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loadh(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first,
              cpu.get_halfmem(cpu.get_reg(REGISTER_PC) + inst.offset));

  return UpdatePC;
}

/**
 * Load a half word (16 bits) from memory using two base registers.
 *
 * REGISTERS[FIRST] = MEMORY[REGISTERS[SECOND] + REGISTERS[DATA]]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loadhrr(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first,
              cpu.get_halfmem(cpu.get_reg(inst.second) +
                              cpu.get_reg(inst.data)));

  return UpdatePC;
}

/**
 * Load a half word (16 bits) from memory using a base register and a offset.
 *
 * REGISTERS[FIRST] = MEMORY[REGISTERS[SECOND] + OFFSET]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loadhri(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first,
              cpu.get_halfmem(cpu.get_reg(inst.second) + inst.offset));

  return UpdatePC;
}

/**
 * Load a quarter word (8 bits) from memory.
 *
 * REGISTERS[FIRST] = MEMORY[PC + OFFSET]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loadq(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first,
              cpu.get_quartermem(cpu.get_reg(REGISTER_PC) + inst.offset));

  return UpdatePC;
}

/**
 * Load a quarter word (8 bits) from memory using two base registers.
 *
 * REGISTERS[FIRST] = MEMORY[REGISTERS[SECOND] + REGISTERS[DATA]]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loadqrr(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first,
              cpu.get_quartermem(cpu.get_reg(inst.second) +
                                 cpu.get_reg(inst.data)));

  return UpdatePC;
}

/**
 * Load a quarter word (8 bits) from memory using a base register and a offset.
 *
 * REGISTERS[FIRST] = MEMORY[REGISTERS[SECOND] + OFFSET]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loadqri(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first,
              cpu.get_quartermem(cpu.get_reg(inst.second) + inst.offset));

  return UpdatePC;
}


/**
 * Load the data.
 * The higher 16 bits are set to 0.
 *
 * REGISTERS[FIRST] = DATA (the upper 32bits are cleared)
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loadi(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first, inst.data);

  return UpdatePC;
}

/**
 * Load the data into the higher 16 bits.
 * The lower 16 bits don't change.
 *
 * REGISTERS[FIRST] = DATA (the lower 16bits are perserved)
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loadhi(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first,
              ((cpu.get_reg(inst.first)) & 0x0000ffff) |
              (inst.data << 16));

  return UpdatePC;
}

/**
 * Load the absolute address from the offset.
 *
 * REGISTERS[FIRST] = PC + OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loada(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first, cpu.get_reg(REGISTER_PC) + inst.offset);

  return UpdatePC;
}


/* Store operations */

/**
 * Store a word to memory.
 *
 * MEMORY[PC + OFFSET] = REGISTERS[FIRST]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update store(CPU& cpu, Instruction inst)
{
  cpu.set_mem(cpu.get_reg(REGISTER_PC) + inst.offset, cpu.get_reg(inst.first));

  return UpdatePC;
}

/**
 * Store a word to memory using two base registers.
 *
 * MEMORY[REGISTERS[FIRST] + REGISTERS[DATA]] = REGISTERS[SECOND]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update storerr(CPU& cpu, Instruction inst)
{
  cpu.set_mem(cpu.get_reg(inst.first) + cpu.get_reg(inst.data),
              cpu.get_reg(inst.second));

  return UpdatePC;
}

/**
 * Store a word to memory using a base register and the data.
 *
 * MEMORY[REGISTERS[FIRST] + OFFSET] = REGISTERS[SECOND]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update storeri(CPU& cpu, Instruction inst)
{
  cpu.set_mem(cpu.get_reg(inst.first) + inst.offset, cpu.get_reg(inst.second));

  return UpdatePC;
}

/**
 * Store a half word (16 bits) to memory.
 *
 * MEMORY[PC + OFFSET] = REGISTERS[FIRST]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update storeh(CPU& cpu, Instruction inst)
{
  cpu.set_halfmem(cpu.get_reg(REGISTER_PC) + inst.offset,
                  static_cast<HalfWord>(cpu.get_reg(inst.first)));

  return UpdatePC;
}

/**
 * Store a half word (16 bits) to memory using two base registers.
 *
 * MEMORY[REGISTERS[FIRST] + REGISTERS[DATA]] = REGISTERS[SECOND]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update storehrr(CPU& cpu, Instruction inst)
{
  cpu.set_halfmem(cpu.get_reg(inst.first) + cpu.get_reg(inst.data),
                  static_cast<HalfWord>(cpu.get_reg(inst.second)));

  return UpdatePC;
}

/**
 * Store a half word (16 bits) to memory using a base register and a offset.
 *
 * MEMORY[REGISTERS[FIRST] + OFFSET] = REGISTERS[SECOND]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update storehri(CPU& cpu, Instruction inst)
{
  cpu.set_halfmem(cpu.get_reg(inst.first) + inst.offset,
                  static_cast<HalfWord>(cpu.get_reg(inst.second)));

  return UpdatePC;
}

/**
 * Store a quarter word (16 bits) to memory.
 *
 * MEMORY[PC + OFFSET] = REGISTERS[FIRST]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update storeq(CPU& cpu, Instruction inst)
{
  cpu.set_quartermem(cpu.get_reg(REGISTER_PC) + inst.offset,
                     static_cast<QuarterWord>(cpu.get_reg(inst.first)));

  return UpdatePC;
}

/**
 * Store a quarter word (16 bits) to memory using two base registers.
 *
 * MEMORY[REGISTERS[FIRST] + REGISTERS[DATA]] = REGISTERS[SECOND]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update storeqrr(CPU& cpu, Instruction inst)
{
  cpu.set_quartermem(cpu.get_reg(inst.first) + cpu.get_reg(inst.data),
                     static_cast<QuarterWord>(cpu.get_reg(inst.second)));

  return UpdatePC;
}

/**
 * Store a quarter word (16 bits) to memory using a base register and a offset.
 *
 * MEMORY[REGISTERS[FIRST] + OFFSET] = REGISTERS[SECOND]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update storeqri(CPU& cpu, Instruction inst)
{
  cpu.set_quartermem(cpu.get_reg(inst.first) + inst.offset,
                     static_cast<QuarterWord>(cpu.get_reg(inst.second)));

  return UpdatePC;
}


/* Stack operations */

/**
 * Move the content of the register to the top of the stack.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update push(CPU& cpu, Instruction inst)
{
  // Save the register in the top of the stack
  cpu.set_mem(cpu.get_reg(REGISTER_SP), cpu.get_reg(inst.first));
  // Update stack pointer
  cpu.set_reg(REGISTER_SP, cpu.get_reg(REGISTER_SP) + sizeof(Word));

  return UpdatePC;
}

/**
 * Move the top of the stack to a register.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update pop(CPU& cpu, Instruction inst)
{
  // Update stack pointer
  cpu.set_reg(REGISTER_SP, cpu.get_reg(REGISTER_SP) - sizeof(Word));
  // Restore the register
  cpu.set_reg(inst.first, cpu.get_mem(cpu.get_reg(REGISTER_SP)));

  return UpdatePC;
}

}
}
