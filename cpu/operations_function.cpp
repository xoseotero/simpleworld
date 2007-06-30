/**
 * @file cpu/operations_function.cpp
 * Function operations of the Simple CPU.
 *
 * begin:     Sat, 11 Nov 2006 19:15:19 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2006, Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  License: See COPYING file that comes with this distribution
 */

#include "operations.hpp"
#include "interrupt.hpp"

#include <iostream>

namespace SimpleWorld
{
namespace CPU
{

Update call(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  // Save the program counter (pc) in the top of the stack
  mem.set_word(regs[REGISTER_STP], regs[REGISTER_PC]);
  // Update stack pointer
  regs.set_word(REGISTER_STP, regs[REGISTER_STP] - 4);
  // Execute the function
  regs.set_word(REGISTER_PC, inst.address);

  return None;
}

Update interrupt(Memory& regs, Memory& mem, Interrupt& interrupt,
		 Instruction inst)
{
  interrupt.type = SoftwareInterrupt;
  interrupt.r0 = static_cast<Word>(SoftwareInterrupt);
  interrupt.r1 = inst.address;
  return UpdateInterrupt;
}

Update ret(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  // Update stack pointer
  regs.set_word(REGISTER_STP, regs[REGISTER_STP] + 4);
  // Restore the program counter
  regs.set_word(REGISTER_PC, mem[regs[REGISTER_STP]]);

  return UpdatePC;
}

Update reti(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  Sint8 i;
  for (i = 15; i >= 0; i--) {
    // Restore a register:
    // Update stack pointer
    regs.set_word(REGISTER_STP, regs[REGISTER_STP] + 4);
    // Restore the register
    regs.set_word(REGISTER(i), regs[REGISTER_STP]);
  }

  return UpdatePC;
}

}
}
