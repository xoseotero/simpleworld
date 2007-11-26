/**
 * @file simpleworld/cpu/instruction.cpp
 * Struct of a instruction.
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

#include <boost/format.hpp>

#include <simpleworld/config.hpp>

#include "word.hpp"
#include "instruction.hpp"

namespace SimpleWorld
{
namespace CPU
{

Word Instruction::encode(const Instruction& instruction)
{
  Word word;

  set_byte(&word, 0, instruction.code);
  set_byte(&word, 1, static_cast<Uint8>(instruction.first << 4 |
                                        instruction.second));

#ifdef IS_BIG_ENDIAN
  set_byte(&word, 2, get_byte(static_cast<Word>(instruction.address), 2));
  set_byte(&word, 3, get_byte(static_cast<Word>(instruction.address), 3));
#else
  set_byte(&word, 2, get_byte(static_cast<Word>(instruction.address), 1));
  set_byte(&word, 3, get_byte(static_cast<Word>(instruction.address), 0));
#endif

  return word;
}

Instruction Instruction::decode(Word word)
{
  Instruction instruction;

  instruction.code = get_byte(word, 0);
  instruction.first = get_byte(word, 1) >> 4;
  instruction.second = get_byte(word, 1) & 0x0F;

#ifdef IS_BIG_ENDIAN
  set_byte(&instruction.address, 0, get_byte(word, 2));
  set_byte(&instruction.address, 1, get_byte(word, 3));
#else
  set_byte(reinterpret_cast<Word*>(&instruction.address), 0,
           get_byte(word, 3));
  set_byte(reinterpret_cast<Word*>(&instruction.address), 1,
           get_byte(word, 2));
#endif

  return instruction;
}

}
}
