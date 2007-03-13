/**
 * @file cpu/operations_arithmetic.cpp
 * Arithmetic operations of the Simple CPU.
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

#ifdef IS_BIG_ENDIAN
#define HIGHBITS_64BITS SINT64(0xFFFFFFFF00000000)
#define LOWBITS_64BITS SINT64(0x00000000FFFFFFFF)
#else
#define HIGHBITS_64BITS SINT64(0x00000000FFFFFFFF)
#define LOWBITS_64BITS SINT64(0xFFFFFFFF00000000)
#endif

namespace SimpleWorld
{
namespace CPU
{

add::add(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update add::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       this->regs_[inst.second * 4] +
		       this->regs_[inst.address * 4]);

  return UpdatePC;
}


addi::addi(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update addi::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       this->regs_[inst.second * 4] + inst.address);

  return UpdatePC;
}


sub::sub(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update sub::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4, this->regs_[inst.second * 4] -
		       this->regs_[inst.address * 4]);

  return UpdatePC;
}


subi::subi(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update subi::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       this->regs_[inst.second * 4] - inst.address);

  return UpdatePC;
}


multl::multl(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update multl::operator()(Instruction inst)
{
  Sint64 result = static_cast<Sint32>(this->regs_[inst.second * 4]) *
    static_cast<Sint32>(this->regs_[inst.address * 4]);
  this->regs_.set_word(inst.first * 4,
		       static_cast<Sint32>(result & LOWBITS_64BITS));

  return UpdatePC;
}


multli::multli(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update multli::operator()(Instruction inst)
{
  Sint64 result = static_cast<Sint32>(this->regs_[inst.second * 4]) *
    static_cast<Sint32>(inst.address);
  this->regs_.set_word(inst.first * 4,
		       static_cast<Sint32>(result & LOWBITS_64BITS));

  return UpdatePC;
}


multlu::multlu(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update multlu::operator()(Instruction inst)
{
  Uint64 result = this->regs_[inst.second * 4] * this->regs_[inst.address * 4];
  this->regs_.set_word(inst.first * 4,
		       static_cast<Uint32>(result & LOWBITS_64BITS));

  return UpdatePC;
}


multlui::multlui(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update multlui::operator()(Instruction inst)
{
  Uint64 result = this->regs_[inst.second * 4] *
    static_cast<Uint32>(inst.address);
  this->regs_.set_word(inst.first * 4,
		       static_cast<Uint32>(result & LOWBITS_64BITS));

  return UpdatePC;
}


multh::multh(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update multh::operator()(Instruction inst)
{
  Sint64 result = static_cast<Sint32>(this->regs_[inst.second * 4]) *
    static_cast<Sint32>(this->regs_[inst.address * 4]);
  this->regs_.set_word(inst.first * 4,
		       static_cast<Sint32>((result & HIGHBITS_64BITS) >> 32));

  return UpdatePC;
}


multhi::multhi(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update multhi::operator()(Instruction inst)
{
  Sint64 result = static_cast<Sint32>(this->regs_[inst.second * 4]) *
    static_cast<Sint32>(inst.address);
  this->regs_.set_word(inst.first * 4,
		       static_cast<Sint32>((result & HIGHBITS_64BITS) >> 32));

  return UpdatePC;
}


multhu::multhu(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update multhu::operator()(Instruction inst)
{
  Uint64 result = this->regs_[inst.second * 4] * this->regs_[inst.address * 4];
  this->regs_.set_word(inst.first * 4,
		       static_cast<Uint32>((result & HIGHBITS_64BITS) >> 32));

  return UpdatePC;
}


multhui::multhui(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update multhui::operator()(Instruction inst)
{
  Uint64 result = this->regs_[inst.second * 4] * inst.address;
  this->regs_.set_word(inst.first * 4,
		       static_cast<Uint32>((result & HIGHBITS_64BITS) >> 32));

  return UpdatePC;
}


div::div(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update div::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       static_cast<Sint32>(this->regs_[inst.second * 4]) /
		       static_cast<Sint32>(this->regs_[inst.address * 4]));

  return UpdatePC;
}


divi::divi(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update divi::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       static_cast<Sint32>(this->regs_[inst.second * 4]) /
		       static_cast<Sint32>(inst.address));

  return UpdatePC;
}


divu::divu(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update divu::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       this->regs_[inst.second * 4] /
		       this->regs_[inst.address * 4]);

  return UpdatePC;
}


divui::divui(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update divui::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       this->regs_[inst.second * 4] / inst.address);

  return UpdatePC;
}


mod::mod(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update mod::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       static_cast<Sint32>(this->regs_[inst.second * 4]) %
		       static_cast<Sint32>(this->regs_[inst.address * 4]));

  return UpdatePC;
}


modi::modi(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update modi::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       static_cast<Sint32>(this->regs_[inst.second * 4]) %
		       static_cast<Sint32>(inst.address));

  return UpdatePC;
}


modu::modu(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update modu::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       this->regs_[inst.second * 4] %
		       this->regs_[inst.address * 4]);

  return UpdatePC;
}


modui::modui(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update modui::operator()(Instruction inst)
{
  this->regs_.set_word(inst.first * 4,
		       this->regs_[inst.second * 4] % inst.address);

  return UpdatePC;
}

} // namespace CPU
} // namespace SimpleWorld
