/**
 * @file tests/simpleworld/world_test.cpp
 * Unit test for World.
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
  sw::Position p0 = {0, 0}, p1 = {15, 15};
  sw::Element e0(sw::ElementEgg, p0);
  sw::Element e1(sw::ElementBug, p1);

  world.add(&e0, p0);
  world.add(&e1, p1);

  BOOST_CHECK_EQUAL(world.used(p0), true);
  BOOST_CHECK_EQUAL(world.used(p1), true);
  BOOST_CHECK_EQUAL(world.get(p0), &e0);
  BOOST_CHECK_EQUAL(world.get(p1), &e1);
  BOOST_CHECK_EQUAL(world.get(p0)->type, sw::ElementEgg);
  BOOST_CHECK_EQUAL(world.get(p1)->type, sw::ElementBug);
  BOOST_CHECK_EQUAL(world.get(p0)->position, p0);
  BOOST_CHECK_EQUAL(world.get(p1)->position, p1);
}

/**
 * Add elements to the World in invalid positions.
 */
BOOST_AUTO_TEST_CASE(world_add_outside)
{
  sw::World world(16, 32);
  sw::Position p0 = {0, 32}, p1 = {16, 0};
  sw::Element e(sw::ElementFood, p0);

  BOOST_CHECK_THROW(world.add(&e, p0), sw::WorldError);
  BOOST_CHECK_THROW(world.add(&e, p1), sw::WorldError);
}

/**
 * Add elements to the World in used positions.
 */
BOOST_AUTO_TEST_CASE(world_add_used)
{
  sw::World world(16, 16);
  sw::Position p = {0, 0};
  sw::Element e0(sw::ElementFood, p);
  sw::Element e1(sw::ElementFood, p);

  world.add(&e0, p);
  BOOST_CHECK_THROW(world.add(&e1, p), sw::WorldError);
}

/**
 * Remove elements from the World.
 */
BOOST_AUTO_TEST_CASE(world_remove)
{
  sw::World world(16, 16);
  sw::Position p = {0, 0};
  sw::Element e(sw::ElementFood, p);

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
  sw::Position p = {0, 0};

  BOOST_CHECK_THROW(world.remove(p), sw::WorldError);
}

/**
 * Move elements in the World.
 */
BOOST_AUTO_TEST_CASE(world_move)
{
  sw::World world(16, 16);
  sw::Position p0 = {0, 0}, p1 = {1, 2};
  sw::Element e(sw::ElementBug, p0);

  world.add(&e, p0);
  world.move(p0, p1);

  BOOST_CHECK_EQUAL(world.used(p0), false);
  BOOST_CHECK_EQUAL(world.used(p1), true);
  BOOST_CHECK_EQUAL(world.get(p1), &e);
  BOOST_CHECK_EQUAL(e.position, p1);
}

/**
 * Move no movable elements in the World.
 */
BOOST_AUTO_TEST_CASE(world_move_no_movable)
{
  sw::World world(16, 16);
  sw::Position p0 = {0, 0}, p1 = {1, 2};
  sw::Element e0(sw::ElementNothing, p0);
  sw::Element e1(sw::ElementFood, p0);
  sw::Element e2(sw::ElementEgg, p0);

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
//   sw::Position p0 = {0, 0}, p1 = {0, 16}, p2 = {16, 0};
//   sw::Element e(sw::ElementFood, p0);

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
  sw::Position p0 = {0, 0}, p1 = {1, 2};

  BOOST_CHECK_THROW(world.move(p0, p1);, sw::WorldError);
}
