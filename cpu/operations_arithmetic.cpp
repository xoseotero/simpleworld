/**
 * @file cpu/operations_arithmetic.cpp
 * Arithmetic operations of the Simple CPU.
 *
 * begin:     Sat, 11 Nov 2006 19:15:19 +0100
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2006, Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  License: See COPYING file that comes with this distribution
 */

#include "../simple/config.hpp"
#include "operations.hpp"

#ifdef IS_BIG_ENDIAN
#define HIGHBITS_64BITS SINT64(0xFFFFFFFF00000000)
#define LOWBITS_64BITS SINT64(0x00000000FFFFFFFF)
#else
#define HIGHBITS_64BITS SINT64(0x00000000FFFFFFFF)
#define LOWBITS_64BITS SINT64(0xFFFFFFFF00000000)
#endif

namespace SimpleWorld
{
namespace CPU
{

Update add(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  regs.set_word(REGISTER(inst.first), regs[REGISTER(inst.second)] +
                regs[REGISTER(inst.address)]);

  return UpdatePC;
}

Update addi(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  regs.set_word(REGISTER(inst.first), regs[REGISTER(inst.second)] +
                inst.address);

  return UpdatePC;
}

Update sub(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  regs.set_word(REGISTER(inst.first), regs[REGISTER(inst.second)] -
                regs[REGISTER(inst.address)]);

  return UpdatePC;
}

Update subi(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  regs.set_word(REGISTER(inst.first), regs[REGISTER(inst.second)] -
                inst.address);

  return UpdatePC;
}

Update multl(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  Sint64 result = static_cast<Sint32>(regs[REGISTER(inst.second)]) *
    static_cast<Sint32>(regs[REGISTER(inst.address)]);
  regs.set_word(REGISTER(inst.first),
                static_cast<Sint32>(result & LOWBITS_64BITS));

  return UpdatePC;
}

Update multli(Memory& regs, Memory& mem, Interrupt& interrupt,
	      Instruction inst)
{
  Sint64 result = static_cast<Sint32>(regs[REGISTER(inst.second)]) *
    static_cast<Sint32>(inst.address);
  regs.set_word(REGISTER(inst.first),
                static_cast<Sint32>(result & LOWBITS_64BITS));

  return UpdatePC;
}

Update multlu(Memory& regs, Memory& mem, Interrupt& interrupt,
	      Instruction inst)
{
  Uint64 result = regs[REGISTER(inst.second)] * regs[REGISTER(inst.address)];
  regs.set_word(REGISTER(inst.first),
                static_cast<Uint32>(result & LOWBITS_64BITS));

  return UpdatePC;
}

Update multlui(Memory& regs, Memory& mem, Interrupt& interrupt,
	       Instruction inst)
{
  Uint64 result = regs[REGISTER(inst.second)] *
    static_cast<Uint32>(inst.address);
  regs.set_word(REGISTER(inst.first),
                static_cast<Uint32>(result & LOWBITS_64BITS));

  return UpdatePC;
}

Update multh(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  Sint64 result = static_cast<Sint32>(regs[REGISTER(inst.second)]) *
    static_cast<Sint32>(regs[REGISTER(inst.address)]);
  regs.set_word(REGISTER(inst.first),
                static_cast<Sint32>((result & HIGHBITS_64BITS) >> 32));

  return UpdatePC;
}

Update multhi(Memory& regs, Memory& mem, Interrupt& interrupt,
	      Instruction inst)
{
  Sint64 result = static_cast<Sint32>(regs[REGISTER(inst.second)]) *
    static_cast<Sint32>(inst.address);
  regs.set_word(REGISTER(inst.first),
                static_cast<Sint32>((result & HIGHBITS_64BITS) >> 32));

  return UpdatePC;
}

Update multhu(Memory& regs, Memory& mem, Interrupt& interrupt,
	      Instruction inst)
{
  Uint64 result = regs[REGISTER(inst.second)] * regs[REGISTER(inst.address)];
  regs.set_word(REGISTER(inst.first),
                static_cast<Uint32>((result & HIGHBITS_64BITS) >> 32));

  return UpdatePC;
}

Update multhui(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  Uint64 result = regs[REGISTER(inst.second)] * inst.address;
  regs.set_word(REGISTER(inst.first),
                static_cast<Uint32>((result & HIGHBITS_64BITS) >> 32));

  return UpdatePC;
}

Update div(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  Word dividend = static_cast<Sint32>(regs[REGISTER(inst.second)]);
  Word divisor = static_cast<Sint32>(regs[REGISTER(inst.address)]);
  if (divisor == 0) {
    interrupt.type = DivisionByZero;
    interrupt.r0 = static_cast<Word>(DivisionByZero);
    interrupt.r1 = regs[REGISTER_PC];
    interrupt.r2 = dividend;

    return UpdateInterrupt;
  } else {
    regs.set_word(REGISTER(inst.first), dividend / divisor);

    return UpdatePC;
  }
}

Update divi(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  Word dividend = static_cast<Sint32>(regs[REGISTER(inst.second)]);
  Word divisor = static_cast<Sint32>(inst.address);
  if (divisor == 0) {
    interrupt.type = DivisionByZero;
    interrupt.r0 = static_cast<Word>(DivisionByZero);
    interrupt.r1 = regs[REGISTER_PC];
    interrupt.r2 = dividend;

    return UpdateInterrupt;
  } else {
    regs.set_word(REGISTER(inst.first), dividend / divisor);

    return UpdatePC;
  }
}

Update divu(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  Word dividend = regs[REGISTER(inst.second)];
                       Word divisor = regs[REGISTER(inst.address)];
  if (divisor == 0) {
    interrupt.type = DivisionByZero;
    interrupt.r0 = static_cast<Word>(DivisionByZero);
    interrupt.r1 = regs[REGISTER_PC];
    interrupt.r2 = dividend;

    return UpdateInterrupt;
  } else {
    regs.set_word(REGISTER(inst.first), dividend / divisor);

    return UpdatePC;
  }
}

Update divui(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  Word dividend = regs[REGISTER(inst.second)];
  Word divisor = inst.address;
  if (divisor == 0) {
    interrupt.type = DivisionByZero;
    interrupt.r0 = static_cast<Word>(DivisionByZero);
    interrupt.r1 = regs[REGISTER_PC];
    interrupt.r2 = dividend;

    return UpdateInterrupt;
  } else {
    regs.set_word(REGISTER(inst.first), dividend / divisor);

    return UpdatePC;
  }
}

Update mod(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  Word dividend = static_cast<Sint32>(regs[REGISTER(inst.second)]);
  Word divisor = static_cast<Sint32>(regs[REGISTER(inst.address)]);
  if (divisor == 0) {
    interrupt.type = DivisionByZero;
    interrupt.r0 = static_cast<Word>(DivisionByZero);
    interrupt.r1 = regs[REGISTER_PC];
    interrupt.r2 = dividend;

    return UpdateInterrupt;
  } else {
    regs.set_word(REGISTER(inst.first), dividend % divisor);

    return UpdatePC;
  }
}

Update modi(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  Word dividend = static_cast<Sint32>(regs[REGISTER(inst.second)]);
  Word divisor = static_cast<Sint32>(inst.address);
  if (divisor == 0) {
    interrupt.type = DivisionByZero;
    interrupt.r0 = static_cast<Word>(DivisionByZero);
    interrupt.r1 = regs[REGISTER_PC];
    interrupt.r2 = dividend;

    return UpdateInterrupt;
  } else {
    regs.set_word(REGISTER(inst.first), dividend % divisor);

    return UpdatePC;
  }
}

Update modu(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  Word dividend = regs[REGISTER(inst.second)];
  Word divisor = regs[REGISTER(inst.address)];
  if (divisor == 0) {
    interrupt.type = DivisionByZero;
    interrupt.r0 = static_cast<Word>(DivisionByZero);
    interrupt.r1 = regs[REGISTER_PC];
    interrupt.r2 = dividend;

    return UpdateInterrupt;
  } else {
    regs.set_word(REGISTER(inst.first), dividend % divisor);

    return UpdatePC;
  }
}

Update modui(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  Word dividend = regs[REGISTER(inst.second)];
  Word divisor = inst.address;
  if (divisor == 0) {
    interrupt.type = DivisionByZero;
    interrupt.r0 = static_cast<Word>(DivisionByZero);
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
