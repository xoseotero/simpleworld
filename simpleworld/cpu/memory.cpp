/**
 * @file simpleworld/cpu/memory.cpp
 * Memory class for accessing words from memory.
 *
 *  Copyright (C) 2006-2010  Xosé Otero <xoseotero@gmail.com>
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
#include <cstring>

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

Memory::Memory(const void* data, Address size)
  : size_(0), memory_(NULL)
{
  this->resize(size);

  std::memcpy(this->memory_, data, size);
}

/**
 * Copy constructor.
 * @param memory memory to copy.
 */
Memory::Memory(const Memory& memory)
  : size_(0), memory_(NULL)
{
  this->resize(memory.size_);

  std::memcpy(this->memory_, memory.memory_, this->size_);
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
  Uint8* tmp = NULL;
  if (size != 0)
    tmp = new Uint8[size];

  Address limit = std::min(this->size_, size);
  // Copy old memory
  std::memcpy(tmp, this->memory_, limit);
  // Zeroed new memory
  std::memset(tmp + limit, 0, size - limit);

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
 * @param system_endian if the word must be in the system endianness
 * @return the word
 * @exception MemoryError if address > (size - 4)
 */
Word Memory::get_word(Address address, bool system_endian) const
{
  if (address > (this->size_ - sizeof(Word)))
    throw EXCEPTION1(MemoryError, boost::str(boost::format("\
Address 0x%08X is out of range")
                                            % address), address);

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
 * Get a half word.
 * In big endian systems the system_endian parameter does nothing.
 * In little endian systems the word is returned in little endian if
 * system_endian is true and in big endian if system_endian is false.
 * @param address address of the half word
 * @param system_endian if the half word must be in the system endianness
 * @return the half word
 * @exception MemoryError if address > (size - 2)
 */
HalfWord Memory::get_halfword(Address address, bool system_endian) const
{
  if (address > (this->size_ - sizeof(HalfWord)))
    throw EXCEPTION1(MemoryError, boost::str(boost::format("\
Address 0x%08X is out of range")
                                            % address), address);

#if defined(IS_BIG_ENDIAN)
  return *(reinterpret_cast<HalfWord*>(&this->memory_[address]));
#elif defined(IS_LITTLE_ENDIAN)
  if (system_endian)
    return change_byte_order(*(reinterpret_cast<HalfWord*>(&this->memory_[address])));
  else
    return *(reinterpret_cast<HalfWord*>(&this->memory_[address]));
#else
#error endianness not specified
#endif
}

/**
 * Get a quarter word.
 * @param address address of the word
 * @return the quarter word
 * @exception MemoryError if address > (size - 1)
 */
QuarterWord Memory::get_quarterword(Address address) const
{
  if (address > (this->size_ - sizeof(QuarterWord)))
    throw EXCEPTION1(MemoryError, boost::str(boost::format("\
Address 0x%08X is out of range")
                                            % address), address);

  return *(reinterpret_cast<QuarterWord*>(&this->memory_[address]));
}


/**
 * Set the value of a word.
 * In big endian systems the system_endian parameter does nothing.
 * In little endian systems the word is supposed to be in little endian if
 * system_endian is true and in big endian if system_endian is false.
 * @param address address of the word
 * @param value value of the word
 * @param system_endian if the word is in the systen endianness
 * @exception MemoryError if address > (size - 4)
 */
void Memory::set_word(Address address, Word value, bool system_endian)
{
  if (address > (this->size_ - sizeof(Word)))
    throw EXCEPTION1(MemoryError, boost::str(boost::format("\
Address 0x%08X is out of range")
                                            % address), address);


#if defined(IS_BIG_ENDIAN)
  *(reinterpret_cast<Word*>(&this->memory_[address])) = value;
#elif defined(IS_LITTLE_ENDIAN)
  if (system_endian)
    *(reinterpret_cast<Word*>(&this->memory_[address])) =
      change_byte_order(value);
  else
    *(reinterpret_cast<Word*>(&this->memory_[address])) = value;
#else
#error endianness not specified
#endif
}

/**
 * Set the value of a half word.
 * In big endian systems the system_endian parameter does nothing.
 * In little endian systems the word is supposed to be in little endian if
 * system_endian is true and in big endian if system_endian is false.
 * @param address address of the half word
 * @param value value of the half word
 * @param system_endian if the half word is in the systen endianness
 * @exception MemoryError if address > (size - 2)
 */
void Memory::set_halfword(Address address, HalfWord value, bool system_endian)
{
  if (address > (this->size_ - sizeof(HalfWord)))
    throw EXCEPTION1(MemoryError, boost::str(boost::format("\
Address 0x%08X is out of range")
                                            % address), address);


#if defined(IS_BIG_ENDIAN)
  *(reinterpret_cast<HalfWord*>(&this->memory_[address])) = value;
#elif defined(IS_LITTLE_ENDIAN)
  if (system_endian)
    *(reinterpret_cast<HalfWord*>(&this->memory_[address])) =
      change_byte_order(value);
  else
    *(reinterpret_cast<HalfWord*>(&this->memory_[address])) = value;
#else
#error endianness not specified
#endif
}

/**
 * Set the value of a quarter word.
 * @param address address of the quarter word
 * @param value value of the quarter word
 * @exception MemoryError if address > (size - 1)
 */
void Memory::set_quarterword(Address address, QuarterWord value)
{
  if (address > (this->size_ - sizeof(QuarterWord)))
    throw EXCEPTION1(MemoryError, boost::str(boost::format("\
Address 0x%08X is out of range")
                                            % address), address);


  *(reinterpret_cast<QuarterWord*>(&this->memory_[address])) = value;
}


/**
 * Copy the content of other Memory class.
 * @param memory memory to copy.
 * @return a reference to this object.
 */
Memory& Memory::assign(const Memory& memory)
{
  this->resize(memory.size_);

  std::memcpy(this->memory_, memory.memory_, this->size_);

  return *this;
}

}
}
