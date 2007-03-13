/**
 * @file cpu/operations_function.cpp
 * Function operations of the Simple CPU.
 *
 * begin:     Sat, 11 Nov 2006 19:15:19 +0100
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2006, Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  License: See COPYING file that comes with this distribution
 */

#include "operations.hpp"

#include <iostream>

namespace SimpleWorld
{
namespace CPU
{

Update call(Memory& regs, Memory& mem, Instruction inst)
{
  // Save the program counter (pc) in the top of the stack
  mem.set_word(regs[REGISTER_STP], regs[REGISTER_PC]);
  // Update stack pointer
  regs.set_word(REGISTER_STP, regs[REGISTER_STP] - 4);
  // Execute the function
  regs.set_word(REGISTER_PC, (regs[REGISTER_SGP] << 16) + inst.address);

  return None;
}

Update ret(Memory& regs, Memory& mem, Instruction inst)
{
  // Update stack pointer
  regs.set_word(REGISTER_STP, regs[REGISTER_STP] + 4);
  // Restore the program counter
  regs.set_word(REGISTER_PC, mem[regs[REGISTER_STP]]);

  return UpdatePC;
}

Update rete(Memory& regs, Memory& mem, Instruction inst)
{
  Sint8 i;
  for (i = 15; i >= 0; i--) {
    // Restore a register:
    // Update stack pointer
    regs.set_word(REGISTER_STP, regs[REGISTER_STP] + 4);
    // Restore the register
    regs.set_word(i * 4, mem[regs[REGISTER_STP]]);
  }

  return UpdatePC;
}

#warning World operation not implemented
Update world(Memory& regs, Memory& mem, Instruction inst)
{
  std::cout << "Calling world with parameter " << inst.address << std::endl;

  return UpdatePC;
}

} // namespace CPU
} // namespace SimpleWorld
