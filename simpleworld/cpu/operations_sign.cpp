/**
 * @file simpleworld/cpu/operations_sign.cpp
 * Sign extension operations of the Simple CPU.
 *
 *  Copyright (C) 2008  Xosé Otero <xoseotero@gmail.com>
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
#include "operations.hpp"

#define HALFWORD_SIGN 0x8000
#define QUARTERWORD_SIGN 0x0080

namespace simpleworld
{
namespace cpu
{

/**
 * Extend the sign of a half word to the whole word.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update signh(CPU& cpu, Instruction inst)
{
  Word word = cpu.get_reg(inst.second);
  if (word & HALFWORD_SIGN) {
#if defined(IS_BIG_ENDIAN)
    set_byte(&word, 0, 0xff);
    set_byte(&word, 1, 0xff);
#elif defined(IS_LITTLE_ENDIAN)
    set_byte(&word, 2, 0xff);
    set_byte(&word, 3, 0xff);
#else
#error endianness not specified
#endif
  } else {
#if defined(IS_BIG_ENDIAN)
    set_byte(&word, 0, 0x0);
    set_byte(&word, 1, 0x0);
#elif defined(IS_LITTLE_ENDIAN)
    set_byte(&word, 2, 0x0);
    set_byte(&word, 3, 0x0);
#else
#error endianness not specified
#endif
  }

  cpu.set_reg(inst.first, word);

  return UpdatePC;
}

/**
 * Extend the sign of a quarter word to the whole word.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update signq(CPU& cpu, Instruction inst)
{
  Word word = cpu.get_reg(inst.second);
  if (word & QUARTERWORD_SIGN) {
#if defined(IS_BIG_ENDIAN)
    set_byte(&word, 0, 0xff);
    set_byte(&word, 1, 0xff);
    set_byte(&word, 2, 0xff);
#elif defined(IS_LITTLE_ENDIAN)
    set_byte(&word, 1, 0xff);
    set_byte(&word, 2, 0xff);
    set_byte(&word, 3, 0xff);
#else
#error endianness not specified
#endif
  } else {
#if defined(IS_BIG_ENDIAN)
    set_byte(&word, 0, 0x0);
    set_byte(&word, 1, 0x0);
    set_byte(&word, 2, 0x0);
#elif defined(IS_LITTLE_ENDIAN)
    set_byte(&word, 1, 0x0);
    set_byte(&word, 2, 0x0);
    set_byte(&word, 3, 0x0);
#else
#error endianness not specified
#endif
  }

  cpu.set_reg(inst.first, word);

  return UpdatePC;
}

}
}
