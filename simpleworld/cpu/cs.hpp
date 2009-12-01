/**
 * @file simpleworld/cpu/cs.hpp
 * Control & Status register.
 *
 *  Copyright (C) 2008  Xosé Otero <xoseotero@users.sourceforge.net>
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

#ifndef SIMPLEWORLD_CPU_CS_HPP
#define SIMPLEWORLD_CPU_CS_HPP

#include <simpleworld/config.hpp>
#include <simpleworld/ints.hpp>
#include <simpleworld/cpu/types.hpp>

// flags for specific bits
#if defined(IS_BIG_ENDIAN)
#define ENABLE_FLAG    0x00000080
#define INTERRUPT_FLAG 0x00000040
#define MAXINTS_MASK   0x0000000F
#elif defined(IS_LITTLE_ENDIAN)
#define ENABLE_FLAG    0x80000000
#define INTERRUPT_FLAG 0x40000000
#define MAXINTS_MASK   0x0F000000
#else
#error endianness not specified
#endif

namespace simpleworld
{
namespace cpu
{

/**
 * Control & Status register.
 */
class CS
{
public:
  /**
   * Default constructor.
   */
  CS();

  /**
   * Constructor that decodes a word (in big endian).
   * @param word word to decode (in big endian).
   */
  CS(Word word);


  /**
   * Encode the data to a Word (in big endian).
   * @return the data encoded.
   */
  Word encode() const;

  /**
   * Decode the  (in big endian).
   * @param word word to decode (in big endian).
   */
  void decode(Word word);


  // data
  Uint16 itp;                   /**< Interrupt Table Pointer */

  bool enable;                  /**< Interrupt enabled */
  bool interrupt;               /**< Interrupt found */
  Uint8 max_interrupts:4;       /**< Maximun number of interrupts */
};

}
}

#endif // SIMPLEWORLD_CPU_CS_HPP
