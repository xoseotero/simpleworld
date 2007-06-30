/**
 * @file cpu/operations_shift.cpp
 * Shift operations of the Simple CPU.
 *
 * begin:     Sat, 11 Nov 2006 19:15:19 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2006, Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  License: See COPYING file that comes with this distribution
 */

#include "../simple/config.hpp"
#include "operations.hpp"

namespace SimpleWorld
{
namespace CPU
{

Update sll(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] << regs[REGISTER(inst.address)]);

  return UpdatePC;
}

Update slli(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] << inst.address);

  return UpdatePC;
}

Update srl(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] >> regs[REGISTER(inst.address)]);

  return UpdatePC;
}

Update srli(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] >> inst.address);

  return UpdatePC;
}

Update sla(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] << regs[REGISTER(inst.address)]);

  return UpdatePC;
}

Update slai(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] << inst.address);

  return UpdatePC;
}

Update sra(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
#ifdef IS_BIG_ENDIAN
  Uint32 sign = regs[REGISTER(inst.second)] & 0x80000000;
#else
  Uint32 sign = regs[REGISTER(inst.second)] & 0x00000001;
#endif
  regs.set_word(REGISTER(inst.first),
                (regs[REGISTER(inst.second)] >>
                 regs[REGISTER(inst.address)]) | sign);

  return UpdatePC;
}

Update srai(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
#ifdef IS_BIG_ENDIAN
  Uint32 sign = regs[REGISTER(inst.second)] & 0x80000000;
#else
  Uint32 sign = regs[REGISTER(inst.second)] & 0x00000001;
#endif
  regs.set_word(REGISTER(inst.first),
                (regs[REGISTER(inst.second)] >> inst.address) | sign);

  return UpdatePC;
}

Update rl(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] << regs[REGISTER(inst.address)] |
                regs[REGISTER(inst.second)] >>
                (32 - (regs[REGISTER(inst.address)] % 32)));

  return UpdatePC;
}

Update rli(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] << inst.address |
                regs[REGISTER(inst.second)] >> (32 - (inst.address % 32)));

  return UpdatePC;
}

Update rr(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] >> regs[REGISTER(inst.address)] |
                regs[REGISTER(inst.second)] <<
                (32 - (regs[REGISTER(inst.address)] % 32)));

  return UpdatePC;
}

Update rri(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] >> inst.address |
                regs[REGISTER(inst.second)] << (32 - (inst.address % 32)));

  return UpdatePC;
}

}
}
