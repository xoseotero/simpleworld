/**
 * @file cpu/operations_logic.cpp
 * Logic operations of the Simple CPU.
 *
 * begin:     Sat, 11 Nov 2006 19:15:19 +0100
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2006, Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  License: See COPYING file that comes with this distribution
 */

#include "operations.hpp"

namespace SimpleWorld
{
namespace CPU
{

Update lnot(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  regs.set_word(REGISTER(inst.first), ~ regs[REGISTER(inst.second)]);

  return UpdatePC;
}

Update lor(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] | regs[REGISTER(inst.address)]);

  return UpdatePC;
}

Update lori(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] | inst.address);

  return UpdatePC;
}

Update land(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] & regs[REGISTER(inst.address)]);

  return UpdatePC;
}

Update landi(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] & inst.address);

  return UpdatePC;
}

Update lxor(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] ^ regs[REGISTER(inst.address)]);

  return UpdatePC;
}

Update lxori(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] ^ inst.address);

  return UpdatePC;
}

}
}
