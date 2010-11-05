/**
 * @file simpleworld/reproduction.hpp
 * Reproduction of bugs.
 *
 *  Copyright (C) 2008-2010  Xosé Otero <xoseotero@gmail.com>
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

#ifndef SIMPLEWORLD_REPRODUCTION_HPP
#define SIMPLEWORLD_REPRODUCTION_HPP

#include <simpleworld/types.hpp>
#include <simpleworld/db/bug.hpp>

namespace simpleworld
{

/**
 * Get a copy of the code but with occasional mutations.
 * @param bug bug to mutate.
 * @param probability probability to happen a mutation.
 * @param time the current time.
 * @return if the code was mutated.
*/
bool mutate(db::Bug* bug, float probability, Time time);

}

#endif // SIMPLEWORLD_REPRODUCTION_HPP
