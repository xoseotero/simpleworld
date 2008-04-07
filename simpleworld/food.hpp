/**
 * @file simpleworld/food.hpp
 * Food in Simple World.
 *
 * begin:     Sat, 21 Jul 2007 12:04:50 +0200
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

#ifndef SIMPLEWORLD_FOOD_HPP
#define SIMPLEWORLD_FOOD_HPP

#include <simpleworld/db/food.hpp>

namespace simpleworld
{

class SimpleWorld;

class Food: public db::Food
{
public:
  /**
   * Constructor.
   * @param sw world where the bug lives.
   * @param id id of the food.
   * @exception DBException if there is a error in the database.
   */
  Food(SimpleWorld* sw, db::ID id);
};

}

#endif // SIMPLEWORLD_FOOD_HPP
