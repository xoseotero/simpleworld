/**
 * @file simpleworld/db/memory.cpp
 * Accessing words from memory advising when a change was made.
 *
 * begin:     Fri, 28 Sep 2007 22:30:53 +0200
 * last:      $Date$
 *
 *  Copyright (C) 2007  Xosé Antón Otero Ferreira <xoseotero@gmail.com>
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

Memory::Memory(bool* changed, CPU::Address size)
  : CPU::Memory(size), changed_(changed)
{
  *this->changed_ = false;
}

Memory::Memory(const Memory& memory, bool* changed)
  : CPU::Memory(memory), changed_(changed)
{
  *this->changed_ = false;
}


void Memory::resize(CPU::Address size)
{
  CPU::Memory::resize(size);

  *this->changed_ = true;
}


void Memory::set_word(CPU::Address address, CPU::Word value,
                      bool system_endian)
{
  CPU::Memory::set_word(address, value, system_endian);

  *this->changed_ = true;
}


Memory& Memory::assign(const CPU::Memory& memory)
{
  CPU::Memory::assign(memory);

  *this->changed_ = true;

  return *this;
}

}
}
