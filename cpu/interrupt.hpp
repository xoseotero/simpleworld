/**
 * @file cpu/interrupt.hpp
 * Definition of the interrupts and events.
 *
 * begin:     Mon, 18 Jun 2007 04:51:55 +0200
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2007, Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __CPU_INTERRUPT_H__
#define __CPU_INTERRUPT_H__

#include <cpu/memory.hpp>

namespace SimpleWorld
{
namespace CPU
{

/**
 * Types of interrupts.
 */
enum InterruptType {
  InvalidInstruction = 0,
  InvalidMemoryLocation = 1,
  DivisionByZero = 2,
  TimerInterrupt = 3,
  InvalidWorldCommand = 4,
  WorldEvent = 5,
  SoftwareInterrupt = 6
};


/**
 * Types of events.
 */
enum EventType {
  Birth = 0,
  Attack = 1
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

#endif // __CPU_INTERRUPT_H__
