/**
 * @file simpleworld/reproduction.hpp
 * Reproduction of bugs.
 *
 * begin:     Sun, 27 Jan 2008 05:57:39 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2008  Xosé Otero <xoseotero@gmail.com>
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
#include <simpleworld/db/code.hpp>

namespace simpleworld
{

/**
 * Get a copy of the code but with occasional mutations.
 * @param code the original code.
 * @param probability probability to happen a mutation.
 * @param egg true if it's a egg, false if it's a bug.
 * @param time the current time.
 * @return the copy of the code.
 */
db::Code mutate(const db::Code& code, float probability, bool egg, Time time);

}

#endif // SIMPLEWORLD_REPRODUCTION_HPP
