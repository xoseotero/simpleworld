/**
 * @file simpleworld/cpu/word.cpp
 * Access bytes in a half word/word and change the byte ordering.
 *
 *  Copyright (C) 2004-2013  Xosé Otero <xoseotero@gmail.com>
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
void set_byte(HalfWord* hword, Uint8 byte, QuarterWord value)
{
  if (byte > 1)
    throw EXCEPTION(CPUException, boost::str(boost::format("\
Byte 0x%02X is out of range")
                                             % byte));

  reinterpret_cast<QuarterWord*>(hword)[byte] = value;
}


// Use the swap functions provided by the compiler
#if defined(__GNUC__)
#define SWAP32(x) __builtin_bswap32(x)
#define SWAP16(x) __builtin_bswap16(x)

#elif defined(_MSC_VER)
#include <stdlib.h>
#define SWAP32(x) _byteswap_ulong(x)
#define SWAP16(x) _byteswap_ushort(x)

// Use the swap functions provided by the operating system
#elif defined(__gnu_linux__)
#include <byteswap.h>
#define SWAP32(x) bswap_32(x)
#define SWAP16(x) bswap_16(x)

#elif defined(__FreeBSD__)
#include <sys/endian.h>
#define SWAP32(x) bswap32(x)
#define SWAP16(x) bswap16(x)

#elif defined(__APPLE__) && defined(__MACH__)
#include <libkern/OSByteOrder.h>
#define SWAP32(x) OSSwapInt32(x)
#define SWAP16(x) OSSwapInt16(x)

#else
#define SWAP32(x) ((((x) & 0xFF000000) >> 24) | \
		   (((x) & 0x00FF0000) >>  8) | \
		   (((x) & 0x0000FF00) <<  8) | \
		   (((x) & 0x000000FF) << 24))
#define SWAP16(x) ((((x) & 0xFF00) >> 8) |	\
		   (((x) & 0x00FF) << 8))

#endif

/**
 * Change the byte order of a word.
 * Big Endian <-> Little Endian.
 * @param word Word to change.
 * @return the word with the order changed.
 */
Word change_byte_order(Word word)
{
  return SWAP32(word);
}

/**
 * Change the byte order of a half word.
 * Big Endian <-> Little Endian.
 * @param hword Half word to change.
 * @return the half word with the order changed.
 */
HalfWord change_byte_order(HalfWord hword)
{
  return SWAP16(hword);
}

}
}
