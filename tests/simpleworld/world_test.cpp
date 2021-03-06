/**
 * @file tests/simpleworld/world_test.cpp
 * Unit test for World.
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

#define BOOST_TEST_MODULE Unit test for World
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/ints.hpp>
#include <simpleworld/types.hpp>
#include <simpleworld/element.hpp>
#include <simpleworld/worlderror.hpp>
#include <simpleworld/element.hpp>
#include <simpleworld/world.hpp>
namespace sw = simpleworld;


namespace simpleworld
{

/**
 * Compare two positions.
 * @param a First position.
 * @param b Second position.
 * @return true if they are the same position, false if not.
 */
inline bool operator ==(const sw::Position& a, const sw::Position& b)
{
  return a.x == b.x and a.y == b.y;
}

/**
 * Print a position.
 * 
 * @param p The position.
 * @return true if they are the same position, false if not.
 */
inline std::ostream& operator <<(std::ostream& os, const sw::Position& p)
{
  return os << "(" << p.x << ", " << p.y << ")";
}

};


/**
 * Check if the World is correctly initialized.
 */
BOOST_AUTO_TEST_CASE(world_initialization)
{
  sw::World world(16, 32);

  BOOST_CHECK_EQUAL(world.size().x, 16);
  BOOST_CHECK_EQUAL(world.size().y, 32);
  BOOST_CHECK_EQUAL(world.num_elements(), 0);

  sw::Position i;
  for (i.x = 0; i.x < 16; i.x++)
    for (i.y = 0; i.y < 32; i.y++) {
      BOOST_CHECK_EQUAL(world.used(i), false);
      BOOST_CHECK_THROW(world.get(i), sw::WorldError);
    }
}

/**
 * Add elements to the World.
 */
BOOST_AUTO_TEST_CASE(world_add)
{
  sw::World world(16, 16);
  sw::Position p0(0, 0), p1(15, 15);
  sw::Element e0(sw::ElementEgg);
  sw::Element e1(sw::ElementBug);

  world.add(&e0, p0);
  world.add(&e1, p1);

  BOOST_CHECK_EQUAL(world.used(p0), true);
  BOOST_CHECK_EQUAL(world.used(p1), true);
  BOOST_CHECK_EQUAL(world.get(p0), &e0);
  BOOST_CHECK_EQUAL(world.get(p1), &e1);
  BOOST_CHECK_EQUAL(world.get(p0)->type, sw::ElementEgg);
  BOOST_CHECK_EQUAL(world.get(p1)->type, sw::ElementBug);
}

/**
 * Add elements to the World in invalid positions.
 */
BOOST_AUTO_TEST_CASE(world_add_outside)
{
  sw::World world(16, 32);
  sw::Position p0(0, 32), p1(16, 0);
  sw::Element e(sw::ElementFood);

  BOOST_CHECK_THROW(world.add(&e, p0), sw::WorldError);
  BOOST_CHECK_THROW(world.add(&e, p1), sw::WorldError);
}

/**
 * Add elements to the World in used positions.
 */
BOOST_AUTO_TEST_CASE(world_add_used)
{
  sw::World world(16, 16);
  sw::Position p(0, 0);
  sw::Element e0(sw::ElementFood);
  sw::Element e1(sw::ElementFood);

  world.add(&e0, p);
  BOOST_CHECK_THROW(world.add(&e1, p), sw::WorldError);
}

/**
 * Remove elements from the World.
 */
BOOST_AUTO_TEST_CASE(world_remove)
{
  sw::World world(16, 16);
  sw::Position p(0, 0);
  sw::Element e(sw::ElementFood);

  world.add(&e, p);
  world.remove(p);

  BOOST_CHECK_EQUAL(world.used(p), false);
  BOOST_CHECK_THROW(world.get(p), sw::WorldError);
}

/**
 * Remove elements in not used position from the World.
 */
BOOST_AUTO_TEST_CASE(world_remove_notused)
{
  sw::World world(16, 16);
  sw::Position p(0, 0);

  BOOST_CHECK_THROW(world.remove(p), sw::WorldError);
}

/**
 * Get an unused position.
 */
BOOST_AUTO_TEST_CASE(world_unused_position)
{
  sw::World world(16, 16);
  sw::Position p = world.unused_position();

  BOOST_CHECK_EQUAL(world.used(p), false);
  BOOST_CHECK(p.x >= 0 and p.x < 16);
  BOOST_CHECK(p.y >= 0 and p.y < 16);
}

/**
 * Get an unused position with the World full.
 */
BOOST_AUTO_TEST_CASE(world_unused_position_full)
{
  sw::World world(16, 16);
  sw::Element e(sw::ElementBug);
  sw::Position p;
  for (p.x = 0; p.x < 16; p.x++)
    for (p.y = 0; p.y < 16; p.y++)
      world.add(&e, p);

  BOOST_CHECK_THROW(world.unused_position(), sw::WorldError);
}

/**
 * Get an unused position in a region.
 */
BOOST_AUTO_TEST_CASE(world_unused_position_region)
{
  sw::World world(16, 16);
  sw::Position start(3, 5), end(10, 6);
  sw::Position p = world.unused_position(start, end);

  BOOST_CHECK_EQUAL(world.used(p), false);
  BOOST_CHECK(p.x >= start.x and p.x < end.x);
  BOOST_CHECK(p.y >= start.y and p.y < end.y);
}

/**
 * Get an unused position in a region with the region full.
 */
BOOST_AUTO_TEST_CASE(world_unused_position_region_full)
{
  sw::World world(16, 16);
  sw::Position start(4, 2), end(6, 3);
  sw::Element e(sw::ElementBug);
  sw::Position p;
  for (p.x = start.x; p.x < end.x; p.x++)
    for (p.y = start.y; p.y < end.y; p.y++)
      world.add(&e, p);

  BOOST_CHECK_THROW(world.unused_position(start, end), sw::WorldError);
}

/**
 * Get a random orientation.
 */
BOOST_AUTO_TEST_CASE(world_random_orientation)
{
  sw::Orientation o = sw::World::random_orientation();

  BOOST_CHECK(o == sw::OrientationNorth or
              o == sw::OrientationEast or
              o == sw::OrientationSouth or
              o == sw::OrientationWest);
}

/**
 * Move elements in the World.
 */
BOOST_AUTO_TEST_CASE(world_move)
{
  sw::World world(16, 16);
  sw::Position p0(0, 0), p1(1, 2);
  sw::Element e(sw::ElementBug);

  world.add(&e, p0);
  world.move(p0, p1);

  BOOST_CHECK_EQUAL(world.used(p0), false);
  BOOST_CHECK_EQUAL(world.used(p1), true);
  BOOST_CHECK_EQUAL(world.get(p1), &e);
}

/**
 * Move no movable elements in the World.
 */
BOOST_AUTO_TEST_CASE(world_move_no_movable)
{
  sw::World world(16, 16);
  sw::Position p0(0, 0), p1(1, 2);
  sw::Element e0(sw::ElementNothing);
  sw::Element e1(sw::ElementFood);
  sw::Element e2(sw::ElementEgg);

  world.add(&e0, p0);
  BOOST_CHECK_THROW(world.move(p0, p1), sw::WorldError);
  world.remove(p0);

  world.add(&e1, p0);
  BOOST_CHECK_THROW(world.move(p0, p1), sw::WorldError);
  world.remove(p0);

  world.add(&e2, p0);
  BOOST_CHECK_THROW(world.move(p0, p1), sw::WorldError);
}

// /**
//  * Move elements in the World to invalid positions.
//  */
// BOOST_AUTO_TEST_CASE(world_move_outside)
// {
//   sw::World world(16, 16);
//   sw::Position p0(0, 0), p1(0, 16), p2(16, 0);
//   sw::Element e(sw::ElementFood);

//   world.add(&e, p0);

//   BOOST_CHECK_THROW(world.move(p0, p1);, sw::WorldError);
//   BOOST_CHECK_THROW(world.move(p0, p1);, sw::WorldError);
//   BOOST_CHECK_THROW(world.move(p0, p2);, sw::WorldError);
// }

/**
 * Move elements that not exist in the World.
 */
BOOST_AUTO_TEST_CASE(world_move_notexist)
{
  sw::World world(16, 16);
  sw::Position p0(0, 0), p1(1, 2);

  BOOST_CHECK_THROW(world.move(p0, p1);, sw::WorldError);
}
