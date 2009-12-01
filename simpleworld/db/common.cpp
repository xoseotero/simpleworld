/**
 * @file simpleworld/db/common.cpp
 * Common functionality for the DB namespace.
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

#include <boost/format.hpp>
#include <boost/scoped_array.hpp>

#include <simpleworld/cpu/types.hpp>
#include "common.hpp"

namespace simpleworld
{
namespace db
{

/**
 * Get a block of memory from a sqlite3_cursor.
 * @param cursor pointer to a cursor of SQLite3x.
 * @param index row where the BLOB is stored.
 * @return the block of memory.
 */
cpu::Memory get_memory(sqlite3x::sqlite3_cursor* cursor, int index)
{
  cpu::Memory memory;

  int size;
  const cpu::Word* blob =
    static_cast<const cpu::Word*>(cursor->getblob(index, size));

  memory.resize(static_cast<cpu::Address>(size));

  for (cpu::Address i = 0; i < size; i += sizeof(cpu::Word))
    memory.set_word(i, blob[i / sizeof(cpu::Word)], false);

  return memory;
}

/**
 * Bind a block of memory to a sqlite3_command.
 * @param sql pointer to a command of SQLite3x.
 * @param index row where the BLOB is stored.
 * @param memory the block of memory.
 */
void bind_memory(sqlite3x::sqlite3_command* sql, int index,
                 const cpu::Memory& memory)
{
  Uint16 size = memory.size();

  boost::scoped_array<cpu::Word> blob(new cpu::Word[size / sizeof(cpu::Word)]);

  for (cpu::Address i = 0; i < size; i += sizeof(cpu::Word))
    blob[i / sizeof(cpu::Word)] = memory.get_word(i, false);

  sql->bind(index, blob.get(), size);
}

}
}
