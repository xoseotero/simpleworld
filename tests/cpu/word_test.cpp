/**
 * @file tests/cpu/word_test.cpp
 * Unit test for cpu/word.hpp.
 *
 * begin:     Fri, 30 Nov 2007 18:19:17 +0100
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

#define BOOST_TEST_MODULE Unit test for cpu/word.hpp
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/config.hpp>
#include <simpleworld/ints.hpp>
#include <simpleworld/cpu/word.hpp>
namespace sw = SimpleWorld;
namespace cpu = SimpleWorld::CPU;


///**
// * Getting exception.
// */
//BOOST_AUTO_TEST_CASE(word_exception)
//{
//  cpu::get_byte(0, 4);
//}

/**
 * Getting bytes.
 */
BOOST_AUTO_TEST_CASE(word_get_bytes)
{
  sw::Uint32 value = 0xaabbccdd;
#ifdef IS_BIG_ENDIAN
  BOOST_CHECK_EQUAL(cpu::get_byte(value, 0), 0xaa);
  BOOST_CHECK_EQUAL(cpu::get_byte(value, 1), 0xbb);
  BOOST_CHECK_EQUAL(cpu::get_byte(value, 2), 0xcc);
  BOOST_CHECK_EQUAL(cpu::get_byte(value, 3), 0xdd);
#else
  BOOST_CHECK_EQUAL(cpu::get_byte(value, 0), 0xdd);
  BOOST_CHECK_EQUAL(cpu::get_byte(value, 1), 0xcc);
  BOOST_CHECK_EQUAL(cpu::get_byte(value, 2), 0xbb);
  BOOST_CHECK_EQUAL(cpu::get_byte(value, 3), 0xaa);
#endif
}

/**
 * Setting bytes.
 */
BOOST_AUTO_TEST_CASE(word_set_bytes)
{
  sw::Uint32 value = 0;
#ifdef IS_BIG_ENDIAN
  cpu::set_byte(&value, 0, 0xaa);
  cpu::set_byte(&value, 1, 0xbb);
  cpu::set_byte(&value, 2, 0xcc);
  cpu::set_byte(&value, 3, 0xdd);
#else
  cpu::set_byte(&value, 0, 0xdd);
  cpu::set_byte(&value, 1, 0xcc);
  cpu::set_byte(&value, 2, 0xbb);
  cpu::set_byte(&value, 3, 0xaa);
#endif

  BOOST_CHECK_EQUAL(value, 0xaabbccdd);
}

/**
 * Changing byte order.
 */
BOOST_AUTO_TEST_CASE(word_change_endianness)
{
  BOOST_CHECK_EQUAL(0x01234567, cpu::change_byte_order(0x67452301));
}

/**
 * Changing byte order (middle).
 */
BOOST_AUTO_TEST_CASE(word_change_endianness_middle)
{
  BOOST_CHECK_EQUAL(0x01234567, cpu::change_byte_order_middle(0x23016745));
}
