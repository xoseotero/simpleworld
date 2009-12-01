/**
 * @file simpleworld/egg.cpp
 * A egg in Simple World.
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

#include <simpleworld/simpleworld.hpp>

#include "egg.hpp"

namespace simpleworld
{

/**
 * Constructor.
 * @param sw world where the bug lives.
 * @param id id of the egg.
 * @exception DBException if there is a error in the database.
 */
Egg::Egg(SimpleWorld* sw, db::ID id)
  : db::Egg(sw, id)
{
}

}
