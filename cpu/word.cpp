/**
 * @file cpu/word.cpp
 * Word class for accessing its bytes.
 *
 * begin:     Sat, 11 Dec 2004 22:06:28 +0100
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2004, 2006, Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  License: See COPYING file that comes with this distribution
 */

#include "word.h"

namespace SimpleWorld
{
namespace CPU
{

Uint8 get_byte(Uint32 word, Uint8 byte) throw (ByteOutOfRange)
{
  if (byte > 3)
    throw ByteOutOfRange(__FILE__, __LINE__);;

  return reinterpret_cast<Uint8*>(&word)[byte];
}

void set_byte(Uint32* word, Uint8 byte, Uint8 value)
  throw (ByteOutOfRange)
{
  if (byte > 3)
    throw ByteOutOfRange(__FILE__, __LINE__);;

  reinterpret_cast<Uint8*>(word)[byte] = value;
}

Uint32 change_byte_order(Uint32 word) throw ()
{
  return get_byte(word, 0) << 24 | get_byte(word, 1) << 16 |
    get_byte(word, 2) << 8 | get_byte(word, 3);
}

Uint32 change_byte_order_middle(Uint32 word) throw ()
{
  return get_byte(word, 0) << 8 | get_byte(word, 1) | get_byte(word, 2) << 24 |
    get_byte(word, 3) << 16;
}

} // namespace CPU
} // namespace SimpleWorld
