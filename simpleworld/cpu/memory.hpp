 /**
 * @file simpleworld/cpu/memory.hpp
 * Accessing words from memory.
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

#ifndef SIMPLEWORLD_CPU_MEMORY_HPP
#define SIMPLEWORLD_CPU_MEMORY_HPP

#include <simpleworld/ints.hpp>
#include <simpleworld/cpu/types.hpp>

namespace SimpleWorld
{
namespace CPU
{

/**
 * Memory array addressable in words.
 * The words are stored in big endian.
 */
class Memory
{
public:
  /**
   * Constructor.
   * Memory outside the 16bits range can't be used.
   * The memory is zeroed after being allocated.
   * @param size bytes of the memory
   */
  Memory(Address size = 0);

  /**
   * Copy constructor.
   * @param memory memory to copy.
   */
  Memory(const Memory& memory);

  /**
   * Destructor.
   */
  virtual ~Memory();


  /**
   * Get the size of the memory.
   * @return the size of the memory.
   */
  Address size() const { return this->size_; }

  /**
   * Set the size of the memory.
   * The new memory is zeroed.
   * @param size new size of the memory.
   */
  virtual void resize(Address size);


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
  virtual Word get_word(Address address, bool system_endian = true) const;

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
  virtual void set_word(Address address, Word value, bool system_endian = true);


  /**
   * Get a word in the system endian.
   * In big endian systems the system_endian parameter does nothing.
   * In little endian systems the word is returned in little endian if
   * system_endian is true and in big endian if system_endian is false.
   * @param address address of the word
   * @return the word
   * @exception MemoryError if address > (size - 3)
   */
  Word operator [](Address address) const
  { return this->get_word(address); }


  /**
   * Copy the content of other Memory class.
   * @param memory memory to copy.
   * @return a reference to this object.
   */
  Memory& assign(const Memory& memory);

  /**
   * Copy the content of other Memory class.
   * @param memory memory to copy.
   * @return a reference to this object.
   */
  Memory& operator =(const Memory& memory) { return this->assign(memory); }

private:
  Address size_;
  Sint8* memory_;
};

}
}

#endif // SIMPLEWORLD_CPU_MEMORY_HPP
