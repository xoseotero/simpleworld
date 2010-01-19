/**
 * @file src/swcpu/memoryfile.hpp
 * MemoryFile subclass that stores the last accessed address.
 *
 *  Copyright (C) 2010  Xosé Otero <xoseotero@gmail.com>
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

#ifndef MEMORYFILE_HPP
#define MEMORYFILE_HPP

#include <string>

#include <simpleworld/cpu/types.hpp>
#include <simpleworld/cpu/memory_file.hpp>
namespace cpu = simpleworld::cpu;


/**
 * MemoryFile subclass that stores the last accessed address.
 */
class MemoryFile: public cpu::MemoryFile
{
public:
  /**
   * Constructor.
   * @param filename filename from where to load the code.
   * @param last_access pointer to store the lattest address used.
   * @exception IOError problem with the file.
   */
  MemoryFile(const std::string& filename, cpu::Address* last_access);


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
  void set_word(cpu::Address address, cpu::Word value,
                bool system_endian = true);

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
  void set_halfword(cpu::Address address, cpu::HalfWord value,
                    bool system_endian = true);

  /**
   * Set the value of a quarter word.
   * @param address address of the quarter word
   * @param value value of the quarter word
   * @exception MemoryError if address > (size - 1)
   */
  void set_quarterword(cpu::Address address, cpu::QuarterWord value);


private:
  cpu::Address* last_access;
};

#endif // MEMORYFILE_HPP
