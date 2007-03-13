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

#include "operations.h"

namespace SimpleWorld
{
namespace CPU
{

lnot::lnot(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update lnot::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4, ~ this->regs_[inst.second * 4]);

  return UpdatePC;
}


lor::lor(Memory& regs, Memory& mem)
  : Operator(regs, mem)
{
}

Update lor::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       this->regs_[inst.second * 4] |
		       this->regs_[inst.address * 4]);

  return UpdatePC;
}


lori::lori(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update lori::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       this->regs_[inst.second * 4] | inst.address);

  return UpdatePC;
}


land::land(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update land::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       this->regs_[inst.second * 4] &
		       this->regs_[inst.address * 4]);

  return UpdatePC;
}


landi::landi(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update landi::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       this->regs_[inst.second * 4] & inst.address);

  return UpdatePC;
}


lxor::lxor(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update lxor::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       this->regs_[inst.second * 4] ^
		       this->regs_[inst.address * 4]);

  return UpdatePC;
}


lxori::lxori(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update lxori::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       this->regs_[inst.second * 4] ^ inst.address);

  return UpdatePC;
}

} // namespace CPU
} // namespace SimpleWorld
