/**
 * @file cpu/memory.cpp
 * Memory class for accessing words from memory.
 *
 * begin:     Sun, 05 Nov 2006 16:33:13 +0100
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2006, Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  License: See COPYING file that comes with this distribution
 */

#include "../simple/config.h"
#include "word.h"
#include "memory.h"

namespace SimpleWorld
{
namespace CPU
{

Memory::Memory(Address length) throw (NotEnoughLength)
  : length_(length)
{
  if (length < 4)
    throw NotEnoughLength(__FILE__, __LINE__);

  this->memory_ = new Sint8[length];

  // Zeroed the memory
  Address i;
  for (i = 0; i < length; i++)
    this->memory_[i] = 0;
}

Memory::~Memory() throw ()
{
  delete[] this->memory_;
}


Word Memory::get_word(Address address, bool system_endian) const
  throw(AddressOutOfRange)
{
  if (address > (this->length_ - 4))
    throw AddressOutOfRange(__FILE__, __LINE__);

#ifdef IS_BIG_ENDIAN
  return *(reinterpret_cast<Word*>(&this->memory_[address]));
#else
  if (system_endian)
    if (address % 2 == 0)
      return
      change_byte_order(*(reinterpret_cast<Word*>(&this->memory_[address])));
    else
      return
change_byte_order_middle(*(reinterpret_cast<Word*>(&this->memory_[address])));
  else
    return *(reinterpret_cast<Word*>(&this->memory_[address]));
#endif
}


Word Memory::set_word(Address address, Uint32 value, bool system_endian)
  throw(AddressOutOfRange)
{
  if (address > (this->length_ - 4))
    throw AddressOutOfRange(__FILE__, __LINE__);

#ifdef IS_BIG_ENDIAN
  *(reinterpret_cast<Uint32*>(&this->memory_[address])) = value;
#else
  if (system_endian)
    if (address % 2 == 0)
      *(reinterpret_cast<Uint32*>(&this->memory_[address])) =
	change_byte_order(value);
    else
      *(reinterpret_cast<Uint32*>(&this->memory_[address])) =
	change_byte_order_middle(value);
  else
    *(reinterpret_cast<Uint32*>(&this->memory_[address])) = value;
#endif
}

} // namespace CPU
} // namespace SimpleWorld
