/**
 * @file simpleworld/cpu/interrupt.hpp
 * Structure of a interrupt.
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

#ifndef SIMPLEWORLD_CPU_INTERRUPT_HPP
#define SIMPLEWORLD_CPU_INTERRUPT_HPP

#include <simpleworld/cpu/memory.hpp>

namespace SimpleWorld
{
namespace CPU
{

/**
 * Structure of a interrupt.
 */
struct Interrupt
{
  Uint8 code;                   /**< Code of the interrupt */

  Word r0;                      /**< Word stored in r0 when the interrupt
                                     handler is executed */
  Word r1;                      /**< Word stored in r1 when the interrupt
                                     handler is executed */
  Word r2;                      /**< Word stored in r2 when the interrupt
                                     handler is executed */
};

}
}

#endif // SIMPLEWORLD_CPU_INTERRUPT_HPP
