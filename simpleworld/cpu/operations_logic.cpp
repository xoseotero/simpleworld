/**
 * @file simpleworld/cpu/operations_logic.cpp
 * Logic operations of the Simple CPU.
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
 * NOT of a register.
 *
 * REGISTERS[FIRST] = ~REGISTERS[SECOND]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lnot(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  regs.set_word(REGISTER(inst.first), ~ regs[REGISTER(inst.second)]);

  return UpdatePC;
}

/**
 * OR of two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] | REGISTERS[ADDRESS]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lor(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] | regs[REGISTER(inst.address)]);

  return UpdatePC;
}

/**
 * OR of a register and a inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] | ADDRESS
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lori(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] | inst.address);

  return UpdatePC;
}

/**
 * AND of two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] & REGISTERS[ADDRESS]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update land(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] & regs[REGISTER(inst.address)]);

  return UpdatePC;
}

/**
 * AND of a register and a inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] & ADDRESS
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update landi(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
             Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] & inst.address);

  return UpdatePC;
}

/**
 * EXOR of two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] ^ REGISTERS[ADDRESS]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lxor(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] ^ regs[REGISTER(inst.address)]);

  return UpdatePC;
}

/**
 * EXOR of a register and a inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] ^ ADDRESS
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lxori(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
             Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] ^ inst.address);

  return UpdatePC;
}

}
}
