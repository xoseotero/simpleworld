/**
 * @file simpleworld/cpu/memory.cpp
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

#include <boost/format.hpp>

#include <simpleworld/config.hpp>

#include "word.hpp"
#include "memory.hpp"
#include "memoryerror.hpp"

namespace simpleworld
{
namespace cpu
{

/**
 * Constructor.
 * Memory outside the 16bits range can't be used.
 * The memory is zeroed after being allocated.
 * @param size bytes of the memory
 */
Memory::Memory(Address size)
  : size_(0), memory_(NULL)
{
  this->resize(size);
}

/**
 * Copy constructor.
 * @param memory memory to copy.
 */
Memory::Memory(const Memory& memory)
  : size_(0), memory_(NULL)
{
  this->resize(memory.size_);

  for (Address i = 0; i < this->size_; i++)
    this->memory_[i] = memory.memory_[i];
}

/**
 * Destructor.
 */
Memory::~Memory()
{
  if (this->memory_ != NULL)
    delete[] this->memory_;
}


/**
 * Set the size of the memory.
 * The new memory is zeroed.
 * @param size new size of the memory.
 */
void Memory::resize(Address size)
{
  Sint8* tmp = NULL;
  if (size != 0)
    tmp = new Sint8[size];

  Address limit = std::min(this->size_, size);
  // Copy old memory
  for (Address i = 0; i < limit; i++)
    tmp[i] = this->memory_[i];

  // Zeroed new memory
  for (Address i = limit; i < size; i++)
    tmp[i] = 0;

  this->size_ = size;
  if (this->memory_ != NULL)
    delete[] this->memory_;
  this->memory_ = tmp;
}


/**
 * Get a word.
 * In big endian systems the system_endian parameter does nothing.
 * In little endian systems the word is returned in little endian if
 * system_endian is true and in big endian if system_endian is false.
 * @param address address of the word
 * @param system_endian if the address must be in the system endianness
 * @return the word
 * @exception MemoryError if address > (size - 3)
 */
Word Memory::get_word(Address address, bool system_endian) const
{
  if (address > (this->size_ - sizeof(Word)))
    throw EXCEPTION(MemoryError, boost::str(boost::format("\
Address 0x%08X is out of range")
                                            % address));

#if defined(IS_BIG_ENDIAN)
  return *(reinterpret_cast<Word*>(&this->memory_[address]));
#elif defined(IS_LITTLE_ENDIAN)
  if (system_endian)
    return change_byte_order(*(reinterpret_cast<Word*>(&this->memory_[address])));
  else
    return *(reinterpret_cast<Word*>(&this->memory_[address]));
#else
#error endianness not specified
#endif
}


/**
 * Set the value of a word.
 * In big endian systems the system_endian parameter does nothing.
 * In little endian systems the word is supposed to be in little endian if
 * system_endian is true and in big endian if system_endian is false.
 * @param address address of the word
 * @param value value of the word
 * @param system_endian if the word is in the systen endianness
 * @exception MemoryError if address > (size - 3)
 */
void Memory::set_word(Address address, Uint32 value, bool system_endian)
{
  if (address > (this->size_ - sizeof(Word)))
    throw EXCEPTION(MemoryError, boost::str(boost::format("\
Address 0x%08X is out of range")
                                            % address));


#if defined(IS_BIG_ENDIAN)
  *(reinterpret_cast<Uint32*>(&this->memory_[address])) = value;
#elif defined(IS_LITTLE_ENDIAN)
  if (system_endian)
    *(reinterpret_cast<Uint32*>(&this->memory_[address])) =
      change_byte_order(value);
  else
    *(reinterpret_cast<Uint32*>(&this->memory_[address])) = value;
#else
#error endianness not specified
#endif
}


/**
 * Copy the content of other Memory class.
 * @param memory memory to copy.
 * @return a reference to this object.
 */
Memory& Memory::assign(const Memory& memory)
{
  this->resize(memory.size_);

  for (Address i = 0; i < this->size_; i++)
    this->memory_[i] = memory.memory_[i];

  return *this;
}

}
}
