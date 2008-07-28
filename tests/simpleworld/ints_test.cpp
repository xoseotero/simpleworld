/**
 * @file tests/simpleworld/ints_test.cpp
 * Unit test for ints.hpp.
 *
 * begin:     Sat, 26 Jul 2008 05:07:34 +0200
 * last:      $Date$
 *
 *  Copyright (C) 2008  Xosé Otero <xoseotero@users.sourceforge.net>
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

#define BOOST_TEST_MODULE Unit test for ints.hpp
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <climits>

#include <simpleworld/ints.hpp>
namespace sw = simpleworld;


/**
 * Check that the integers have the correct number of bits.
 */
BOOST_AUTO_TEST_CASE(ints_bits)
{
  // sizeof() doesn't count the number of bytes but the number of chars
  BOOST_CHECK_EQUAL(sizeof(sw::Sint8) * CHAR_BIT, 8);
  BOOST_CHECK_EQUAL(sizeof(sw::Uint8) * CHAR_BIT, 8);
  BOOST_CHECK_EQUAL(sizeof(sw::Sint16) * CHAR_BIT, 16);
  BOOST_CHECK_EQUAL(sizeof(sw::Uint16) * CHAR_BIT, 16);
  BOOST_CHECK_EQUAL(sizeof(sw::Sint32) * CHAR_BIT, 32);
  BOOST_CHECK_EQUAL(sizeof(sw::Uint32) * CHAR_BIT, 32);
  BOOST_CHECK_EQUAL(sizeof(sw::Sint64) * CHAR_BIT, 64);
  BOOST_CHECK_EQUAL(sizeof(sw::Uint64) * CHAR_BIT, 64);
}
