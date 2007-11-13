/**
 * @file simpleworld/cpu/operations_branch.cpp
 * Branch operations of the Simple CPU.
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
