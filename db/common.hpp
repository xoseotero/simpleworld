/**
 * @file db/common.hpp
 * Common functionality for the DB namespace.
 *
 * begin:     Wed, 19 Sep 2007 19:34:38 +0200
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

#ifndef __DB_COMMON_HPP__
#define __DB_COMMON_HPP__

#include <sqlite3x.hpp>

#include <cpu/memory.hpp>

namespace SimpleWorld
{
namespace DB
{

/**
 * Get a block of memory from a sqlite3_cursor.
 * @param cursor pointer to a cursor of SQLite3x.
 * @param index row where the BLOB is stored.
 * @return the block of memory.
 */
::SimpleWorld::CPU::Memory get_memory(sqlite3x::sqlite3_cursor* cursor,
                                      int index);

/**
 * Bind a block of memory to a sqlite3_command.
 * @param cursor pointer to a cursor of SQLite3x.
 * @param index row where the BLOB is stored.
 * @param memory the block of memory.
 */
void bind_memory(sqlite3x::sqlite3_command* sql, int index,
                 const ::SimpleWorld::CPU::Memory& memory);
}
}

#endif // __DB_COMMON_HPP__
