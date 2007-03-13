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

#include "operations.h"

namespace SimpleWorld
{
namespace CPU
{

b::b(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update b::operator()(Instruction inst)
{
  this->regs_.set_word(REGISTER_PC,
		       (this->regs_[REGISTER_SGP] << 16) + inst.address);

  return None;
}


beq::beq(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update beq::operator()(Instruction inst)
{
  if (this->regs_[inst.first * 4] == this->regs_[inst.second * 4]) {
    this->regs_.set_word(REGISTER_PC,
			 (this->regs_[REGISTER_SGP] << 16) + inst.address);
    return None;
  }

  return UpdatePC;
}


bne::bne(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update bne::operator()(Instruction inst)
{
  if (this->regs_[inst.first * 4] != this->regs_[inst.second * 4]) {
    this->regs_.set_word(REGISTER_PC,
			 (this->regs_[REGISTER_SGP] << 16) + inst.address);
    return None;
  }

  return UpdatePC;
}


blt::blt(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update blt::operator()(Instruction inst)
{
  if (static_cast<Sint32>(this->regs_[inst.first * 4]) <
      static_cast<Sint32>(this->regs_[inst.second * 4])) {
    this->regs_.set_word(REGISTER_PC,
			 (this->regs_[REGISTER_SGP] << 16) + inst.address);
    return None;
  }

  return UpdatePC;
}


bltu::bltu(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update bltu::operator()(Instruction inst)
{
  if (this->regs_[inst.first * 4] < this->regs_[inst.second * 4]) {
    this->regs_.set_word(REGISTER_PC,
			 (this->regs_[REGISTER_SGP] << 16) + inst.address);
    return None;
  }

  return UpdatePC;
}


bgt::bgt(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update bgt::operator()(Instruction inst)
{
  if (static_cast<Sint32>(this->regs_[inst.first * 4]) >
      static_cast<Sint32>(this->regs_[inst.second * 4])) {
    this->regs_.set_word(REGISTER_PC,
			 (this->regs_[REGISTER_SGP] << 16) + inst.address);
    return None;
  }

  return UpdatePC;
}


bgtu::bgtu(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update bgtu::operator()(Instruction inst)
{
  if (this->regs_[inst.first * 4] > this->regs_[inst.second * 4]) {
    this->regs_.set_word(REGISTER_PC,
			 (this->regs_[REGISTER_SGP] << 16) + inst.address);
    return None;
  }

  return UpdatePC;
}


ble::ble(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update ble::operator()(Instruction inst)
{
  if (static_cast<Sint32>(this->regs_[inst.first * 4]) <=
      static_cast<Sint32>(this->regs_[inst.second * 4])) {
    this->regs_.set_word(REGISTER_PC,
			 (this->regs_[REGISTER_SGP] << 16) + inst.address);
    return None;
  }

  return UpdatePC;
}


bleu::bleu(Memory& regs, Memory& mem)
  : Operator(regs, mem)
{
}

Update bleu::operator()(Instruction inst)
{
  if (this->regs_[inst.first * 4] <= this->regs_[inst.second * 4]) {
    this->regs_.set_word(REGISTER_PC,
			 (this->regs_[REGISTER_SGP] << 16) + inst.address);
    return None;
  }

  return UpdatePC;
}


bge::bge(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update bge::operator()(Instruction inst)
{
  if (static_cast<Sint32>(this->regs_[inst.first * 4]) >=
      static_cast<Sint32>(this->regs_[inst.second * 4])) {
    this->regs_.set_word(REGISTER_PC,
			 (this->regs_[REGISTER_SGP] << 16) + inst.address);
    return None;
  }

  return UpdatePC;
}


bgeu::bgeu(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update bgeu::operator()(Instruction inst)
{
  if (this->regs_[inst.first * 4] >= this->regs_[inst.second * 4]) {
    this->regs_.set_word(REGISTER_PC,
			 (this->regs_[REGISTER_SGP] << 16) + inst.address);
    return None;
  }

  return UpdatePC;
}

} // namespace CPU
} // namespace SimpleWorld
