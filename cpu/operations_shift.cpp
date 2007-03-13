/**
 * @file cpu/operations_shift.cpp
 * Shift operations of the Simple CPU.
 *
 * begin:     Sat, 11 Nov 2006 19:15:19 +0100
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2006, Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  License: See COPYING file that comes with this distribution
 */

#include "../simple/config.h"
#include "operations.h"

namespace SimpleWorld
{
namespace CPU
{

sll::sll(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update sll::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       this->regs_[inst.second * 4] <<
		       this->regs_[inst.address * 4]);

  return UpdatePC;
}


slli::slli(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update slli::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       this->regs_[inst.second * 4] << inst.address);

  return UpdatePC;
}


srl::srl(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update srl::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       this->regs_[inst.second * 4] >>
		       this->regs_[inst.address * 4]);

  return UpdatePC;
}


srli::srli(Memory& regs, Memory& mem)
  : Operator(regs, mem)
  {
}

Update srli::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       this->regs_[inst.second * 4] >> inst.address);

  return UpdatePC;
}


sla::sla(Memory& regs, Memory& mem)
  : Operator(regs, mem)
  {
}

Update sla::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       this->regs_[inst.second * 4] <<
		       this->regs_[inst.address * 4]);

  return UpdatePC;
}


slai::slai(Memory& regs, Memory& mem)
  : Operator(regs, mem)
  {
}

Update slai::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       this->regs_[inst.second * 4] << inst.address);

  return UpdatePC;
}


sra::sra(Memory& regs, Memory& mem)
  : Operator(regs, mem)
{
}

Update sra::operator()(Instruction inst)
{
#ifdef IS_BIG_ENDIAN
  Uint32 sign = this->regs_[inst.second * 4] & 0x80000000;
#else
  Uint32 sign = this->regs_[inst.second * 4] & 0x00000001;
#endif
  this->regs_.set_word(inst.first * 4,
		       (this->regs_[inst.second * 4] >>
			this->regs_[inst.address * 4]) | sign);

  return UpdatePC;
}


srai::srai(Memory& regs, Memory& mem)
  : Operator(regs, mem)
{
}

Update srai::operator()(Instruction inst)
{
#ifdef IS_BIG_ENDIAN
  Uint32 sign = this->regs_[inst.second * 4] & 0x80000000;
#else
  Uint32 sign = this->regs_[inst.second * 4] & 0x00000001;
#endif
  this->regs_.set_word(inst.first * 4,
		       (this->regs_[inst.second * 4] >> inst.address) | sign);

  return UpdatePC;
}


rl::rl(Memory& regs, Memory& mem)
  : Operator(regs, mem)
{
}

Update rl::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       (this->regs_[inst.second * 4] <<
			this->regs_[inst.address * 4]) |
		       (this->regs_[inst.second * 4] >>
			(32 - (this->regs_[inst.address * 4] % 32))));

  return UpdatePC;
}


rli::rli(Memory& regs, Memory& mem)
  : Operator(regs, mem)
{
}

Update rli::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       (this->regs_[inst.second * 4] << inst.address) |
		       (this->regs_[inst.second * 4] >>
			(32 - (inst.address % 32))));

  return UpdatePC;
}


rr::rr(Memory& regs, Memory& mem)
  : Operator(regs, mem)
{
}

Update rr::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       (this->regs_[inst.second * 4] >>
			this->regs_[inst.address * 4]) |
		       (this->regs_[inst.second * 4] <<
			(32 - (this->regs_[inst.address * 4] % 32))));

  return UpdatePC;
}


rri::rri(Memory& regs, Memory& mem)
  : Operator(regs, mem)
{
}

Update rri::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       (this->regs_[inst.second * 4] >> inst.address) |
		       (this->regs_[inst.second * 4] <<
			(32 - (inst.address % 32))));

  return UpdatePC;
}

} // namespace CPU
} // namespace SimpleWorld
