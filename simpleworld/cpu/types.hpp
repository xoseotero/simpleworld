/**
 * @file simpleworld/cpu/types.hpp
 * Definition of the types used in the CPU.
 *
 *  Copyright (C) 2007-2010  Xosé Otero <xoseotero@users.sourceforge.net>
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

#ifndef SIMPLEWORLD_CPU_TYPES_HPP
#define SIMPLEWORLD_CPU_TYPES_HPP

#include <simpleworld/ints.hpp>

namespace simpleworld
{
namespace cpu
{

typedef Uint32 Word;
typedef Uint16 HalfWord;
typedef Uint8 QuarterWord;

typedef Uint32 Address;
typedef Sint16 Offset;

}
}

#endif // SIMPLEWORLD_CPU_TYPES_HPP
