/**
 * @file simpleworld/cpu/operations_logic.cpp
 * Logic operations of the Simple CPU.
 *
 * begin:     Sat, 11 Nov 2006 19:15:19 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2006-2007  Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "operations.hpp"

namespace SimpleWorld
{
namespace CPU
{

Update lnot(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  regs.set_word(REGISTER(inst.first), ~ regs[REGISTER(inst.second)]);

  return UpdatePC;
}

Update lor(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] | regs[REGISTER(inst.address)]);

  return UpdatePC;
}

Update lori(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] | inst.address);

  return UpdatePC;
}

Update land(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] & regs[REGISTER(inst.address)]);

  return UpdatePC;
}

Update landi(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
             Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] & inst.address);

  return UpdatePC;
}

Update lxor(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] ^ regs[REGISTER(inst.address)]);

  return UpdatePC;
}

Update lxori(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
             Instruction inst)
{
  regs.set_word(REGISTER(inst.first),
                regs[REGISTER(inst.second)] ^ inst.address);

  return UpdatePC;
}

}
}
