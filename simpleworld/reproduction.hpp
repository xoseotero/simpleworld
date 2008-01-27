/**
 * @file simpleworld/reproduction.hpp
 * Reproduction of bugs.
 *
 * begin:     Sun, 27 Jan 2008 05:57:39 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2008  Xosé Antón Otero Ferreira <xoseotero@gmail.com>
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

#include <simpleworld/db/code.hpp>

namespace SimpleWorld
{

/**
 * Get a copy of the code but with occasional mutations.
 * @param code the original code.
 * @return the copy of the code.
 */
::SimpleWorld::DB::Code copy_code(const ::SimpleWorld::DB::Code& code,
                                  float mutations_probability);

}

#endif // SIMPLEWORLD_REPRODUCTION_HPP
