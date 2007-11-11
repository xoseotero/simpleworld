/**
 * @file cpu/interrupt.hpp
 * Definition of the interrupts and events.
 *
 * begin:     Mon, 18 Jun 2007 04:51:55 +0200
 * last:      $Date$
 *
 *  Copyright (C) 2007  Xosé Otero <xoseotero@users.sourceforge.net>
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

#ifndef __CPU_INTERRUPT_HPP__
#define __CPU_INTERRUPT_HPP__

#include <cpu/memory.hpp>

namespace SimpleWorld
{
namespace CPU
{

/**
 * Types of interrupts.
 */
enum InterruptType {
  TimerInterrupt = 0,
  SoftwareInterrupt = 1,
  InvalidInstruction = 2,
  InvalidMemoryLocation = 3,
  DivisionByZero = 4,
  InvalidWorldCommand = 5,
  WorldEvent = 6
};


/**
 * Types of events.
 */
enum EventType {
  Attack = 0
};


/**
 *
 */
class Interrupt
{
public:
  InterruptType type;

  Word r0;
  Word r1;
  Word r2;
};

}
}

#endif // __CPU_INTERRUPT_HPP__
