/**
 * @file simpleworld/cpu/operations_branch.cpp
 * Branch operations of the Simple CPU.
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

// Before executing each operation, it's checked if the address is valid.
// If the address is out of range, a Invalid Memory location is raised
// giving the as data.


/**
 * Branch always.
 *
 * PC += OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update b(CPU& cpu, Instruction inst)
{
  // Check if the address is valid.
  Address address = cpu.get_reg(REGISTER_PC) + inst.offset;
  cpu.get_mem(address);

  cpu.set_reg(REGISTER_PC, address);

  return UpdateNone;
}

/**
 * Branch if zero.
 *
 * if REGISTERS[FIRST] == 0 -> PC += OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bz(CPU& cpu, Instruction inst)
{
  // Check if the address is valid.
  Address address = cpu.get_reg(REGISTER_PC) + inst.offset;
  cpu.get_mem(address);

  if (cpu.get_reg(inst.first) == 0) {
    cpu.set_reg(REGISTER_PC, address);
    return UpdateNone;
  }

  return UpdatePC;
}

/**
 * Branch if not zero.
 *
 * if REGISTERS[FIRST] != 0 -> PC += OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bnz(CPU& cpu, Instruction inst)
{
  // Check if the address is valid.
  Address address = cpu.get_reg(REGISTER_PC) + inst.offset;
  cpu.get_mem(address);

  if (cpu.get_reg(inst.first) != 0) {
    cpu.set_reg(REGISTER_PC, address);
    return UpdateNone;
  }

  return UpdatePC;
}

/**
 * Branch on equal.
 *
 * if REGISTERS[FIRST] == REGISTERS[SECOND] -> PC += OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update beq(CPU& cpu, Instruction inst)
{
  // Check if the address is valid.
  Address address = cpu.get_reg(REGISTER_PC) + inst.offset;
  cpu.get_mem(address);

  if (cpu.get_reg(inst.first) == cpu.get_reg(inst.second)) {
    cpu.set_reg(REGISTER_PC, address);
    return UpdateNone;
  }

  return UpdatePC;
}

/**
 * Branch on not equal.
 *
 * if REGISTERS[FIRST] != REGISTERS[SECOND] -> PC += OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bne(CPU& cpu, Instruction inst)
{
  // Check if the address is valid.
  Address address = cpu.get_reg(REGISTER_PC) + inst.offset;
  cpu.get_mem(address);

  if (cpu.get_reg(inst.first) != cpu.get_reg(inst.second)) {
    cpu.set_reg(REGISTER_PC, address);
    return UpdateNone;
  }

  return UpdatePC;
}

/**
 * Branch on less than (signed comparission).
 *
 * if REGISTERS[FIRST] < REGISTERS[SECOND] -> PC += OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update blt(CPU& cpu, Instruction inst)
{
  // Check if the address is valid.
  Address address = cpu.get_reg(REGISTER_PC) + inst.offset;
  cpu.get_mem(address);

  if (static_cast<Sint32>(cpu.get_reg(inst.first)) <
      static_cast<Sint32>(cpu.get_reg(inst.second))) {
    cpu.set_reg(REGISTER_PC, address);
    return UpdateNone;
  }

  return UpdatePC;
}

/**
 * Branch on less than (unsigned comparission).
 *
 * if REGISTERS[FIRST] < REGISTERS[SECOND] -> PC += OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bltu(CPU& cpu, Instruction inst)
{
  // Check if the address is valid.
  Address address = cpu.get_reg(REGISTER_PC) + inst.offset;
  cpu.get_mem(address);

  if (cpu.get_reg(inst.first) < cpu.get_reg(inst.second)) {
    cpu.set_reg(REGISTER_PC, address);
    return UpdateNone;
  }

  return UpdatePC;
}

/**
 * Branch on great than (signed comparission).
 *
 * if REGISTERS[FIRST] > REGISTERS[SECOND] -> PC += OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bgt(CPU& cpu, Instruction inst)
{
  // Check if the address is valid.
  Address address = cpu.get_reg(REGISTER_PC) + inst.offset;
  cpu.get_mem(address);

  if (static_cast<Sint32>(cpu.get_reg(inst.first)) >
      static_cast<Sint32>(cpu.get_reg(inst.second))) {
    cpu.set_reg(REGISTER_PC, address);
    return UpdateNone;
  }

  return UpdatePC;
}

/**
 * Branch on great than (unsigned comparission).
 *
 * if REGISTERS[FIRST] > REGISTERS[SECOND] -> PC += OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bgtu(CPU& cpu, Instruction inst)
{
  // Check if the address is valid.
  Address address = cpu.get_reg(REGISTER_PC) + inst.offset;
  cpu.get_mem(address);

  if (cpu.get_reg(inst.first) > cpu.get_reg(inst.second)) {
    cpu.set_reg(REGISTER_PC, address);
    return UpdateNone;
  }

  return UpdatePC;
}

/**
 * Branch on less or equal (signed comparission).
 *
 * if REGISTERS[FIRST] <= REGISTERS[SECOND] -> PC += OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update ble(CPU& cpu, Instruction inst)
{
  // Check if the address is valid.
  Address address = cpu.get_reg(REGISTER_PC) + inst.offset;
  cpu.get_mem(address);

  if (static_cast<Sint32>(cpu.get_reg(inst.first)) <=
      static_cast<Sint32>(cpu.get_reg(inst.second))) {
    cpu.set_reg(REGISTER_PC, address);
    return UpdateNone;
  }

  return UpdatePC;
}

/**
 * Branch on less or equal (unsigned comparission).
 *
 * if REGISTERS[FIRST] <= REGISTERS[SECOND] -> PC += OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bleu(CPU& cpu, Instruction inst)
{
  // Check if the address is valid.
  Address address = cpu.get_reg(REGISTER_PC) + inst.offset;
  cpu.get_mem(address);

  if (cpu.get_reg(inst.first) <= cpu.get_reg(inst.second)) {
    cpu.set_reg(REGISTER_PC, address);
    return UpdateNone;
  }

  return UpdatePC;
}

/**
 * Branch on greater or equal (signed comparission).
 *
 * if REGISTERS[FIRST] >= REGISTERS[SECOND] -> PC += OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bge(CPU& cpu, Instruction inst)
{
  // Check if the address is valid.
  Address address = cpu.get_reg(REGISTER_PC) + inst.offset;
  cpu.get_mem(address);

  if (static_cast<Sint32>(cpu.get_reg(inst.first)) >=
      static_cast<Sint32>(cpu.get_reg(inst.second))) {
    cpu.set_reg(REGISTER_PC, address);
    return UpdateNone;
  }

  return UpdatePC;
}

/**
 * Branch on greater or equal (unsigned comparission).
 *
 * if REGISTERS[FIRST] >= REGISTERS[SECOND] -> PC += OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bgeu(CPU& cpu, Instruction inst)
{
  // Check if the address is valid.
  Address address = cpu.get_reg(REGISTER_PC) + inst.offset;
  cpu.get_mem(address);

  if (cpu.get_reg(inst.first) >= cpu.get_reg(inst.second)) {
    cpu.set_reg(REGISTER_PC, address);
    return UpdateNone;
  }

  return UpdatePC;
}

}
}
