/**
 * @file simpleworld/cpu.hpp
 * A CPU in Simple World.
 *
 *  Copyright (C) 2007-2010  Xosé Otero <xoseotero@gmail.com>
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

#ifndef SIMPLEWORLD_CPU_HPP
#define SIMPLEWORLD_CPU_HPP

#include <simpleworld/cpu/memory.hpp>
#include <simpleworld/cpu/cpu.hpp>

#define INTERRUPT_WORLDACTION (0x5)
#define INTERRUPT_WORLDEVENT (0x6)

namespace simpleworld
{

class Bug;

class CPU: public cpu::CPU
{
public:
  /**
   * Constructor.
   * @param registers registers of the CPU.
   * @param memory memory of the CPU.
   * @param bug bug owner of the CPU.
   */
  CPU(cpu::Memory* registers, cpu::Memory* memory, Bug* bug);


  /**
  * Execute the next instruction.
  * @exception CPUException A stop instruction was found
  */
  void next();


  Bug* bug;
};

}

#endif // SIMPLEWORLD_CPU_HPP
