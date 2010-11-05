/**
 * @file simpleworld/egg.cpp
 * A egg in Simple World.
 *
 *  Copyright (C) 2007-2010  Xosé Otero <xoseotero@gmail.com>
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
  : Element(ElementEgg), db::Bug(sw, id), db::Egg(sw, id),
    db::World(sw, this->world_id())
{
}


/**
 * Check if colname is NULL.
 * @param colname name of the column.
 * @return true if colname is NULL, else false.
 */
bool Egg::is_null(const std::string& colname) const
{
  if (colname == "father_id")
    return db::Bug::is_null(colname);
  else if (colname == "orientation")
    return db::World::is_null(colname);
  else
    return false;
}

/**
 * Set colname as NULL.
 * @param colname name of the column.
 */
void Egg::set_null(const std::string& colname)
{
  if (colname == "father_id")
    db::Bug::set_null(colname);
  else if (colname == "orientation")
    db::World::set_null(colname);
}

}
