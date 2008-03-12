/**
 * @file simpleworld/cpu/operations_branch.cpp
 * Branch operations of the Simple CPU.
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

#include "operations.hpp"

namespace SimpleWorld
{
namespace CPU
{

// Before executing each operation, it's checked if the address is valid.
// If the addres is out of range, a Invalid Memory location is raised
// giving the as data.


/**
 * Branch always.
 *
 * PC += OFFSET
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update b(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
         Instruction inst)
{
  // Check if the address is valid.
  Address address = regs[REGISTER_PC] + inst.offset;
  mem[address];

  regs.set_word(REGISTER_PC, address);

  return UpdateNone;
}

/**
 * Branch on equal.
 *
 * if REGISTERS[FIRST] == REGISTERS[SECOND] -> PC += OFFSET
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update beq(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst)
{
  // Check if the address is valid.
  Address address = regs[REGISTER_PC] + inst.offset;
  mem[address];

  if (regs[REGISTER(inst.first)] == regs[REGISTER(inst.second)]) {
    regs.set_word(REGISTER_PC, address);
    return UpdateNone;
  }

  return UpdatePC;
}

/**
 * Branch on not equal.
 *
 * if REGISTERS[FIRST] != REGISTERS[SECOND] -> PC += OFFSET
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bne(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst)
{
  // Check if the address is valid.
  Address address = regs[REGISTER_PC] + inst.offset;
  mem[address];

  if (regs[REGISTER(inst.first)] != regs[REGISTER(inst.second)]) {
    regs.set_word(REGISTER_PC, address);
    return UpdateNone;
  }

  return UpdatePC;
}

/**
 * Branch on less than (signed comparission).
 *
 * if REGISTERS[FIRST] < REGISTERS[SECOND] -> PC += OFFSET
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update blt(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst)
{
  // Check if the address is valid.
  Address address = regs[REGISTER_PC] + inst.offset;
  mem[address];

  if (static_cast<Sint32>(regs[REGISTER(inst.first)]) <
      static_cast<Sint32>(regs[REGISTER(inst.second)])) {
    regs.set_word(REGISTER_PC, address);
    return UpdateNone;
  }

  return UpdatePC;
}

/**
 * Branch on less than (unsigned comparission).
 *
 * if REGISTERS[FIRST] < REGISTERS[SECOND] -> PC += OFFSET
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bltu(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  // Check if the address is valid.
  Address address = regs[REGISTER_PC] + inst.offset;
  mem[address];

  if (regs[REGISTER(inst.first)] < regs[REGISTER(inst.second)]) {
    regs.set_word(REGISTER_PC, address);
    return UpdateNone;
  }

  return UpdatePC;
}

/**
 * Branch on great than (signed comparission).
 *
 * if REGISTERS[FIRST] > REGISTERS[SECOND] -> PC += OFFSET
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bgt(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst)
{
  // Check if the address is valid.
  Address address = regs[REGISTER_PC] + inst.offset;
  mem[address];

  if (static_cast<Sint32>(regs[REGISTER(inst.first)]) >
      static_cast<Sint32>(regs[REGISTER(inst.second)])) {
    regs.set_word(REGISTER_PC, address);
    return UpdateNone;
  }

  return UpdatePC;
}

/**
 * Branch on great than (unsigned comparission).
 *
 * if REGISTERS[FIRST] > REGISTERS[SECOND] -> PC += OFFSET
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bgtu(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  // Check if the address is valid.
  Address address = regs[REGISTER_PC] + inst.offset;
  mem[address];

  if (regs[REGISTER(inst.first)] > regs[REGISTER(inst.second)]) {
    regs.set_word(REGISTER_PC, address);
    return UpdateNone;
  }

  return UpdatePC;
}

/**
 * Branch on less or equal (signed comparission).
 *
 * if REGISTERS[FIRST] <= REGISTERS[SECOND] -> PC += OFFSET
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update ble(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst)
{
  // Check if the address is valid.
  Address address = regs[REGISTER_PC] + inst.offset;
  mem[address];

  if (static_cast<Sint32>(regs[REGISTER(inst.first)]) <=
      static_cast<Sint32>(regs[REGISTER(inst.second)])) {
    regs.set_word(REGISTER_PC, address);
    return UpdateNone;
  }

  return UpdatePC;
}

/**
 * Branch on less or equal (unsigned comparission).
 *
 * if REGISTERS[FIRST] <= REGISTERS[SECOND] -> PC += OFFSET
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bleu(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  // Check if the address is valid.
  Address address = regs[REGISTER_PC] + inst.offset;
  mem[address];

  if (regs[REGISTER(inst.first)] <= regs[REGISTER(inst.second)]) {
    regs.set_word(REGISTER_PC, address);
    return UpdateNone;
  }

  return UpdatePC;
}

/**
 * Branch on greater or equal (signed comparission).
 *
 * if REGISTERS[FIRST] >= REGISTERS[SECOND] -> PC += OFFSET
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bge(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst)
{
  // Check if the address is valid.
  Address address = regs[REGISTER_PC] + inst.offset;
  mem[address];

  if (static_cast<Sint32>(regs[REGISTER(inst.first)]) >=
      static_cast<Sint32>(regs[REGISTER(inst.second)])) {
    regs.set_word(REGISTER_PC, address);
    return UpdateNone;
  }

  return UpdatePC;
}

/**
 * Branch on greater or equal (unsigned comparission).
 *
 * if REGISTERS[FIRST] >= REGISTERS[SECOND] -> PC += OFFSET
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bgeu(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  // Check if the address is valid.
  Address address = regs[REGISTER_PC] + inst.offset;
  mem[address];

  if (regs[REGISTER(inst.first)] >= regs[REGISTER(inst.second)]) {
    regs.set_word(REGISTER_PC, address);
    return UpdateNone;
  }

  return UpdatePC;
}

}
}
