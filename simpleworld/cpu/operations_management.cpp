/**
 * @file simpleworld/cpu/operations_management.cpp
 * Stop/restart operations of the Simple CPU.
 *
 * begin:     Xov Nov 30 2006
 * last:      $Date$
 *
 *  Copyright (C) 2006-2008  Xosé Otero <xoseotero@users.sourceforge.net>
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

namespace simpleworld
{
namespace cpu
{

/**
 * Stop the CPU.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update stop(CPU& cpu, Instruction inst)
{
  return UpdateStop;
}

/**
 * Restart the CPU (zeroing the registers).
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update restart(CPU& cpu, Instruction inst)
{
  for (Uint8 i = 0; i < 16; i++)
    cpu.set_reg(i, 0);

  return UpdateNone;
}

}
}
