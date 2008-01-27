/**
 * @file simpleworld/cpu/operations_shift.cpp
 * Shift operations of the Simple CPU.
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

/**
 * Logic shift left.
 *
 * Move the bits X positions to the left inserting zeros on the right.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update sll(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] << regs[REGISTER(inst.address)]);

  return UpdatePC;
}

/**
 * Logic shift left.
 *
 * Move the bits X positions to the left inserting zeros on the right.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update slli(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] << inst.address);

  return UpdatePC;
}

/**
 * Logic shift right.
 *
 * Move the bits X positions to the right inserting zeros on the left.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update srl(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] >> regs[REGISTER(inst.address)]);

  return UpdatePC;
}

/**
 * Logic shift right.
 *
 * Move the bits X positions to the right inserting zeros on the left.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update srli(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] >> inst.address);

  return UpdatePC;
}

/**
 * Arithmetic shift left.
 *
 * Same as logic shift left.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update sla(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] << regs[REGISTER(inst.address)]);

  return UpdatePC;
}

/**
 * Arithmetic shift left.
 *
 * Same as logic shift left.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update slai(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] << inst.address);

  return UpdatePC;
}

/**
 * Arithmetic shift right.
 *
 * Same as logic shift right but performing sign extension.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update sra(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst)
{
  Uint32 sign = regs[REGISTER(inst.second)] & 0x80000000;
  regs.set_word(REGISTER(inst.first),
                (regs[REGISTER(inst.second)] >>
                 regs[REGISTER(inst.address)]) | sign);

  return UpdatePC;
}

/**
 * Arithmetic shift right.
 *
 * Same as logic shift right but performing sign extension.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update srai(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  Uint32 sign = regs[REGISTER(inst.second)] & 0x80000000;
  regs.set_word(REGISTER(inst.first),
                (regs[REGISTER(inst.second)] >> inst.address) | sign);

  return UpdatePC;
}

/**
 * Rotate left.
 *
 * Same as logic shift left but inserting the removed bits on the right.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update rl(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
          Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] << regs[REGISTER(inst.address)] |
                regs[REGISTER(inst.second)] >>
                (32 - (regs[REGISTER(inst.address)] % 32)));

  return UpdatePC;
}

/**
 * Rotate left.
 *
 * Same as logic shift left but inserting the removed bits on the right.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update rli(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] << inst.address |
                regs[REGISTER(inst.second)] >> (32 - (inst.address % 32)));

  return UpdatePC;
}

/**
 * Rotate right.
 *
 * Same as logic shift right but inserting the removed bits on the left.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update rr(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
          Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] >> regs[REGISTER(inst.address)] |
                regs[REGISTER(inst.second)] <<
                (32 - (regs[REGISTER(inst.address)] % 32)));

  return UpdatePC;
}

/**
 * Rotate right.
 *
 * Same as logic shift right but inserting the removed bits on the left.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update rri(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] >> inst.address |
                regs[REGISTER(inst.second)] << (32 - (inst.address % 32)));

  return UpdatePC;
}

}
}
