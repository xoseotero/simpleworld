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

#define HIGHBITS_64BITS SINT64(0xFFFFFFFF00000000)
#define LOWBITS_64BITS SINT64(0x00000000FFFFFFFF)

namespace SimpleWorld
{
namespace CPU
{

Update add(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst)
{
  regs.set_word(REGISTER(inst.first), regs[REGISTER(inst.second)] +
                regs[REGISTER(inst.address)]);

  return UpdatePC;
}

Update addi(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  regs.set_word(REGISTER(inst.first), regs[REGISTER(inst.second)] +
                inst.address);

  return UpdatePC;
}

Update sub(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst)
{
  regs.set_word(REGISTER(inst.first), regs[REGISTER(inst.second)] -
                regs[REGISTER(inst.address)]);

  return UpdatePC;
}

Update subi(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  regs.set_word(REGISTER(inst.first), regs[REGISTER(inst.second)] -
                inst.address);

  return UpdatePC;
}

Update multl(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
             Instruction inst)
{
  Sint64 result = static_cast<Sint32>(regs[REGISTER(inst.second)]) *
    static_cast<Sint32>(regs[REGISTER(inst.address)]);
  regs.set_word(REGISTER(inst.first),
                static_cast<Sint32>(result & LOWBITS_64BITS));

  return UpdatePC;
}

Update multli(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
              Instruction inst)
{
  Sint64 result = static_cast<Sint32>(regs[REGISTER(inst.second)]) *
    static_cast<Sint32>(inst.address);
  regs.set_word(REGISTER(inst.first),
                static_cast<Sint32>(result & LOWBITS_64BITS));

  return UpdatePC;
}

Update multlu(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
              Instruction inst)
{
  Uint64 result = regs[REGISTER(inst.second)] * regs[REGISTER(inst.address)];
  regs.set_word(REGISTER(inst.first),
                static_cast<Uint32>(result & LOWBITS_64BITS));

  return UpdatePC;
}

Update multlui(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
               Instruction inst)
{
  Uint64 result = regs[REGISTER(inst.second)] *
    static_cast<Uint32>(inst.address);
  regs.set_word(REGISTER(inst.first),
                static_cast<Uint32>(result & LOWBITS_64BITS));

  return UpdatePC;
}

Update multh(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
             Instruction inst)
{
  Sint64 result = static_cast<Sint32>(regs[REGISTER(inst.second)]) *
    static_cast<Sint32>(regs[REGISTER(inst.address)]);
  regs.set_word(REGISTER(inst.first),
                static_cast<Sint32>((result & HIGHBITS_64BITS) >> 32));

  return UpdatePC;
}

Update multhi(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
              Instruction inst)
{
  Sint64 result = static_cast<Sint32>(regs[REGISTER(inst.second)]) *
    static_cast<Sint32>(inst.address);
  regs.set_word(REGISTER(inst.first),
                static_cast<Sint32>((result & HIGHBITS_64BITS) >> 32));

  return UpdatePC;
}

Update multhu(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
              Instruction inst)
{
  Uint64 result = static_cast<Uint64>(regs[REGISTER(inst.second)]) *
    regs[REGISTER(inst.address)];
  regs.set_word(REGISTER(inst.first),
                static_cast<Uint32>((result & HIGHBITS_64BITS) >> 32));

  return UpdatePC;
}

Update multhui(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
               Instruction inst)
{
  Uint64 result = static_cast<Uint64>(regs[REGISTER(inst.second)]) *
    inst.address;
  regs.set_word(REGISTER(inst.first),
                static_cast<Uint32>((result & HIGHBITS_64BITS) >> 32));

  return UpdatePC;
}

Update div(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst)
{
  Word dividend = static_cast<Sint32>(regs[REGISTER(inst.second)]);
  Word divisor = static_cast<Sint32>(regs[REGISTER(inst.address)]);
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

Update divi(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  Word dividend = static_cast<Sint32>(regs[REGISTER(inst.second)]);
  Word divisor = static_cast<Sint32>(inst.address);
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

Update divu(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  Word dividend = regs[REGISTER(inst.second)];
                       Word divisor = regs[REGISTER(inst.address)];
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

Update divui(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
             Instruction inst)
{
  Word dividend = regs[REGISTER(inst.second)];
  Word divisor = inst.address;
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

Update mod(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst)
{
  Word dividend = static_cast<Sint32>(regs[REGISTER(inst.second)]);
  Word divisor = static_cast<Sint32>(regs[REGISTER(inst.address)]);
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

Update modi(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  Word dividend = static_cast<Sint32>(regs[REGISTER(inst.second)]);
  Word divisor = static_cast<Sint32>(inst.address);
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

Update modu(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  Word dividend = regs[REGISTER(inst.second)];
  Word divisor = regs[REGISTER(inst.address)];
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

Update modui(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
             Instruction inst)
{
  Word dividend = regs[REGISTER(inst.second)];
  Word divisor = inst.address;
  if (divisor == 0) {
    Word code = static_cast<Word>(isa.interrupt_code("DivisionByZero"));
    interrupt.code = interrupt.r0 = code;
    interrupt.r1 = regs[REGISTER_PC];
    interrupt.r2 = dividend;

    return UpdateInterrupt;
  } else {
    regs.set_word(REGISTER(inst.first), dividend % inst.address);

    return UpdatePC;
  }
}

}
}
