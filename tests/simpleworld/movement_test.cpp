/**
 * @file tests/simpleworld/movement_test.cpp
 * Unit test for movement.hpp.
 *
 * begin:     Sun, 09 Dec 2007 08:35:04 +0100
 * last:      $Date$
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

#define BOOST_TEST_MODULE Unit test for movement.hpp
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/types.hpp>
#include <simpleworld/movement.hpp>
namespace sw = SimpleWorld;


namespace SimpleWorld
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
 * Change the orientation.
 */
BOOST_AUTO_TEST_CASE(movement_turn)
{
  sw::Orientation o = sw::OrientationNorth;

  // 90 degrees turns
  BOOST_CHECK_EQUAL(sw::turn(o, sw::TurnLeft), sw::OrientationWest);
  BOOST_CHECK_EQUAL(sw::turn(o, sw::TurnRight), sw::OrientationEast);
  // 180 degrees turns
  BOOST_CHECK_EQUAL(sw::turn(sw::turn(o, sw::TurnRight), sw::TurnRight),
                    sw::OrientationSouth);
  BOOST_CHECK_EQUAL(sw::turn(sw::turn(o, sw::TurnLeft), sw::TurnLeft),
                    sw::OrientationSouth);
  // 360 degrees turns
  BOOST_CHECK_EQUAL(sw::turn(sw::turn(sw::turn(sw::turn(o, sw::TurnRight),
                                               sw::TurnRight),
                                      sw::TurnRight),
                             sw::TurnRight),
                    sw::OrientationNorth);
  BOOST_CHECK_EQUAL(sw::turn(sw::turn(sw::turn(sw::turn(o, sw::TurnLeft),
                                               sw::TurnLeft),
                                      sw::TurnLeft),
                             sw::TurnLeft),
                    sw::OrientationNorth);
  // 0 degrees turns
  BOOST_CHECK_EQUAL(sw::turn(sw::turn(o, sw::TurnRight), sw::TurnLeft),
                    sw::OrientationNorth);
  BOOST_CHECK_EQUAL(sw::turn(sw::turn(o, sw::TurnLeft), sw::TurnRight),
                    sw::OrientationNorth);
}

/**
 * Change the position.
 */
BOOST_AUTO_TEST_CASE(movement_move)
{
  sw::Position p = {4, 4};
  sw::Position front_north = {3, 4};
  sw::Position behind_north = {5, 4};
  sw::Position front_east = {4, 5};
  sw::Position behind_east = {4, 3};
  sw::Position front_south = {5, 4};
  sw::Position behind_south = {3, 4};
  sw::Position front_west = {4, 3};
  sw::Position behind_west = {4, 5};
  sw::Position max = {8, 8};

  BOOST_CHECK_EQUAL(sw::move(p, sw::OrientationNorth, sw::MoveForward, max),
                    front_north);
  BOOST_CHECK_EQUAL(sw::move(p, sw::OrientationNorth, sw::MoveBackward, max),
                    behind_north);
  BOOST_CHECK_EQUAL(sw::move(p, sw::OrientationEast, sw::MoveForward, max),
                    front_east);
  BOOST_CHECK_EQUAL(sw::move(p, sw::OrientationEast, sw::MoveBackward, max),
                    behind_east);
  BOOST_CHECK_EQUAL(sw::move(p, sw::OrientationSouth, sw::MoveForward, max),
                    front_south);
  BOOST_CHECK_EQUAL(sw::move(p, sw::OrientationSouth, sw::MoveBackward, max),
                    behind_south);
  BOOST_CHECK_EQUAL(sw::move(p, sw::OrientationWest, sw::MoveForward, max),
                    front_west);
  BOOST_CHECK_EQUAL(sw::move(p, sw::OrientationWest, sw::MoveBackward, max),
                    behind_west);

  BOOST_CHECK_EQUAL(sw::move(sw::move(p,
                                      sw::OrientationNorth,
                                      sw::MoveBackward,
                                      max),
                             sw::OrientationNorth,
                             sw::MoveForward,
                             max),
                    p);
}
