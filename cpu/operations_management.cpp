/**
 * @file cpu/operations_management.cpp
 * Stop/restart operations of the Simple CPU.
 *
 * begin:     Xov Nov 30 2006
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

stop::stop(Memory& regs, Memory& mem)
  : Operation(regs, mem)
{
}

Update stop::operator()(Instruction inst)
{
  return Stop;
}


restart::restart(Memory& regs, Memory& mem)
  : Operator(regs, mem)
{
}

Update restart::operator()(Instruction inst)
{
  Uint8 i;
  for (i = 0; i < 16; i++)
    this->regs_.set_word(i * 4, 0);

  return None;
}

} // namespace CPU
} // namespace SimpleWorld
