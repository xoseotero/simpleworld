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

Update lnot(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange)
{
  regs.set_word(inst.first * 4, ~ regs[inst.second * 4]);

  return UpdatePC;
}

Update lor(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange)
{
  regs.set_word(inst.first * 4, regs[inst.second * 4] | regs[inst.address * 4]);

  return UpdatePC;
}

Update lori(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange)
{
  regs.set_word(inst.first * 4, regs[inst.second * 4] | inst.address);

  return UpdatePC;
}

Update land(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange)
{
  regs.set_word(inst.first * 4, regs[inst.second * 4] & regs[inst.address * 4]);

  return UpdatePC;
}

Update landi(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange)
{
  regs.set_word(inst.first * 4, regs[inst.second * 4] & inst.address);

  return UpdatePC;
}

Update lxor(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange)
{
  regs.set_word(inst.first * 4, regs[inst.second * 4] ^ regs[inst.address * 4]);

  return UpdatePC;
}

Update lxori(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange)
{
  regs.set_word(inst.first * 4, regs[inst.second * 4] ^ inst.address);

  return UpdatePC;
}

} // namespace CPU
} // namespace SimpleWorld
