/**
 * @file cpu/operations_shift.cpp
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
