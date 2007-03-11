/**
 * @file cpu/operations_move.cpp
 * Load/store/move/stack operations of the Simple CPU.
 *
 * begin:     Sat, 11 Nov 2006 19:15:19 +0100
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2006, Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  License: See COPYING file that comes with this distribution
 */

#include "../simple/types.h"
#include "operations.h"

namespace SimpleWorld
{
namespace CPU
{

/* Move operations */
Update move(Memory& regs, Memory& mem, Instruction inst)
{
  regs.set_word(inst.first * 4, regs[inst.second * 4]);

  return UpdatePC;
}

Update swap(Memory& regs, Memory& mem, Instruction inst)
{
  regs.set_word(inst.first * 4,
                regs[inst.second * 4] << 16 | regs[inst.second * 4] >> 16);

  return UpdatePC;
}


/* Stack operations */
Update push(Memory& regs, Memory& mem, Instruction inst)
{
  // Save the register in the top of the stack
  regs.set_word(REGISTER_STP, regs[inst.first * 4]);
  // Update stack pointer
  regs.set_word(REGISTER_STP, regs[REGISTER_STP] - 4);

  return UpdatePC;
}

Update pop(Memory& regs, Memory& mem, Instruction inst)
{
  // Update stack pointer
  regs.set_word(REGISTER_STP, regs[REGISTER_STP] + 4);
  // Restore the register
  regs.set_word(inst.first * 4, regs[REGISTER_STP]);

  return UpdatePC;
}


/* Load operations */
Update load(Memory& regs, Memory& mem, Instruction inst)
{
  regs.set_word(inst.first * 4, mem[(regs[REGISTER_SGP] << 16) + inst.address]);

  return UpdatePC;
}

Update loadi(Memory& regs, Memory& mem, Instruction inst)
{
  regs.set_word(inst.first * 4, inst.address);

  return UpdatePC;
}

Update loadrr(Memory& regs, Memory& mem, Instruction inst)
{
  regs.set_word(inst.first * 4, mem[regs[inst.second * 4] +
                                    regs[inst.address * 4]]);

  return UpdatePC;
}

Update loadri(Memory& regs, Memory& mem, Instruction inst)
{
  regs.set_word(inst.first * 4, mem[regs[inst.second * 4] + inst.address]);

  return UpdatePC;
}


/* Store operations */
Update store(Memory& regs, Memory& mem, Instruction inst)
{
  mem.set_word((regs[REGISTER_SGP] << 16) + inst.address, regs[inst.first * 4]);

  return UpdatePC;
}

Update storerr(Memory& regs, Memory& mem, Instruction inst)
{
  mem.set_word(regs[inst.first] + regs[inst.address], regs[inst.second]);

  return UpdatePC;
}

Update storeri(Memory& regs, Memory& mem, Instruction inst)
{
  mem.set_word(regs[inst.first] + inst.address, regs[inst.second]);

  return UpdatePC;
}

} // namespace CPU
} // namespace SimpleWorld
