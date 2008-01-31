/**
 * @file simpleworld/types.hpp
 * Types definition for Simple World.
 *
 * begin:     Mon, 30 Jul 2007 12:16:49 +0200
 * last:      $Date$
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

#ifndef SIMPLEWORLD_TYPES_HPP
#define SIMPLEWORLD_TYPES_HPP

#include <simpleworld/ints.hpp>

namespace SimpleWorld
{

/**
 * Coordinate of a position.
 */
typedef Uint16 Coord;

/**
 * Position of a element.
 */
struct Position {
  Coord x;
  Coord y;
};

/**
 * Type of movement of a bug.
 */
enum Movement {
  MoveForward = 0,
  MoveBackward = 1
};


/**
 * Orientation of a element.
 */
enum Orientation {
  OrientationNorth = 0,
  OrientationEast = 1,
  OrientationSouth = 2,
  OrientationWest = 3
};

/**
 * Type of turn of a bug.
 */
enum Turn {
  TurnLeft = 0,
  TurnRight = 1
};


/**
 * Time in the world.
 */
typedef Uint32 Time;


/**
 * Type of elements in the World.
 */
enum ElementType {
  ElementNothing = 0,
  ElementEgg = 1,
  ElementBug = 2,
  ElementFood = 3
};


/**
 * Status of a action.
 */
enum ActionStatus {
  ActionSuccess = 0,
  ActionFailure = 1
};


/**
 * Types of events.
 */
enum EventType {
  EventAttack = 0
};


/**
 * Energy of a element.
 */
typedef Uint32 Energy;


/**
 * Information of the World.
 */
enum Info {
  InfoID = 0,
  InfoSize = 1,
  InfoEnergy = 2,
  InfoPosition = 3,
  InfoOrientation = 4
};

}

#endif // SIMPLEWORLD_TYPES_HPP
