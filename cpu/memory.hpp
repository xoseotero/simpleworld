/**
 * @file cpu/memory.h
 * Accessing words from memory.
 *
 * begin:     Sun, 05 Nov 2006 16:33:13 +0100
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2006, Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __CPU_MEMORY_H__
#define __CPU_MEMORY_H__

#include <simple/types.hpp>
#include <cpu/types.hpp>
#include <cpu/exception.hpp>

namespace SimpleWorld
{
namespace CPU
{

/**
 * Memory exception.
 * It's raised if you try to use a memory with less than 4 bytes.
 */
class NotEnoughLength: public std::runtime_error, public CPUException
{
  /**
   * Constructor.
   * @param file File where the exception is raised.
   * @param line Line where the exception is raised.
   * @param what Reason of the exception.
   */
  public:
  NotEnoughLength(std::string file = "", Uint32 line = 0,
                  const std::string& what = "Not enough lenght") throw()
    : runtime_error(what), CPUException(file, line)
  {}

  /**
   * Destructor.
   */
  ~NotEnoughLength() throw () {}
};

/**
 * Memory exception.
 * It's raised if you try to get a wrong word.
 */
class AddressOutOfRange: public std::out_of_range, public CPUException
{
public:
  /**
   * Constructor.
   * @param file File where the exception is raised.
   * @param line Line where the exception is raised.
   * @param what Reason of the exception.
   */
  AddressOutOfRange(std::string file = "", Uint32 line = 0,
                    const std::string& what = "Address out of range") throw()
    : out_of_range(what), CPUException(file, line)
  {}

  /**
   * Destructor.
   */
  ~AddressOutOfRange() throw () {}
};


/**
 * Memory array addressable in words.
 * The words are stored in big endian.
 */
class Memory
{
public:
  /**
   * Constructor.
   * The memory is zeroed after being allocated.
   * @param length bytes of the memory
   * @exception NotEnoughLength length < 4
   */
  Memory(Address length);

  /**
   * Destructor.
   */
  ~Memory();


  /**
   * Get a word.
   * In big endian systems the system_endian parameter does nothing.
   * In little endian systems the word is returned in little endian if
   * system_endian is true and in big endian if system_endian is false.
   * @param address address of the word
   * @param system_endian if the address must be in the system endianness
   * @return the word
   * @exception AddressOutOfRange address > (length - 4)
   */
  Word get_word(Address address, bool system_endian = true) const;

  /**
   * Set the value of a word.
   * In big endian systems the system_endian parameter does nothing.
   * In little endian systems the word is supposed to be in little endian if
   * system_endian is true and in big endian if system_endian is false.
   * @param address address of the word
   * @param value value of the word
   * @param system_endian if the word is in the systen endianness
   * @return the word
   * @exception AddressOutOfRange address > (length - 4)
   */
  Word set_word(Address address, Word value, bool system_endian = true);


  /**
   * Get a word.
   * @param address address of the word
   * @return the word
   * @exception AddressOutOfRange address > (length - 4)
   */
  Word operator [](Address address) const
  { return this->get_word(address); }

private:
  Address length_;
  Sint8* memory_;
};

}
}

#endif // __CPU_MEMORY_H__
