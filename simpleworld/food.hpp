/**
 * @file simpleworld/food.hpp
 * Food in Simple World.
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

#ifndef SIMPLEWORLD_FOOD_HPP
#define SIMPLEWORLD_FOOD_HPP

#include <simpleworld/element.hpp>
#include <simpleworld/db/food.hpp>
#include <simpleworld/db/world.hpp>

namespace simpleworld
{

class SimpleWorld;

class Food: public Element, public db::Food, public db::World
{
public:
  /**
   * Constructor.
   * @param sw world where the bug lives.
   * @param id id of the food.
   * @exception DBException if there is a error in the database.
   */
  Food(SimpleWorld* sw, db::ID id);


  /**
   * Get the id of the food.
   * @return the id of the food.
   */
  db::ID id() const { return db::Food::id_; }

  /**
   * Set the id of the food.
   * @param id the new id.
   * @exception DBException if there is an error with the update.
   */
  void id(db::ID id) { db::Food::id(id); }


  /**
   * Check if colname is NULL.
   * @param colname name of the column.
   * @return true if colname is NULL, else false.
   */
  bool is_null(const std::string& colname) const;

  /**
   * Set colname as NULL.
   * @param colname name of the column.
   */
  void set_null(const std::string& colname);
};

}

#endif // SIMPLEWORLD_FOOD_HPP
