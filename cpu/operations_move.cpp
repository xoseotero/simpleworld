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
move::move(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update move::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4, this->regs_[inst.second * 4]);

  return UpdatePC;
}


swap::swap(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update swap::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       this->regs_[inst.second * 4] << 16 |
		       this->regs_[inst.second * 4] >> 16);

  return UpdatePC;
}


/* Stack operations */
push::push(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update push::operator()(Instruction inst)
{
  // Save the register in the top of the stack
  this->regs_.set_word(REGISTER_STP, this->regs_[inst.first * 4]);
  // Update stack pointer
  this->regs_.set_word(REGISTER_STP, this->regs_[REGISTER_STP] - 4);

  return UpdatePC;
}


pop::pop(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update pop::operator()(Instruction inst)
{
  // Update stack pointer
  this->regs_.set_word(REGISTER_STP, this->regs_[REGISTER_STP] + 4);
  // Restore the register
  this->regs_.set_word(inst.first * 4, this->regs_[REGISTER_STP]);

  return UpdatePC;
}


/* Load operations */
load::load(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update load::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       this->mem_[(this->regs_[REGISTER_SGP] << 16) +
				  inst.address]);

  return UpdatePC;
}


loadi::loadi(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update loadi::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4, inst.address);

  return UpdatePC;
}


loadrr::loadrr(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update loadrr::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       this->mem_[this->regs_[inst.second * 4] +
				  this->regs_[inst.address * 4]]);

  return UpdatePC;
}


loadri::loadri(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update loadri::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       this->mem_[this->regs_[inst.second * 4] +
				  inst.address]);

  return UpdatePC;
}


/* Store operations */
store::store(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update store::operator()(Instruction inst)
{
  this->mem_.set_word((this->regs_[REGISTER_SGP] << 16) + inst.address,
		      this->regs_[inst.first * 4]);

  return UpdatePC;
}


storerr::storerr(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update storerr::operator()(Instruction inst)
{
  this->mem_.set_word(this->regs_[inst.first] + this->regs_[inst.address],
		      this->regs_[inst.second]);

  return UpdatePC;
}


storeri::storeri(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update storeri::operator()(Instruction inst)
{
  this->mem_.set_word(this->regs_[inst.first] + inst.address,
		      this->regs_[inst.second]);

  return UpdatePC;
}

} // namespace CPU
} // namespace SimpleWorld
