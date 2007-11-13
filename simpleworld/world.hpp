/**
 * @file simpleworld/world.hpp
 * Simple World World.
 *
 * begin:     Wed, 27 Dec 2006 18:00:22 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2006-2007  Xosé Otero <xoseotero@users.sourceforge.net>
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
#ifndef __SIMPLEWORLD_WORLD_HPP__
#define __SIMPLEWORLD_WORLD_HPP__

#include <map>

#include <boost/multi_array.hpp>

#include <simpleworld/ints.hpp>
#include <simpleworld/exception.hpp>
#include <simpleworld/element.hpp>

namespace SimpleWorld
{

/**
 * World exception.
 * It's raised if the position is outside of the territory limits.
 */
class InvalidPosition: public std::runtime_error, public Exception
{
public:
  /**
   * Constructor.
   * @param file File where the exception is raised.
   * @param line Line where the exception is raised.
   * @param position Invalid position.
   * @param what Reason of the exception.
   */
  InvalidPosition(std::string file, Uint32 line, Position position,
                  const std::string& what = "Invalid position") throw()
    : runtime_error(what), Exception(file, line), position(position)
  {}

  /**
   * Destructor.
   */
  ~InvalidPosition() throw () {}


  Position position;		/**< Invalid position. */
};

/**
 * World exception.
 * It's raised if the position is not used.
 */
class UnusedPosition: public std::runtime_error, public Exception
{
public:
  /**
   * Constructor.
   * @param file File where the exception is raised.
   * @param line Line where the exception is raised.
   * @param position Wrong position.
   * @param what Reason of the exception.
   */
  UnusedPosition(std::string file, Uint32 line, Position position,
                 const std::string& what = "Unused position") throw()
    : runtime_error(what), Exception(file, line), position(position)
  {}

  /**
   * Destructor.
   */
  ~UnusedPosition() throw () {}


  Position position;		/**< Unused position. */
};

/**
 * World exception.
 * It's raised if the position is occupied.
 */
class UsedPosition: public std::runtime_error, public Exception
{
public:
  /**
   * Constructor.
   * @param file File where the exception is raised.
   * @param line Line where the exception is raised.
   * @param position Wrong position.
   * @param what Reason of the exception.
   */
  UsedPosition(std::string file, Uint32 line, Position position,
	       const std::string& what = "Invalid position") throw()
    : runtime_error(what), Exception(file, line), position(position)
  {}

  /**
   * Destructor.
   */
  ~UsedPosition() throw () {}


  Position position;		/**< Used position. */
};

/**
 * World exception.
 * It's raised if the element is not movable.
 */
class NoMovableElement: public std::runtime_error, public Exception
{
public:
  /**
   * Constructor.
   * @param file File where the exception is raised.
   * @param line Line where the exception is raised.
   * @param element Element not movable.
   * @param what Reason of the exception.
   */
  NoMovableElement(std::string file, Uint32 line, const Element& element,
                   const std::string& what = "Element not movable") throw()
    : runtime_error(what), Exception(file, line), element(element)
  {}

  /**
   * Destructor.
   */
  ~NoMovableElement() throw () {}


  Element element;		/**< Element not movable. */
};


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
   * @return the number of element.
   */
  Uint16 num_elements() const { return this->num_elements_; }


  /**
   * Add a element to the World.
   * @param element element to add.
   * @param position position of the element.
   * @exception InvalidPosition if the position is out of the limits.
   * @exception UsedPosition if the position is used.
   */
  void add(Element* element, Position position);

  /**
   * Remove a element from the World.
   * @param position position of the element.
   * @exception InvalidPosition if the position is out of the limits.
   * @exception UnusedPosition if the position is not used.
   */
  void remove(Position position);


  /**
   * Check if the position is used.
   * @param position position to check.
   * @return return true if the position is used, false if not.
   * @exception InvalidPosition if the position is out of the limits.
   */
  bool used(Position position) const;

  /**
   * Get a element from the World.
   * @param position position of the element.
   * @return the element.
   * @exception InvalidPosition if the position is out of the limits.
   * @exception UnusedPosition if the position is not used.
   */
  Element* get(Position position) const;

  /**
   * Get a element from the World.
   * @param position position of the element.
   * @return the element.
   * @exception InvalidPosition if the position is out of the limits.
   * @exception UnusedPosition if the position is not used.
   */
  Element* operator[](Position position) const { return this->get(position); };


  /**
   * Move a element.
   * @param oldposition current position of the element.
   * @param newposition new position of the element.
   * @exception InvalidPosition if the position is out of the limits.
   * @exception UnusedPosition if the position is not used.
   * @exception UsedPosition if the new position is used.
   * @exception NoMovableElement if the element can't be moved.
   */
  void move(Position oldposition, Position newposition);

private:
  Position size_;
  Uint16 num_elements_;

  boost::multi_array<Element*, 2> terrain_;
};

}

#endif // __SIMPLEWORLD_WORLD_HPP__
