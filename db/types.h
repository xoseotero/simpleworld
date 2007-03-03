/**
 * @file db/types.h
 * Definition of the types used in the database.
 *
 * begin:     Sat, 13 Jan 2007 00:46:33 +0100
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2007, Xosé Otero <xoseotero@users.sourceforge.net>
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

#ifndef __DB_TYPES_H__
#define __DB_TYPES_H__

#include <sqlite3x.hpp>

#include <simple/types.h>
#include <world/types.h>
#include <cpu/types.h>

namespace SimpleWorld
{
namespace DB
{

typedef int64_t ID;

using World::SortOrder;
using World::Energy;
typedef Uint32 Position;
typedef Uint8 Orientation;
using World::Time;
using CPU::Word;

}
}

#endif // __DB_TYPES_H__
