/**
 * @file cpu/operation.cpp
 * Base class for all the operation of the Simple CPU.
 *
 * begin:     Tue, 13 Mar 2007 16:46:41 +0100
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2006, Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  License: See COPYING file that comes with this distribution
 */

#include "operation.h"

namespace SimpleWorld
{
namespace CPU
{

Operation::Operation(Memory& regs, Memory& mem)
  : regs_(regs), mem_(mem)
{
}

Update Operation::operator()(Instruction inst)
{
  throw NotSupported;
}

}
}
