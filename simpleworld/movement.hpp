/**
 * @file simpleworld/movement.hpp
 * Movements that a bug can do.
 *
 * begin:     Wed, 18 Jul 2007 11:59:37 +0200
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

#ifndef SIMPLEWORLD_MOVEMENT_HPP
#define SIMPLEWORLD_MOVEMENT_HPP

#include <simpleworld/types.hpp>

namespace SimpleWorld
{

/**
 * Change the orientation by making a turn.
 * @param orientation original orientation.
 * @param side where to turn.
 * @return the new orientation.
 * @exception WorldError if orientation or side have wrong values.
 */
Orientation turn(Orientation orientation, Turn side);

/**
 * Change the position by making a movement.
 * @param position original position.
 * @param orientation original orientation.
 * @param movement where to move.
 * @param max maximun values for the position.
 * @return the new position.
 * @exception WorldError if orientation or movement have wrong values.
 */
Position move(Position position, Orientation orientation, Movement movement,
              Position max);

}

#endif // SIMPLEWORLD_MOVEMENT_HPP
