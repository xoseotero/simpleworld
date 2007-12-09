/**
 * @file simpleworld/world.cpp
 * Simple World World.
 *
 * begin:     Fri, 13 Jul 2007 07:03:58 +0200
 * last:      $Date$
 *
 *  Copyright (C) 2007  Xosé Antón Otero Ferreira <xoseotero@gmail.com>
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

namespace SimpleWorld
{

/**
 * Check if position > max.
 * @param position position to check.
 * @param max maximun size of the position.
 * @exception Exception if the position is out of range.
 */
inline void exception_outofrange(Position position, Position max)
{
  if (position.x >= max.x or position.y >= max.y)
    throw EXCEPTION(WorldError, boost::str(boost::format(\
"Position (%1%, %2%) is outside of the World (%1%, %2%)")
                                           % position.x
                                           % position.y
                                           % max.x
                                           % max.y));
}

/**
 * Check if position is used.
 * @param terrain where to search.
 * @param position position to check.
 * @exception Exception if the position is used.
 */
inline void exception_used(const boost::multi_array<Element*, 2>& terrain,
                           Position position)
{
  if (terrain[position.x][position.y] != NULL)
    throw EXCEPTION(WorldError, boost::str(boost::format(\
"Position (%1%, %2%) is already used")
                                           % position.x
                                           % position.y));
}

/**
 * Check if position is not used.
 * @param terrain where to search.
 * @param position position to check.
 * @exception Exception if the position is not used.
 */
inline void exception_notused(const boost::multi_array<Element*, 2>& terrain,
                              Position position)
{
  if (terrain[position.x][position.y] == NULL)
    throw EXCEPTION(WorldError, boost::str(boost::format(\
"Position (%1%, %2%) is not used")
                                           % position.x
                                           % position.y));
}


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

World::World(Position size)
  : size_(size), num_elements_(0), terrain_(boost::extents[size.x][size.y])
{
  Position i;

  for (i.x = 0; i.x < size.x; i.x++)
    for (i.y = 0; i.y < size.y; i.y++)
      this->terrain_[i.x][i.y] = NULL;
}


void World::add(Element* element, Position position)
{
  exception_outofrange(position, this->size_);
  exception_used(this->terrain_, position);

  if (element != NULL) {
    this->num_elements_++;

    // make sure the element has the correct position
    element->position = position;
  }

  this->terrain_[position.x][position.y] = element;
}

void World::remove(Position position)
{
  exception_outofrange(position, this->size_);
  exception_notused(this->terrain_, position);

  if (this->terrain_[position.x][position.y] != NULL)
    this->num_elements_--;
  this->terrain_[position.x][position.y] = NULL;
}


bool World::used(Position position) const
{
  exception_outofrange(position, this->size_);

  return this->terrain_[position.x][position.y] != NULL;
}

Element* World::get(Position position) const
{
  exception_outofrange(position, this->size_);
  exception_notused(this->terrain_, position);

  return this->terrain_[position.x][position.y];
}


void World::move(Position oldposition, Position newposition)
{
  exception_outofrange(oldposition);
  exception_notused(this->terrain_, oldposition);
  exception_outofrange(newposition);
  exception_used(this->terrain_, newposition);

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
