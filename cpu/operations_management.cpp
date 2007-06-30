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

#include "operations.hpp"

namespace SimpleWorld
{
namespace CPU
{

Update stop(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst)
{
  return Stop;
}

Update restart(Memory& regs, Memory& mem, Interrupt& interrupt,
	       Instruction inst)
{
  Uint8 i;
  for (i = 0; i < 16; i++)
    regs.set_word(REGISTER(i), 0);

  return None;
}

}
}
