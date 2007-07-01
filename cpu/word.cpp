/**
 * @file cpu/word.cpp
 * Word class for accessing its bytes.
 *
 * begin:     Sat, 11 Dec 2004 22:06:28 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2004, 2006, Xosé Otero <xoseotero@users.sourceforge.net>
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

#include "word.hpp"

namespace SimpleWorld
{
namespace CPU
{

Uint8 get_byte(Word word, Uint8 byte)
{
  if (byte > 3)
    throw ByteOutOfRange(__FILE__, __LINE__);

  return reinterpret_cast<Uint8*>(&word)[byte];
}

void set_byte(Word* word, Uint8 byte, Uint8 value)
{
  if (byte > 3)
    throw ByteOutOfRange(__FILE__, __LINE__);;

  reinterpret_cast<Uint8*>(word)[byte] = value;
}

Word change_byte_order(Word word)
{
  return get_byte(word, 0) << 24 | get_byte(word, 1) << 16 |
    get_byte(word, 2) << 8 | get_byte(word, 3);
}

Word change_byte_order_middle(Word word)
{
  return get_byte(word, 0) << 8 | get_byte(word, 1) | get_byte(word, 2) << 24 |
    get_byte(word, 3) << 16;
}

}
}
