/**
 * @file simpleworld/isa.hpp
 * Instruction set architecture.
 *
 *  Copyright (C) 2013  Xosé Otero <xoseotero@gmail.com>
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

#ifndef SIMPLEWORLD_ISA_HPP
#define SIMPLEWORLD_ISA_HPP

#include <simpleworld/cpu/isa.hpp>


#define INTERRUPT_WORLDACTION (0x5)
#define INTERRUPT_WORLDEVENT (0x6)


namespace simpleworld
{

/**
 * Instruction set architecture.
 */
class ISA: public cpu::ISA
{
public:
  /**
   * Constructor.
   */
  ISA();
};


/**
 * Global variable with the ISA.
 */
extern const ISA isa;

}

#endif // SIMPLEWORLD_ISA_HPP
