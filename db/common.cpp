/**
 * @file db/common.cpp
 * Common functionality for the DB namespace.
 *
 * begin:     Wed, 19 Sep 2007 19:51:45 +0200
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

#include <boost/format.hpp>
#include <boost/scoped_array.hpp>

#include <cpu/types.hpp>
#include "common.hpp"

namespace SimpleWorld
{
namespace DB
{

::SimpleWorld::CPU::Memory get_memory(sqlite3x::sqlite3_cursor* cursor,
                                      int index)
{
  ::SimpleWorld::CPU::Memory memory;

  int size;
  const CPU::Word* blob =
    static_cast<const CPU::Word*>(cursor->getblob(index, size));

  memory.resize(static_cast<CPU::Address>(size));

  CPU::Address i;
  for (i = 0; i < size; i += sizeof(CPU::Word))
    memory.set_word(i, blob[i / sizeof(CPU::Word)], false);

  return memory;
}

void bind_memory(sqlite3x::sqlite3_command* sql, int index,
                 const ::SimpleWorld::CPU::Memory& memory)
{
  Uint16 size = memory.size();

  boost::scoped_array<CPU::Word> blob(new CPU::Word[size / sizeof(CPU::Word)]);

  CPU::Address i;
  for (i = 0; i < size; i += sizeof(CPU::Word))
    blob[i / sizeof(CPU::Word)] = memory.get_word(i, false);

  sql->bind(index, blob.get(), size);
}

}
}
