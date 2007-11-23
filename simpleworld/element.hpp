/**
 * @file simpleworld/element.hpp
 * A element on the World.
 *
 * begin:     Mon, 05 Mar 2007 09:11:19 +0100
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2007  Xosé Otero <xoseotero@users.sourceforge.net>
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

namespace SimpleWorld
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
   * @param position position of the element.
   */
  Element(ElementType type, Position position);

  /**
   * Destructor.
   */
  virtual ~Element() {}


  /**
   * Type of element.
   */
  ElementType type;


  /**
   * Position of the element.
   */
  Position position;


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
