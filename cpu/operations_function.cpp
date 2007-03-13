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

#include "operations.h"

#include <iostream>

namespace SimpleWorld
{
namespace CPU
{

call::call(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update call::operator()(Instruction inst)
{
  // Save the program counter (pc) in the top of the stack
  this->mem_.set_word(this->regs_[REGISTER_STP], this->regs_[REGISTER_PC]);
  // Update stack pointer
  this->regs_.set_word(REGISTER_STP, this->regs_[REGISTER_STP] - 4);
  // Execute the function
  this->regs_.set_word(REGISTER_PC,
		       (this->regs_[REGISTER_SGP] << 16) + inst.address);

  return None;
}


ret::ret(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update ret::operator()(Instruction inst)
{
  // Update stack pointer
  this->regs_.set_word(REGISTER_STP, this->regs_[REGISTER_STP] + 4);
  // Restore the program counter
  this->regs_.set_word(REGISTER_PC, this->mem_[this->regs_[REGISTER_STP]]);

  return UpdatePC;
}


rete::rete(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update rete::operator()(Instruction inst)
{
  Sint8 i;
  for (i = 15; i >= 0; i--) {
    // Restore a register:
    // Update stack pointer
    this->regs_.set_word(REGISTER_STP, this->regs_[REGISTER_STP] + 4);
    // Restore the register
    this->regs_.set_word(i * 4, this->mem_[this->regs_[REGISTER_STP]]);
  }

  return UpdatePC;
}


#warning World operation not implemented
world::world(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update world::operator()(Instruction inst)
{
  std::cout << "Calling world with parameter " << inst.address << std::endl;

  return UpdatePC;
}

} // namespace CPU
} // namespace SimpleWorld
