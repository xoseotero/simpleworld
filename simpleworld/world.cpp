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

#include "world.hpp"

namespace SimpleWorld
{

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


/**
 * Check if position > max.
 * @param position position to check.
 * @param max maximun size of the position.
 * @return if position > max.
 */
static bool operator>(Position position, Position max)
{
  return position.x >= max.x or position.y >= max.y;
}

void World::add(Element* element, Position position)
{
  if (position > this->size_)
    throw InvalidPosition(__FILE__, __LINE__, position);
  if (this->terrain_[position.x][position.y] != NULL)
    UsedPosition(__FILE__, __LINE__, position);

  if (element != NULL) {
    this->num_elements_++;

    // make sure the element has the correct position
    element->position = position;
  }

  this->terrain_[position.x][position.y] = element;
}

void World::remove(Position position)
{
  if (position > this->size_)
    throw InvalidPosition(__FILE__, __LINE__, position);
  if (this->terrain_[position.x][position.y] == NULL)
    UnusedPosition(__FILE__, __LINE__, position);

  if (this->terrain_[position.x][position.y] != NULL)
    this->num_elements_--;
  this->terrain_[position.x][position.y] = NULL;
}


bool World::used(Position position) const
{
  if (position > this->size_)
    throw InvalidPosition(__FILE__, __LINE__, position);

  return this->terrain_[position.x][position.y] != NULL;
}

Element* World::get(Position position) const
{
  if (position > this->size_)
    throw InvalidPosition(__FILE__, __LINE__, position);

  Element* element = this->terrain_[position.x][position.y];
  if (element == NULL)
    throw UnusedPosition(__FILE__, __LINE__, position);

  return element;
}


void World::move(Position oldposition, Position newposition)
{
  Element* element = this->operator[](oldposition);

  // Check if the new position is used
  if (this->terrain_[newposition.x][newposition.y] != NULL)
    throw UsedPosition(__FILE__, __LINE__, newposition);

  if (not element->movable())
    throw NoMovableElement(__FILE__, __LINE__, *element);

  // Update the World
  this->terrain_[oldposition.x][oldposition.y] = NULL;
  this->terrain_[newposition.x][newposition.y] = element;

  // Update the element
  element->position = newposition;
}

}
