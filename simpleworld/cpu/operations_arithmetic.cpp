/**
 * @file simpleworld/cpu/operations_arithmetic.cpp
 * Arithmetic operations of the Simple CPU.
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

#define LOWBITS_64BITS SINT64(0x00000000ffffffff)

namespace SimpleWorld
{
namespace CPU
{

/**
 * Add two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] + REGISTERS[ADDRESS]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update add(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst)
{
  regs.set_word(REGISTER(inst.first), regs[REGISTER(inst.second)] +
                regs[REGISTER(inst.data)]);

  return UpdatePC;
}

/**
 * Add a register and the data.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] + ADDRESS
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update addi(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  regs.set_word(REGISTER(inst.first), regs[REGISTER(inst.second)] +
                inst.data);

  return UpdatePC;
}

/**
 * Substract two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] - REGISTERS[ADDRESS]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update sub(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst)
{
  regs.set_word(REGISTER(inst.first), regs[REGISTER(inst.second)] -
                regs[REGISTER(inst.data)]);

  return UpdatePC;
}

/**
 * Substract a register and the data.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] - ADDRESS
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update subi(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  regs.set_word(REGISTER(inst.first), regs[REGISTER(inst.second)] -
                inst.data);

  return UpdatePC;
}

/**
 * Low 32bits from multiply two signed registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * REGISTERS[ADDRESS]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multl(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
             Instruction inst)
{
  Sint64 result = static_cast<Sint32>(regs[REGISTER(inst.second)]) *
    static_cast<Sint32>(regs[REGISTER(inst.data)]);
  regs.set_word(REGISTER(inst.first),
                static_cast<Sint32>(result & LOWBITS_64BITS));

  return UpdatePC;
}

/**
 * Low 32bits from multiply a signed registers and the data (signed value).
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * ADDRESS
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multli(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
              Instruction inst)
{
  Sint64 result = static_cast<Sint32>(regs[REGISTER(inst.second)]) *
    static_cast<Sint32>(inst.data);
  regs.set_word(REGISTER(inst.first),
                static_cast<Sint32>(result & LOWBITS_64BITS));

  return UpdatePC;
}

/**
 * Low 32bits from multiply two unsigned registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * REGISTERS[ADDRESS]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multlu(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
              Instruction inst)
{
  Uint64 result = regs[REGISTER(inst.second)] * regs[REGISTER(inst.data)];
  regs.set_word(REGISTER(inst.first),
                static_cast<Uint32>(result & LOWBITS_64BITS));

  return UpdatePC;
}

/**
 * Low 32bits from multiply a unsigned registers and the data (unsigned value).
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * ADDRESS
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multlui(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
               Instruction inst)
{
  Uint64 result = regs[REGISTER(inst.second)] *
    static_cast<Uint32>(inst.data);
  regs.set_word(REGISTER(inst.first),
                static_cast<Uint32>(result & LOWBITS_64BITS));

  return UpdatePC;
}

/**
 * High 32bits from multiply two signed registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * REGISTERS[ADDRESS]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multh(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
             Instruction inst)
{
  Sint64 result = static_cast<Sint32>(regs[REGISTER(inst.second)]) *
    static_cast<Sint32>(regs[REGISTER(inst.data)]);
  regs.set_word(REGISTER(inst.first), static_cast<Sint32>(result >> 32));

  return UpdatePC;
}

/**
 * High 32bits from multiply a signed registers and the data (signed value).
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * ADDRESS
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multhi(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
              Instruction inst)
{
  Sint64 result = static_cast<Sint32>(regs[REGISTER(inst.second)]) *
    static_cast<Sint32>(inst.data);
  regs.set_word(REGISTER(inst.first), static_cast<Sint32>(result >> 32));

  return UpdatePC;
}

/**
 * High 32bits from multiply two unsigned registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * REGISTERS[ADDRESS]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multhu(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
              Instruction inst)
{
  Uint64 result = static_cast<Uint64>(regs[REGISTER(inst.second)]) *
    regs[REGISTER(inst.data)];
  regs.set_word(REGISTER(inst.first), static_cast<Uint32>(result >> 32));

  return UpdatePC;
}

/**
 * High 32bits from multiply a unsigned registers and the data (unsigned value).
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * ADDRESS
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multhui(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
               Instruction inst)
{
  Uint64 result = static_cast<Uint64>(regs[REGISTER(inst.second)]) *
    inst.data;
  regs.set_word(REGISTER(inst.first), static_cast<Uint32>(result >> 32));

  return UpdatePC;
}

/**
 * Divide two signed registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] / REGISTERS[ADDRESS]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update div(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst)
{
  Word dividend = static_cast<Sint32>(regs[REGISTER(inst.second)]);
  Word divisor = static_cast<Sint32>(regs[REGISTER(inst.data)]);
  if (divisor == 0) {
    Word code = static_cast<Word>(isa.interrupt_code("DivisionByZero"));
    interrupt.code = interrupt.r0 = code;
    interrupt.r1 = regs[REGISTER_PC];
    interrupt.r2 = dividend;

    return UpdateInterrupt;
  } else {
    regs.set_word(REGISTER(inst.first), dividend / divisor);

    return UpdatePC;
  }
}

/**
 * Divide a signed register and the data (signed value).
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] / ADDRESS
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update divi(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  Word dividend = static_cast<Sint32>(regs[REGISTER(inst.second)]);
  Word divisor = static_cast<Sint32>(inst.data);
  if (divisor == 0) {
    Word code = static_cast<Word>(isa.interrupt_code("DivisionByZero"));
    interrupt.code = interrupt.r0 = code;
    interrupt.r1 = regs[REGISTER_PC];
    interrupt.r2 = dividend;

    return UpdateInterrupt;
  } else {
    regs.set_word(REGISTER(inst.first), dividend / divisor);

    return UpdatePC;
  }
}

/**
 * Divide two unsigned registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] / REGISTERS[ADDRESS]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update divu(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  Word dividend = regs[REGISTER(inst.second)];
                       Word divisor = regs[REGISTER(inst.data)];
  if (divisor == 0) {
    Word code = static_cast<Word>(isa.interrupt_code("DivisionByZero"));
    interrupt.code = interrupt.r0 = code;
    interrupt.r1 = regs[REGISTER_PC];
    interrupt.r2 = dividend;

    return UpdateInterrupt;
  } else {
    regs.set_word(REGISTER(inst.first), dividend / divisor);

    return UpdatePC;
  }
}

/**
 * Divide a unsigned register and a the data (unsigned value).
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] / ADDRESS
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update divui(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
             Instruction inst)
{
  Word dividend = regs[REGISTER(inst.second)];
  Word divisor = inst.data;
  if (divisor == 0) {
    Word code = static_cast<Word>(isa.interrupt_code("DivisionByZero"));
    interrupt.code = interrupt.r0 = code;
    interrupt.r1 = regs[REGISTER_PC];
    interrupt.r2 = dividend;

    return UpdateInterrupt;
  } else {
    regs.set_word(REGISTER(inst.first), dividend / divisor);

    return UpdatePC;
  }
}

/**
 * Module of two signed registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] % REGISTERS[ADDRESS]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update mod(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst)
{
  Word dividend = static_cast<Sint32>(regs[REGISTER(inst.second)]);
  Word divisor = static_cast<Sint32>(regs[REGISTER(inst.data)]);
  if (divisor == 0) {
    Word code = static_cast<Word>(isa.interrupt_code("DivisionByZero"));
    interrupt.code = interrupt.r0 = code;
    interrupt.r1 = regs[REGISTER_PC];
    interrupt.r2 = dividend;

    return UpdateInterrupt;
  } else {
    regs.set_word(REGISTER(inst.first), dividend % divisor);

    return UpdatePC;
  }
}

/**
 * Module of a signed register and the data (signed value).
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] % ADDRESS
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update modi(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  Word dividend = static_cast<Sint32>(regs[REGISTER(inst.second)]);
  Word divisor = static_cast<Sint32>(inst.data);
  if (divisor == 0) {
    Word code = static_cast<Word>(isa.interrupt_code("DivisionByZero"));
    interrupt.code = interrupt.r0 = code;
    interrupt.r1 = regs[REGISTER_PC];
    interrupt.r2 = dividend;

    return UpdateInterrupt;
  } else {
    regs.set_word(REGISTER(inst.first), dividend % divisor);

    return UpdatePC;
  }
}

/**
 * Module of two unsigned registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] % REGISTERS[ADDRESS]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update modu(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  Word dividend = regs[REGISTER(inst.second)];
  Word divisor = regs[REGISTER(inst.data)];
  if (divisor == 0) {
    Word code = static_cast<Word>(isa.interrupt_code("DivisionByZero"));
    interrupt.code = interrupt.r0 = code;
    interrupt.r1 = regs[REGISTER_PC];
    interrupt.r2 = dividend;

    return UpdateInterrupt;
  } else {
    regs.set_word(REGISTER(inst.first), dividend % divisor);

    return UpdatePC;
  }
}

/**
 * Module of a unsigned register and the data (unsigned value).
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] % ADDRESS
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update modui(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
             Instruction inst)
{
  Word dividend = regs[REGISTER(inst.second)];
  Word divisor = inst.data;
  if (divisor == 0) {
    Word code = static_cast<Word>(isa.interrupt_code("DivisionByZero"));
    interrupt.code = interrupt.r0 = code;
    interrupt.r1 = regs[REGISTER_PC];
    interrupt.r2 = dividend;

    return UpdateInterrupt;
  } else {
    regs.set_word(REGISTER(inst.first), dividend % inst.data);

    return UpdatePC;
  }
}

}
}
