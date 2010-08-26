/**
 * @file simpleworld/cpu/operations_logic.cpp
 * Logic operations of the Simple CPU.
 *
 *  Copyright (C) 2006-2008  Xosé Otero <xoseotero@gmail.com>
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

namespace simpleworld
{
namespace cpu
{

/**
 * NOT of a register.
 *
 * REGISTERS[FIRST] = ~REGISTERS[SECOND]
 * @param regs the registers.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lnot(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first, ~ cpu.get_reg(inst.second));

  return UpdatePC;
}

/**
 * OR of two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] | REGISTERS[ADDRESS]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lor(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first, cpu.get_reg(inst.second) | cpu.get_reg(inst.data));

  return UpdatePC;
}

/**
 * OR of a register and the data.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] | ADDRESS
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lori(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first, cpu.get_reg(inst.second) | inst.data);

  return UpdatePC;
}

/**
 * AND of two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] & REGISTERS[ADDRESS]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update land(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first, cpu.get_reg(inst.second) & cpu.get_reg(inst.data));

  return UpdatePC;
}

/**
 * AND of a register and the data.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] & ADDRESS
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update landi(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first, cpu.get_reg(inst.second) & inst.data);

  return UpdatePC;
}

/**
 * EXOR of two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] ^ REGISTERS[ADDRESS]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lxor(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first, cpu.get_reg(inst.second) ^ cpu.get_reg(inst.data));

  return UpdatePC;
}

/**
 * EXOR of a register and the data.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] ^ ADDRESS
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lxori(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first, cpu.get_reg(inst.second) ^ inst.data);

  return UpdatePC;
}

}
}
