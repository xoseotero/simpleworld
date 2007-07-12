/**
 * @file world/types.hpp
 * Types definition for the world in Simple World.
 *
 * begin:     Sat, 13 Jan 2007 00:50:56 +0100
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

#ifndef __WORLD_TYPES_H__
#define __WORLD_TYPES_H__

#include <fstream>

#include <simple/types.hpp>

namespace SimpleWorld
{
namespace World
{

/**
 * Energy of a bug.
 */
typedef Uint32 Energy;


/**
 * Position of a bug.
 */
struct Position {
  Uint16 x;
  Uint16 y;
};


/**
 * Orientation of a bug.
 */
enum Orientation {
  North,
  East,
  South,
  West
};

/**
 * Get the opposite orientation.
 * @param orientation Initial orientation.
 * @return The new orientation.
 */
Orientation operator !(Orientation orientation);

/**
 * Turn right value times.
 * @param orientation Initial orientation.
 * @param value Times to turn right.
 * @return The new orientation.
 */
Orientation operator +(Orientation orientation, int value);

/**
 * Turn left value times.
 * @param orientation Initial orientation.
 * @param value Times to turn left.
 * @return  The new orientation.
 */
Orientation operator -(Orientation orientation, int value);


/**
 * Time in the world.
 */
typedef Uint32 Time;


/**
 * Order of a bug in the world.
 */
typedef Uint32 SortOrder;

}
}

#endif // __WORLD_TYPES_H__
