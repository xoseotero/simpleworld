/**
 * @file cpu/memory.cpp
 * Memory class for accessing words from memory.
 *
 * begin:     Sun, 05 Nov 2006 16:33:13 +0100
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

#include <algorithm>

#include "../simple/config.hpp"
#include "word.hpp"
#include "memory.hpp"

namespace SimpleWorld
{
namespace CPU
{

Memory::Memory(Address size)
  : size_(0), memory_(NULL)
{
  this->resize(size);
}

Memory::Memory(const Memory& memory)
  : size_(0), memory_(NULL)
{
  this->resize(memory.size_);

  Address i;
  for (i = 0; i < this->size_; i++)
    this->memory_[i] = memory.memory_[i];
}

Memory::~Memory()
{
  if (this->memory_ != NULL)
    delete[] this->memory_;
}


void Memory::resize(Address size)
{
  Sint8* tmp = NULL;
  if (size != 0)
    tmp = new Sint8[size];

  Address i;
  Address limit = std::min(this->size_, size);
  // Copy old memory
  for (i = 0; i < limit; i++)
    tmp[i] = this->memory_[i];

  // Zeroed new memory
  for (i = limit; i < size; i++)
    tmp[i] = 0;

  this->size_ = size;
  if (this->memory_ != NULL)
    delete[] this->memory_;
  this->memory_ = tmp;
}


Word Memory::get_word(Address address, bool system_endian) const
{
  if (address > (this->size_ - (sizeof(Word) - 1)))
    throw AddressOutOfRange(__FILE__, __LINE__, address);

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


void Memory::set_word(Address address, Uint32 value, bool system_endian)
{
  if (address > (this->size_ - (sizeof(Word) - 1)))
    throw AddressOutOfRange(__FILE__, __LINE__, address);

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


Memory& Memory::assign(const Memory& memory)
{
  this->resize(memory.size_);

  Address i;
  for (i = 0; i < this->size_; i++)
    this->memory_[i] = memory.memory_[i];

  return *this;
}

}
}
