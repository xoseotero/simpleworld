/**
 * @file world/types.cpp
 * Types definition for the world in Simple World.
 *
 * begin:     Wed, 24 Jan 2007 00:12:37 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2007, Xosé Otero <xoseotero@users.sourceforge.net>
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

#include "types.hpp"

namespace SimpleWorld
{
namespace World
{

Orientation operator !(Orientation orientation)
{
  return static_cast<Orientation>((static_cast<int>(orientation) + 2) % 4);
}

Orientation operator +(Orientation orientation, int value)
{
  return static_cast<Orientation>(std::abs((static_cast<int>(orientation) +
					    value + 4)) % 4);
}

Orientation operator -(Orientation orientation, int value)
{
  return static_cast<Orientation>(std::abs((static_cast<int>(orientation) -
					    value + 4)) % 4);
}


// /**
//  * Print the orientation.
//  * @param os Where to print the file.
//  * @param orientation Orientation to print.
//  * @return Reference to where to print.
//  */
// std::ostream& operator <<(std::ostream& os, Orientation orientation)
// {
//   static char* names[] = {"North", "East", "South", "West"};

//   return os << names[static_cast<int>(orientation)];
// }

}
}
