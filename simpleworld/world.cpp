/**
 * @file simpleworld/world.cpp
 * Simple World World.
 *
 * begin:     Fri, 13 Jul 2007 07:03:58 +0200
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

#include <boost/format.hpp>

#include "worlderror.hpp"
#include "world.hpp"

namespace simpleworld
{

/**
 * Check if position > max.
 * @param position position to check.
 * @param max maximun size of the position.
 * @exception Exception if the position is out of range.
 */
#define CHECK_OUTOFRANGE(position, max) \
  if (position.x >= max.x or position.y >= max.y) \
    throw EXCEPTION(WorldError, boost::str(boost::format(\
"Position (%1%, %2%) is outside of the World (%3%, %4%)") \
                                           % position.x \
                                           % position.y \
                                           % max.x \
                                           % max.y));

/**
 * Check if position is used.
 * @param terrain where to search.
 * @param position position to check.
 * @exception Exception if the position is used.
 */
#define CHECK_USED(terrain, position) \
  if (terrain[position.x][position.y] != NULL) \
    throw EXCEPTION(WorldError, boost::str(boost::format(\
"Position (%1%, %2%) is already used") \
                                           % position.x \
                                           % position.y));

/**
 * Check if position is not used.
 * @param terrain where to search.
 * @param position position to check.
 * @exception Exception if the position is not used.
 */
#define CHECK_NOTUSED(terrain, position) \
  if (terrain[position.x][position.y] == NULL) \
    throw EXCEPTION(WorldError, boost::str(boost::format(\
"Position (%1%, %2%) is not used") \
                                           % position.x \
                                           % position.y));


/**
 * Constructor.
 * @param width width of the World.
 * @param height height of the World.
 */
World::World(Coord width, Coord height)
  : num_elements_(0), terrain_(boost::extents[width][height])
{
  this->size_.x = width;
  this->size_.y = height;

  Position i;

  for (i.x = 0; i.x < width; i.x++)
    for (i.y = 0; i.y < height; i.y++)
      this->terrain_[i.x][i.y] = NULL;
}

/**
 * Constructor.
 * @param size size of the World.
 */
World::World(Position size)
  : size_(size), num_elements_(0), terrain_(boost::extents[size.x][size.y])
{
  Position i;

  for (i.x = 0; i.x < size.x; i.x++)
    for (i.y = 0; i.y < size.y; i.y++)
      this->terrain_[i.x][i.y] = NULL;
}


/**
 * Add a element to the World.
 * @param element element to add.
 * @param position position of the element.
 * @exception WorldError if the position is out of the limits.
 * @exception WorldError if the position is used.
 */
void World::add(Element* element, Position position)
{
  CHECK_OUTOFRANGE(position, this->size_);
  CHECK_USED(this->terrain_, position);

  if (element != NULL) {
    this->num_elements_++;

    // make sure the element has the correct position
    element->position = position;
  }

  this->terrain_[position.x][position.y] = element;
}

/**
 * Remove a element from the World.
 * @param position position of the element.
 * @exception WorldError if the position is out of the limits.
 * @exception WorldError if the position is not used.
 */
void World::remove(Position position)
{
  CHECK_OUTOFRANGE(position, this->size_);
  CHECK_NOTUSED(this->terrain_, position);

  this->num_elements_--;
  this->terrain_[position.x][position.y] = NULL;
}


/**
 * Check if the position is used.
 * @param position position to check.
 * @return return true if the position is used, false if not.
 * @exception WorldError if the position is out of the limits.
 */
bool World::used(Position position) const
{
  CHECK_OUTOFRANGE(position, this->size_);

  return this->terrain_[position.x][position.y] != NULL;
}

/**
 * Get a element from the World.
 * @param position position of the element.
 * @return the element.
 * @exception WorldError if the position is out of the limits.
 * @exception WorldError if the position is not used.
 */
Element* World::get(Position position) const
{
  CHECK_OUTOFRANGE(position, this->size_);
  CHECK_NOTUSED(this->terrain_, position);

  return this->terrain_[position.x][position.y];
}


/**
 * Move a element.
 * @param oldposition current position of the element.
 * @param newposition new position of the element.
 * @exception WorldError if the position is out of the limits.
 * @exception WorldError if the old position is not used.
 * @exception WorldError if the new position is used.
 * @exception WorldError if the element can't be moved.
 */
void World::move(Position oldposition, Position newposition)
{
  CHECK_OUTOFRANGE(oldposition, this->size_);
  CHECK_NOTUSED(this->terrain_, oldposition);
  CHECK_OUTOFRANGE(newposition, this->size_);
  CHECK_USED(this->terrain_, newposition);

  Element* element = this->get(oldposition);
  if (not element->movable())
    throw EXCEPTION(WorldError, boost::str(boost::format(\
"Element in position (%1%, %2%) is not movable")
                                           % oldposition.x
                                           % oldposition.y));

  // Update the World
  this->terrain_[oldposition.x][oldposition.y] = NULL;
  this->terrain_[newposition.x][newposition.y] = element;

  // Update the element
  element->position = newposition;
}

}
