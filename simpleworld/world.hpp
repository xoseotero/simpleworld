/**
 * @file simpleworld/world.hpp
 * Simple World World.
 *
 *  Copyright (C) 2006-2011  Xosé Otero <xoseotero@gmail.com>
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
#ifndef SIMPLEWORLD_WORLD_HPP
#define SIMPLEWORLD_WORLD_HPP

#include <map>

#include <boost/multi_array.hpp>

#include <simpleworld/ints.hpp>
#include <simpleworld/types.hpp>
#include <simpleworld/element.hpp>

namespace simpleworld
{

/**
 * Simple World World.
 */
class World
{
public:
  /**
   * Constructor.
   * @param width width of the World.
   * @param height height of the World.
   */
  World(Coord width, Coord height);

  /**
   * Constructor.
   * @param size size of the World.
   */
  World(Position size);


  /**
   * Size of the World.
   * @return the size.
   */
  Position size() const { return this->size_; }

  /**
   * Number of elements in the World.
   * @return the number of elements.
   */
  Uint16 num_elements() const { return this->num_elements_; }

  /**
   * Number of elements in a region of the World.
   * @param start left/top position of the region.
   * @param end right/bottom position of the region.
   * @return the number of elements.
   */
  Uint16 num_elements(Position start, Position end) const;

  /**
   * Number of elements of a specific kind in a region of the World.
   * @param start left/top position of the region.
   * @param end right/bottom position of the region.
   * @param type type of element.
   * @return the number of elements.
   */
  Uint16 num_elements(Position start, Position end, ElementType type) const;


  /**
   * Add a element to the World.
   * @param element element to add.
   * @param position position of the element.
   * @exception WorldError if the position is out of the limits.
   * @exception WorldError if the position is used.
   */
  void add(Element* element, Position position);

  /**
   * Remove a element from the World.
   * @param position position of the element.
   * @exception WorldError if the position is out of the limits.
   * @exception WorldError if the position is not used.
   */
  void remove(Position position);


  /**
   * Check if the position is used.
   * @param position position to check.
   * @return return true if the position is used, false if not.
   * @exception WorldError if the position is out of the limits.
   */
  bool used(Position position) const;

  /**
   * Get a element from the World.
   * @param position position of the element.
   * @return the element.
   * @exception WorldError if the position is out of the limits.
   * @exception WorldError if the position is not used.
   */
  Element* get(Position position) const;

  /**
   * Get a element from the World.
   * @param position position of the element.
   * @return the element.
   * @exception WorldError if the position is out of the limits.
   * @exception WorldError if the position is not used.
   */
  Element* operator[](Position position) const { return this->get(position); };


  /**
   * Get a random unused position.
   * @return the unused position.
   * @exception WorldError if there aren't unused positions.
   */
  Position unused_position() const;

  /**
   * Get a random unused position inside a region of the World.
   * @param start left/top position of the region.
   * @param end right/bottom position of the region.
   * @return the unused position.
   * @exception WorldError if there aren't unused positions.
   */
  Position unused_position(Position start, Position end) const;

  /**
   * Get a random orientation.
   * @return the random orientation.
   */
  static Orientation random_orientation();


  /**
   * Move a element.
   * @param oldposition current position of the element.
   * @param newposition new position of the element.
   * @exception WorldError if the position is out of the limits.
   * @exception WorldError if the old position is not used.
   * @exception WorldError if the new position is used.
   * @exception WorldError if the element can't be moved.
   */
  void move(Position oldposition, Position newposition);

private:
  Position size_;
  Uint16 num_elements_;

  boost::multi_array<Element*, 2> terrain_;
};

}

#endif // SIMPLEWORLD_WORLD_HPP
