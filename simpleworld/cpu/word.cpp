/**
 * @file simpleworld/cpu/word.cpp
 * Access bytes in a half word/word and change the byte ordering.
 *
 *  Copyright (C) 2004-2010  Xosé Otero <xoseotero@users.sourceforge.net>
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

#include <boost/format.hpp>

#include <simpleworld/config.hpp>

#include "word.hpp"
#include "exception.hpp"

namespace simpleworld
{
namespace cpu
{

/**
 * Return the value of a byte in a word.
 * @param word Word.
 * @param byte number of byte to get.
 * @return the byte value.
 * @exception CPUException if byte > 3
 */
QuarterWord get_byte(Word word, Uint8 byte)
{
  if (byte > 3)
    throw EXCEPTION(CPUException, boost::str(boost::format("\
Byte 0x%02X is out of range")
                                             % byte));

  return reinterpret_cast<QuarterWord*>(&word)[byte];
}

/**
 * Return the value of a byte in a half word.
 * @param hword Half word.
 * @param byte number of byte to get.
 * @return the byte value.
 * @exception CPUException if byte > 1
 */
QuarterWord get_byte(HalfWord hword, Uint8 byte)
{
  if (byte > 1)
    throw EXCEPTION(CPUException, boost::str(boost::format("\
Byte 0x%02X is out of range")
                                             % byte));

  return reinterpret_cast<QuarterWord*>(&hword)[byte];
}


/**
 * Set the value of a byte in a word.
 * @param word Word.
 * @param byte Number of byte to change.
 * @param value New value for the byte.
 * @return the byte value.
 * @exception CPUException if byte > 3
 */
void set_byte(Word* word, Uint8 byte, QuarterWord value)
{
  if (byte > 3)
    throw EXCEPTION(CPUException, boost::str(boost::format("\
Byte 0x%02X is out of range")
                                             % byte));

  reinterpret_cast<QuarterWord*>(word)[byte] = value;
}

/**
 * Set the value of a byte in a half word.
 * @param hword Half word.
 * @param byte Number of byte to change.
 * @param value New value for the byte.
 * @return the byte value.
 * @exception CPUException if byte > 1
 */
void set_byte(HalfWord* word, Uint8 byte, QuarterWord value)
{
  if (byte > 1)
    throw EXCEPTION(CPUException, boost::str(boost::format("\
Byte 0x%02X is out of range")
                                             % byte));

  reinterpret_cast<QuarterWord*>(word)[byte] = value;
}


/**
 * Change the byte order of a word.
 * Big Endian <-> Little Endian.
 * @param word Word to change.
 * @return the word with the order changed.
 */
Word change_byte_order(Word word)
{
#if defined(IS_BIG_ENDIAN)
  return get_byte(word, 0) | get_byte(word, 1) << 8 |
    get_byte(word, 2) << 16 | get_byte(word, 3) << 24;
#elif defined(IS_LITTLE_ENDIAN)
  return get_byte(word, 0) << 24 | get_byte(word, 1) << 16 |
    get_byte(word, 2) << 8 | get_byte(word, 3);
#else
#error endianness not specified
#endif
}

/**
 * Change the byte order of a half word.
 * Big Endian <-> Little Endian.
 * @param hword Half word to change.
 * @return the half word with the order changed.
 */
HalfWord change_byte_order(HalfWord hword)
{
#if defined(IS_BIG_ENDIAN)
  return get_byte(hword, 0) | get_byte(hword, 1) << 8;
#elif defined(IS_LITTLE_ENDIAN)
  return get_byte(hword, 0) << 8 | get_byte(hword, 1);
#else
#error endianness not specified
#endif
}

}
}
