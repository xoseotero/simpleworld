/**
 * @file cpu/types.hpp
 * Definition of the types used in the CPU.
 *
 * begin:     Thu, 01 Mar 2007 18:42:48 +0100
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

#ifndef __CPU_TYPES_H__
#define __CPU_TYPES_H__

#include <simple/types.hpp>
#include <world/types.hpp>

namespace SimpleWorld
{
namespace CPU
{

typedef Uint32 Word;
typedef Uint16 Address;

}
}

#endif // __CPU_TYPES_H__
