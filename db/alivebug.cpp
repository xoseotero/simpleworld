/**
 * @file db/alivebug.cpp
 * Information about a alive bug.
 *
 * begin:     Wed, 15 Aug 2007 13:56:46 +0200
 * last:      $Date$
 *
 *  Copyright (C) 2007  Xosé Otero <xoseotero@users.sourceforge.net>
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

#include <sqlite3x.hpp>

#include "alivebug.hpp"

namespace SimpleWorld
{
namespace DB
{

AliveBug::AliveBug(DB* db, ID id, ElementType type, Position position)
  : BugElement(db, id, type, position)
{
}

AliveBug::AliveBug(DB* db, ElementType type)
  : BugElement(db, type)
{
}

}
}