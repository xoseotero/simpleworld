/**
 * @file simpleworld/cpu/cs.cpp
 * Control & Status register.
 *
 *  Copyright (C) 2006-2007  Xosé Otero <xoseotero@gmail.com>
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

#include <simpleworld/config.hpp>

#include "types.hpp"
#include "word.hpp"
#include "cs.hpp"

namespace simpleworld
{
namespace cpu
{

/**
 * Default constructor.
 */
CS::CS()
{
}

/**
 * Constructor that decodes a word (in big endian).
 * @param word word to decode (in big endian).
 */
CS::CS(Word word)
{
  this->decode(word);
}


/**
 * Encode the data to a Word (in big endian).
 * @return the data encoded.
 */
Word CS::encode() const
{
  Word word = 0;

#if defined(IS_BIG_ENDIAN)
  set_byte(&word, 0, get_byte(static_cast<Word>(this->itp), 2));
  set_byte(&word, 1, get_byte(static_cast<Word>(this->itp), 3));
#elif defined(IS_LITTLE_ENDIAN)
  set_byte(&word, 0, get_byte(static_cast<Word>(this->itp), 1));
  set_byte(&word, 1, get_byte(static_cast<Word>(this->itp), 0));
#else
#error endianness not specified
#endif

  if (this->enable)
    word |= ENABLE_FLAG;
  if (this->interrupt)
    word |= INTERRUPT_FLAG;
#if defined(IS_BIG_ENDIAN)
  word |= this->max_interrupts;
#elif defined(IS_LITTLE_ENDIAN)
  word |= this->max_interrupts << 24;
#else
#error endianness not specified
#endif

  return word;
}

/**
 * Decode the  (in big endian).
 * @param word word to decode (in big endian).
 */
void CS::decode(Word word)
{
  Word itp = static_cast<Word>(this->itp);
#if defined(IS_BIG_ENDIAN)
  set_byte(&itp, 2, get_byte(word, 0));
  set_byte(&itp, 3, get_byte(word, 1));
#elif defined(IS_LITTLE_ENDIAN)
  set_byte(&itp, 0, get_byte(word, 1));
  set_byte(&itp, 1, get_byte(word, 0));
#else
#error endianness not specified
#endif
  this->itp = itp;

  this->enable = word & ENABLE_FLAG;
  this->interrupt = word & INTERRUPT_FLAG;
#if defined(IS_BIG_ENDIAN)
  this->max_interrupts = word & MAXINTS_MASK;
#elif defined(IS_LITTLE_ENDIAN)
  this->max_interrupts = (word & MAXINTS_MASK) >> 24;
#else
#error endianness not specified
#endif
}

}
}
