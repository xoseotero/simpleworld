/**
 * @file cpu/operations_branch.cpp
 * Branch operations of the Simple CPU.
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

Update b(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  regs.set_word(REGISTER_PC, inst.address);

  return None;
}

Update beq(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  if (regs[REGISTER(inst.first)] == regs[REGISTER(inst.second)]) {
    regs.set_word(REGISTER_PC, inst.address);
    return None;
  }

  return UpdatePC;
}

Update bne(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  if (regs[REGISTER(inst.first)] != regs[REGISTER(inst.second)]) {
    regs.set_word(REGISTER_PC, inst.address);
    return None;
  }

  return UpdatePC;
}

Update blt(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  if (static_cast<Sint32>(regs[REGISTER(inst.first)]) <
      static_cast<Sint32>(regs[REGISTER(inst.second)])) {
    regs.set_word(REGISTER_PC, inst.address);
    return None;
  }

  return UpdatePC;
}

Update bltu(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  if (regs[REGISTER(inst.first)] < regs[REGISTER(inst.second)]) {
    regs.set_word(REGISTER_PC, inst.address);
    return None;
  }

  return UpdatePC;
}

Update bgt(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  if (static_cast<Sint32>(regs[REGISTER(inst.first)]) >
      static_cast<Sint32>(regs[REGISTER(inst.second)])) {
    regs.set_word(REGISTER_PC, inst.address);
    return None;
  }

  return UpdatePC;
}

Update bgtu(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  if (regs[REGISTER(inst.first)] > regs[REGISTER(inst.second)]) {
    regs.set_word(REGISTER_PC, inst.address);
    return None;
  }

  return UpdatePC;
}

Update ble(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  if (static_cast<Sint32>(regs[REGISTER(inst.first)]) <=
      static_cast<Sint32>(regs[REGISTER(inst.second)])) {
    regs.set_word(REGISTER_PC, inst.address);
    return None;
  }

  return UpdatePC;
}

Update bleu(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  if (regs[REGISTER(inst.first)] <= regs[REGISTER(inst.second)]) {
    regs.set_word(REGISTER_PC, inst.address);
    return None;
  }

  return UpdatePC;
}

Update bge(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  if (static_cast<Sint32>(regs[REGISTER(inst.first)]) >=
      static_cast<Sint32>(regs[REGISTER(inst.second)])) {
    regs.set_word(REGISTER_PC, inst.address);
    return None;
  }

  return UpdatePC;
}

Update bgeu(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  if (regs[REGISTER(inst.first)] >= regs[REGISTER(inst.second)]) {
    regs.set_word(REGISTER_PC, inst.address);
    return None;
  }

  return UpdatePC;
}

}
}
