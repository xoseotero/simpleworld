/**
 * @file simpleworld/world.cpp
 * Simple World World.
 *
 *  Copyright (C) 2007-2011  Xosé Otero <xoseotero@gmail.com>
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

#include <cstdlib>

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
 * Number of elements in a region of the World.
 * @param start left/top position of the region.
 * @param end right/bottom position of the region.
 * @return the number of elements.
 */
Uint16 World::num_elements(Position start, Position end) const
{
  Uint16 count = 0;
  for (Coord x = start.x; x < end.x; x++)
    for (Coord y = start.y; y < end.y; y++)
      if (this->terrain_[x][y] != NULL)
        count++;

  return count;
}

/**
 * Number of elements of a specific kind in a region of the World.
 * @param start left/top position of the region.
 * @param end right/bottom position of the region.
 * @param type type of element.
 * @return the number of elements.
 */
Uint16 World::num_elements(Position start, Position end,
			   ElementType type) const
{
  Uint16 count = 0;
  for (Coord x = start.x; x < end.x; x++)
    for (Coord y = start.y; y < end.y; y++) {
      Element* const element = this->terrain_[x][y];
      if (element != NULL and element->type == type)
        count++;
    }

  return count;
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

  if (element != NULL)
    this->num_elements_++;

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
 * Get a random unused position.
 * @return the unused position.
 * @exception WorldError if there aren't unused positions.
 */
Position World::unused_position() const
{
  if (this->size_.x * this->size_.y == this->num_elements_)
    throw EXCEPTION(WorldError, "There aren't unused positions");

  Position pos;
  do {
    pos.x = float(this->size_.x) * (std::rand() / (RAND_MAX + 1.0));
    pos.y = float(this->size_.y) * (std::rand() / (RAND_MAX + 1.0));
  } while (this->terrain_[pos.x][pos.y] != NULL);

  return pos;
}

/**
 * Get a random unused position inside a region of the World.
 * @param start left/top position of the region.
 * @param end right/bottom position of the region.
 * @return the unused position.
 * @exception WorldError if there aren't unused positions.
 */
Position World::unused_position(Position start, Position end) const
{
  Position size(end.x - start.x, end.y - start.y);
  if (size.x * size.y == this->num_elements(start, end))
    throw EXCEPTION(WorldError, "There aren't unused positions");

  Position pos;
  do {
    pos.x = start.x + float(size.x) * (std::rand() / (RAND_MAX + 1.0));
    pos.y = start.y + float(size.y) * (std::rand() / (RAND_MAX + 1.0));
  } while (this->terrain_[pos.x][pos.y] != NULL);

  return pos;
}

/**
 * Get a random orientation.
 * @return the random orientation.
 */
Orientation World::random_orientation()
{
  return static_cast<Orientation>(float(4) * (std::rand() / (RAND_MAX + 1.0)));
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
}

}
