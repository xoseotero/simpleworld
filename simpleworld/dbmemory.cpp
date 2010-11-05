/**
 * @file simpleworld/dbmemory.cpp
 * Memory from the database.
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

#include <cstring>

#include <simpleworld/ints.hpp>
#include <simpleworld/cpu/word.hpp>
#include "dbmemory.hpp"

namespace simpleworld
{

DBMemory::DBMemory(const db::Blob& blob)
  : cpu::Memory(0), blob_(blob)
{
  Uint32 size;
  boost::shared_array<Uint8> data = this->blob_.read(&size);
  Memory::resize(size);
  std::memcpy(this->memory_, data.get(), size);
}


/**
 * Set the size of the memory.
 * The new memory is zeroed.
 * @param size new size of the memory.
 */
void DBMemory::resize(cpu::Address size)
{
  Memory::resize(size);
  this->blob_.write(this->memory_, size);
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
void DBMemory::set_word(cpu::Address address, cpu::Word value,
                        bool system_endian)
{
  Memory::set_word(address, value, system_endian);
  if (system_endian)
    value = cpu::change_byte_order(value);
  this->blob_.write(&value, sizeof(cpu::Word), address);
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
void DBMemory::set_halfword(cpu::Address address, cpu::HalfWord value,
                            bool system_endian)
{
  Memory::set_halfword(address, value, system_endian);
  if (system_endian)
    value = cpu::change_byte_order(value);
  this->blob_.write(&value, sizeof(cpu::HalfWord), address);
}

/**
 * Set the value of a quarter word.
 * @param address address of the quarter word
 * @param value value of the quarter word
 * @exception MemoryError if address > (size - 1)
 */
void DBMemory::set_quarterword(cpu::Address address, cpu::QuarterWord value)
{
  Memory::set_quarterword(address, value);
  this->blob_.write(&value, sizeof(cpu::QuarterWord), address);
}


/**
 * Copy the content of other Memory class.
 * @param memory memory to copy.
 * @return a reference to this object.
 */
DBMemory& DBMemory::assign(const cpu::Memory& memory)
{
  Memory::assign(memory);
  this->blob_.write(this->memory_, memory.size());

  return *this;
}

}
