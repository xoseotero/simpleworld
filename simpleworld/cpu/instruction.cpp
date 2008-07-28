/**
 * @file simpleworld/cpu/instruction.cpp
 * A instruction.
 *
 * begin:     Wed, 08 Nov 2006 16:29:51 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2006-2007  Xosé Otero <xoseotero@users.sourceforge.net>
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

#include "word.hpp"
#include "instruction.hpp"

namespace simpleworld
{
namespace cpu
{

/**
 * Default constructor.
 */
Instruction::Instruction()
{
}

/**
 * Constructor that decodes a word (in big endian).
 * @param word word to decode (in big endian).
 */
Instruction::Instruction(Word word)
{
  this->decode(word);
}


/**
 * Encode the instruction to a Word (in big endian).
 * @return the instruction encoded.
 */
Word Instruction::encode() const
{
  Word word;

  set_byte(&word, 0, this->code);
  set_byte(&word, 1, static_cast<Uint8>(this->first << 4 | this->second));

#if defined(IS_BIG_ENDIAN)
  set_byte(&word, 2, get_byte(static_cast<Word>(this->data), 2));
  set_byte(&word, 3, get_byte(static_cast<Word>(this->data), 3));
#elif defined(IS_LITTLE_ENDIAN)
  set_byte(&word, 2, get_byte(static_cast<Word>(this->data), 1));
  set_byte(&word, 3, get_byte(static_cast<Word>(this->data), 0));
#else
#error endianness not specified
#endif

  return word;
}

/**
 * Decode the instruction (in big endian).
 * @param word word to decode (in big endian).
 */
void Instruction::decode(Word word)
{
  this->code = get_byte(word, 0);
  this->first = get_byte(word, 1) >> 4;
  this->second = get_byte(word, 1) & 0x0f;

  Word data = static_cast<Word>(this->data);
#if defined(IS_BIG_ENDIAN)
  set_byte(&data, 2, get_byte(word, 2));
  set_byte(&data, 3, get_byte(word, 3));
#elif defined(IS_LITTLE_ENDIAN)
  set_byte(&data, 0, get_byte(word, 3));
  set_byte(&data, 1, get_byte(word, 2));
#else
#error endianness not specified
#endif
  this->data = data;
}

}
}
