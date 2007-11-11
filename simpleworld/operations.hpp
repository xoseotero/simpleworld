/**
 * @file simpleworld/operations.hpp
 * World operation of the Simple CPU.
 *
 * begin:     Mon, 27 Aug 2007 22:37:53 +0200
 * last:      $Date$
 *
 *  Copyright (C) 2007  Xosé Antón Otero Ferreira <xoseotero@gmail.com>
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

#ifndef __SIMPLEWORLD_OPERATIONS_HPP__
#define __SIMPLEWORLD_OPERATIONS_HPP__

#include <cpu/types.hpp>
#include <cpu/memory.hpp>
#include <cpu/interrupt.hpp>
#include <cpu/instruction.hpp>

#define REGISTER(NUMBER) (NUMBER * sizeof(CPU::Word))
#define REGISTER_PC REGISTER(0xd)
#define REGISTER_STP REGISTER(0xe)
#define REGISTER_ITP REGISTER(0xf)

namespace SimpleWorld
{

/**
 * Make a action in the world.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
CPU::Update world(CPU::Memory& regs, CPU::Memory& mem,
                  CPU::Interrupt& interrupt, CPU::Instruction inst);

}

#endif // __SIMPLEWORLD_OPERATIONS_HPP__