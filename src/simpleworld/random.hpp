/**
 * @file src/simpleworld/common.cpp
 * Common code for the parts of Simple World.
 *
 *  Copyright (C) 2008  Xosé Otero <xoseotero@users.sourceforge.net>
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

#ifndef SRC_SIMPLEWORLD_RANDOM_HPP
#define SRC_SIMPLEWORLD_RANDOM_HPP

#include <simpleworld/types.hpp>
namespace sw = simpleworld;

/**
 * Calculate a random position.
 * @param max maximum position.
 * @return the random position.
 */
sw::Position random_position(const sw::Position& max);

/**
 * Calculate a random orientation.
 * @return the random orientation.
 */
sw::Orientation random_orientation();

#endif // SRC_SIMPLEWORLD_RANDOM_HPP
