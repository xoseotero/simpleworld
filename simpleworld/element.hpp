/**
 * @file simpleworld/element.hpp
 * A element on the World.
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

#ifndef SIMPLEWORLD_ELEMENT_HPP
#define SIMPLEWORLD_ELEMENT_HPP

#include <simpleworld/types.hpp>

namespace simpleworld
{

/**
 * A element on the World.
 */
class Element
{
public:
  /**
   * Constructor.
   * @param type type of element
   */
  Element(ElementType type);

  /**
   * Destructor.
   */
  virtual ~Element() {}


  /**
   * Type of element.
   */
  ElementType type;


  /**
   * Check if the element can be moved.
   * @return if the element can be moved.
   */
  bool movable() const { return this->movable_; }

private:
  bool movable_;
};

}

#endif // SIMPLEWORLD_ELEMENT_HPP
