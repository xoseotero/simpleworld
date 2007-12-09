/**
 * @file simpleworld/movement.hpp
 * Movements that a bug can do.
 *
 * begin:     Wed, 18 Jul 2007 12:02:53 +0200
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

#include <cstdlib>

#include <boost/format.hpp>

#include "worlderror.hpp"
#include "movement.hpp"

namespace SimpleWorld
{

Orientation turn(Orientation orientation, Turn side)
{
  if (orientation != OrientationNorth and
      orientation != OrientationEast and
      orientation != OrientationSouth and
      orientation != OrientationWest)
    throw EXCEPTION(WorldError, boost::str(boost::format("\
Wrong orientation value (%04x)")
                                           % orientation));


  if (side != TurnLeft and side != TurnRight)
    throw EXCEPTION(WorldError, boost::str(boost::format("\
Wrong turn value (%04x)")
                                           % side));

  switch (side) {
  case TurnLeft:
    return static_cast<Orientation>(std::abs((static_cast<int>(orientation) +
                                              3)) % 4);

  case TurnRight:
    return static_cast<Orientation>(std::abs((static_cast<int>(orientation)
                                              + 5)) % 4);
  }
}


/**
 * Get the opposite direction.
 * @param movement original movement.
 * @return the opposite directio.
 */
static Movement opposite(Movement movement)
{
  if (movement == MoveForward)
    return MoveBackward;
  else
    return MoveForward;
}

Position move(Position position, Orientation orientation, Movement movement,
              Position max)
{
  if (orientation != OrientationNorth and
      orientation != OrientationEast and
      orientation != OrientationSouth and
      orientation != OrientationWest)
    throw EXCEPTION(WorldError, boost::str(boost::format("\
Wrong orientation value (%04x)")
                                           % orientation));

  if (movement != MoveForward and
      movement != MoveBackward)
    throw EXCEPTION(WorldError, boost::str(boost::format("\
Wrong movement value (%04x)")
                                           % movement));

  switch (orientation) {
  case OrientationSouth:
    movement = opposite(movement);
  case OrientationNorth:
    if (movement == MoveForward)
      position.y = (position.y + 1 + max.y) % max.y;
    else
      position.y = (position.y - 1 + max.y) % max.y;

    return position;

  case OrientationWest:
    movement = opposite(movement);
  case OrientationEast:
    if (movement == MoveForward)
      position.x = (position.x + 1 + max.x) % max.x;
    else
      position.x = (position.x - 1 + max.x) % max.x;

    return position;
  }
}

}
