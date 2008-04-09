/**
 * @file simpleworld/cpu/operations_arithmetic.cpp
 * Arithmetic operations of the Simple CPU.
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

#include <simpleworld/ints.hpp>
#include "operations.hpp"

#define LOWBITS_64BITS SINT64(0x00000000ffffffff)

namespace simpleworld
{
namespace cpu
{

/**
 * Add two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] + REGISTERS[ADDRESS]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update add(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first, cpu.get_reg(inst.second) + cpu.get_reg(inst.data));

  return UpdatePC;
}

/**
 * Add a register and the data.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] + ADDRESS
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update addi(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first, cpu.get_reg(inst.second) + inst.data);

  return UpdatePC;
}

/**
 * Substract two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] - REGISTERS[ADDRESS]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update sub(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first, cpu.get_reg(inst.second) - cpu.get_reg(inst.data));

  return UpdatePC;
}

/**
 * Substract a register and the data.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] - ADDRESS
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update subi(CPU& cpu, Instruction inst)
{
  cpu.set_reg(inst.first, cpu.get_reg(inst.second) - inst.data);

  return UpdatePC;
}

/**
 * Low 32bits from multiply two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * REGISTERS[ADDRESS]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multl(CPU& cpu, Instruction inst)
{
  Sint64 result = cpu.get_reg(inst.second) * cpu.get_reg(inst.data);
  cpu.set_reg(inst.first, result & LOWBITS_64BITS);

  return UpdatePC;
}

/**
 * Low 32bits from multiply a registers and the data (signed value).
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * ADDRESS
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multli(CPU& cpu, Instruction inst)
{
  Sint64 result = cpu.get_reg(inst.second) * inst.data;
  cpu.set_reg(inst.first, result & LOWBITS_64BITS);

  return UpdatePC;
}

/**
 * High 32bits from multiply two signed registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * REGISTERS[ADDRESS]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multh(CPU& cpu, Instruction inst)
{
  Sint64 result = static_cast<Sint32>(cpu.get_reg(inst.second)) *
    static_cast<Sint32>(cpu.get_reg(inst.data));
  cpu.set_reg(inst.first, static_cast<Sint32>(result >> 32));

  return UpdatePC;
}

/**
 * High 32bits from multiply a signed registers and the data (signed value).
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * ADDRESS
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multhi(CPU& cpu, Instruction inst)
{
  Sint64 result = static_cast<Sint32>(cpu.get_reg(inst.second)) *
    static_cast<Sint32>(inst.data);
  cpu.set_reg(inst.first, static_cast<Sint32>(result >> 32));

  return UpdatePC;
}

/**
 * High 32bits from multiply two unsigned registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * REGISTERS[ADDRESS]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multhu(CPU& cpu, Instruction inst)
{
  Uint64 result = static_cast<Uint64>(cpu.get_reg(inst.second)) *
    cpu.get_reg(inst.data);
  cpu.set_reg(inst.first, static_cast<Uint32>(result >> 32));

  return UpdatePC;
}

/**
 * High 32bits from multiply a unsigned registers and the data (unsigned value).
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * ADDRESS
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multhui(CPU& cpu, Instruction inst)
{
  Uint64 result = static_cast<Uint64>(cpu.get_reg(inst.second)) * inst.data;
  cpu.set_reg(inst.first, static_cast<Uint32>(result >> 32));

  return UpdatePC;
}

/**
 * Divide two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] / REGISTERS[ADDRESS]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update div(CPU& cpu, Instruction inst)
{
  Word dividend = cpu.get_reg(inst.second);
  Word divisor = cpu.get_reg(inst.data);
  if (divisor == 0) {
    cpu.interrupt(INTERRUPT_DIVISION, cpu.get_reg(REGISTER_PC), dividend);

    return UpdateInterrupt;
  } else {
    cpu.set_reg(inst.first, dividend / divisor);

    return UpdatePC;
  }
}

/**
 * Divide a register and the data.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] / ADDRESS
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update divi(CPU& cpu, Instruction inst)
{
  Word dividend = cpu.get_reg(inst.second);
  Word divisor = inst.data;
  if (divisor == 0) {
    cpu.interrupt(INTERRUPT_DIVISION, cpu.get_reg(REGISTER_PC), dividend);

    return UpdateInterrupt;
  } else {
    cpu.set_reg(inst.first, dividend / divisor);

    return UpdatePC;
  }
}

/**
 * Module of two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] % REGISTERS[ADDRESS]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update mod(CPU& cpu, Instruction inst)
{
  Word dividend = cpu.get_reg(inst.second);
  Word divisor = cpu.get_reg(inst.data);
  if (divisor == 0) {
    cpu.interrupt(INTERRUPT_DIVISION, cpu.get_reg(REGISTER_PC), dividend);

    return UpdateInterrupt;
  } else {
    cpu.set_reg(inst.first, dividend % divisor);

    return UpdatePC;
  }
}

/**
 * Module of a register and the data.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] % ADDRESS
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update modi(CPU& cpu, Instruction inst)
{
  Word dividend = cpu.get_reg(inst.second);
  Word divisor = inst.data;
  if (divisor == 0) {
    cpu.interrupt(INTERRUPT_DIVISION, cpu.get_reg(REGISTER_PC), dividend);

    return UpdateInterrupt;
  } else {
    cpu.set_reg(inst.first, dividend % divisor);

    return UpdatePC;
  }
}

}
}
