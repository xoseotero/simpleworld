/**
 * @file simpleworld/db/memory.cpp
 * Accessing words from memory advising when a change was made.
 *
 * begin:     Fri, 28 Sep 2007 22:30:53 +0200
 * last:      $Date$
 *
 *  Copyright (C) 2007  Xosé Otero <xoseotero@gmail.com>
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

#include "memory.hpp"

namespace SimpleWorld
{
namespace DB
{

/**
 * Constructor.
 * Memory outside the 16bits range can't be used.
 * The memory is zeroed after being allocated.
 * @param changed pointer to the variable used to advise about a change.
 * @param size bytes of the memory
 */
Memory::Memory(bool* changed, CPU::Address size)
  : CPU::Memory(size), changed_(changed)
{
  *this->changed_ = false;
}

/**
 * Copy constructor.
 * @param memory memory to copy.
 * @param changed pointer to the variable used to advise about a change.
 */
Memory::Memory(const Memory& memory, bool* changed)
  : CPU::Memory(memory), changed_(changed)
{
  *this->changed_ = false;
}


/**
 * Set the size of the memory.
 * The new memory is zeroed.
 */
void Memory::resize(CPU::Address size)
{
  CPU::Memory::resize(size);

  *this->changed_ = true;
}


/**
 * Set the value of a word.
 * In big endian systems the system_endian parameter does nothing.
 * In little endian systems the word is supposed to be in little endian if
 * system_endian is true and in big endian if system_endian is false.
 * @param address address of the word
 * @param value value of the word
 * @param system_endian if the word is in the systen endianness
 * @exception SimpleWorld::Exception address > (size - 4)
 */
void Memory::set_word(CPU::Address address, CPU::Word value,
                      bool system_endian)
{
  CPU::Memory::set_word(address, value, system_endian);

  *this->changed_ = true;
}


/**
 * Copy the content of other Memory class.
 * @param memory memory to copy.
 * @return a reference to this object.
 */
Memory& Memory::assign(const CPU::Memory& memory)
{
  CPU::Memory::assign(memory);

  *this->changed_ = true;

  return *this;
}

}
}
