/**
 * @file simpleworld/cpu/operations_shift.cpp
 * Shift operations of the Simple CPU.
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

#include "operations.hpp"

namespace simpleworld
{
namespace cpu
{

/**
 * Logic shift left.
 *
 * Move the bits X positions to the left inserting zeros on the right.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update sll(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first, cpu.get_reg(inst.second) << cpu.get_reg(inst.data));

  return UpdatePC;
}

/**
 * Logic shift left.
 *
 * Move the bits X positions to the left inserting zeros on the right.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update slli(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first, cpu.get_reg(inst.second) << inst.data);

  return UpdatePC;
}

/**
 * Logic shift right.
 *
 * Move the bits X positions to the right inserting zeros on the left.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update srl(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first, cpu.get_reg(inst.second) >> cpu.get_reg(inst.data));

  return UpdatePC;
}

/**
 * Logic shift right.
 *
 * Move the bits X positions to the right inserting zeros on the left.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update srli(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first, cpu.get_reg(inst.second) >> inst.data);

  return UpdatePC;
}

/**
 * Arithmetic shift left.
 *
 * Same as logic shift left.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update sla(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first, cpu.get_reg(inst.second) << cpu.get_reg(inst.data));

  return UpdatePC;
}

/**
 * Arithmetic shift left.
 *
 * Same as logic shift left.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update slai(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first, cpu.get_reg(inst.second) << inst.data);

  return UpdatePC;
}

/**
 * Arithmetic shift right.
 *
 * Same as logic shift right but performing sign extension.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update sra(CPU& cpu, Instruction inst)
{
  Uint32 sign = cpu.get_reg(inst.second) & 0x80000000;
  cpu.set_reg(inst.first,
              (cpu.get_reg(inst.second) >> cpu.get_reg(inst.data)) | sign);

  return UpdatePC;
}

/**
 * Arithmetic shift right.
 *
 * Same as logic shift right but performing sign extension.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update srai(CPU& cpu, Instruction inst)
{
  Uint32 sign = cpu.get_reg(inst.second) & 0x80000000;
  cpu.set_reg(inst.first, (cpu.get_reg(inst.second) >> inst.data) | sign);

  return UpdatePC;
}

/**
 * Rotate left.
 *
 * Same as logic shift left but inserting the removed bits on the right.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update rl(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first,
              cpu.get_reg(inst.second) << cpu.get_reg(inst.data) |
              cpu.get_reg(inst.second) >> (32 -
                                           (cpu.get_reg(inst.data) % 32)));

  return UpdatePC;
}

/**
 * Rotate left.
 *
 * Same as logic shift left but inserting the removed bits on the right.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update rli(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first,
              cpu.get_reg(inst.second) << inst.data |
              cpu.get_reg(inst.second) >> (32 - (inst.data % 32)));

  return UpdatePC;
}

/**
 * Rotate right.
 *
 * Same as logic shift right but inserting the removed bits on the left.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update rr(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first,
              cpu.get_reg(inst.second) >> cpu.get_reg(inst.data) |
              cpu.get_reg(inst.second) << (32 -
                                           (cpu.get_reg(inst.data) % 32)));

  return UpdatePC;
}

/**
 * Rotate right.
 *
 * Same as logic shift right but inserting the removed bits on the left.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update rri(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first,
              cpu.get_reg(inst.second) >> inst.data |
              cpu.get_reg(inst.second) << (32 - (inst.data % 32)));

  return UpdatePC;
}

}
}
