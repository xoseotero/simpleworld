/**
 * @file cpu/operations_move.cpp
 * Load/store/move/stack operations of the Simple CPU.
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

#include "../simple/types.hpp"
#include "operations.hpp"

namespace SimpleWorld
{
namespace CPU
{

/* Move operations */
Update move(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  regs.set_word(REGISTER(inst.first), regs[REGISTER(inst.second)]);

  return UpdatePC;
}

Update swap(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] << 16 |
                regs[REGISTER(inst.second)] >> 16);

  return UpdatePC;
}


/* Stack operations */
Update push(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  // Save the register in the top of the stack
  regs.set_word(REGISTER_STP, regs[REGISTER(inst.first)]);
  // Update stack pointer
  regs.set_word(REGISTER_STP, regs[REGISTER_STP] - 4);

  return UpdatePC;
}

Update pop(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  // Update stack pointer
  regs.set_word(REGISTER_STP, regs[REGISTER_STP] + 4);
  // Restore the register
  regs.set_word(REGISTER(inst.first), regs[REGISTER_STP]);

  return UpdatePC;
}


/* Load operations */
Update load(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  regs.set_word(REGISTER(inst.first), mem[inst.address]);

  return UpdatePC;
}

Update loadi(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  regs.set_word(REGISTER(inst.first), inst.address);

  return UpdatePC;
}

Update loadrr(Memory& regs, Memory& mem, Interrupt& interrupt,
	      Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                mem[regs[REGISTER(inst.second)] +
                    regs[REGISTER(inst.address)]]);

  return UpdatePC;
}

Update loadri(Memory& regs, Memory& mem, Interrupt& interrupt,
	      Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                mem[regs[REGISTER(inst.second)] + inst.address]);

  return UpdatePC;
}


/* Store operations */
Update store(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  mem.set_word(inst.address, regs[REGISTER(inst.first)]);

  return UpdatePC;
}

Update storerr(Memory& regs, Memory& mem, Interrupt& interrupt,
	       Instruction inst)
{
  mem.set_word(regs[inst.first] + regs[inst.address], regs[inst.second]);

  return UpdatePC;
}

Update storeri(Memory& regs, Memory& mem, Interrupt& interrupt,
	       Instruction inst)
{
  mem.set_word(regs[inst.first] + inst.address, regs[inst.second]);

  return UpdatePC;
}

}
}
