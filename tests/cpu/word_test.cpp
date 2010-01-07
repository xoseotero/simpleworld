/**
 * @file tests/cpu/word_test.cpp
 * Unit test for cpu/word.hpp.
 *
 *  Copyright (C) 2007-2010  Xosé Otero <xoseotero@users.sourceforge.net>
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
#include <simpleworld/cpu/types.hpp>
#include <simpleworld/cpu/word.hpp>
namespace sw = simpleworld;
namespace cpu = simpleworld::cpu;


///**
// * Getting exception.
// */
//BOOST_AUTO_TEST_CASE(word_exception)
//{
//  BOOST_CHECK_THROW(cpu::get_byte(static_cast<cpu::Word>(0), 4),
//                    cpu::CPUException);
//}

///**
// * Getting exception.
// */
//BOOST_AUTO_TEST_CASE(word_exception_half)
//{
//  BOOST_CHECK_THROW(cpu::get_byte(static_cast<cpu::HalfWord>(0), 2),
//                    cpu::CPUException);
//}


/**
 * Getting bytes from a word.
 */
BOOST_AUTO_TEST_CASE(word_get_bytes)
{
  cpu::Word value = 0xaabbccdd;
#if defined(IS_BIG_ENDIAN)
  BOOST_CHECK_EQUAL(cpu::get_byte(value, 0), 0xaa);
  BOOST_CHECK_EQUAL(cpu::get_byte(value, 1), 0xbb);
  BOOST_CHECK_EQUAL(cpu::get_byte(value, 2), 0xcc);
  BOOST_CHECK_EQUAL(cpu::get_byte(value, 3), 0xdd);
#elif defined(IS_LITTLE_ENDIAN)
  BOOST_CHECK_EQUAL(cpu::get_byte(value, 0), 0xdd);
  BOOST_CHECK_EQUAL(cpu::get_byte(value, 1), 0xcc);
  BOOST_CHECK_EQUAL(cpu::get_byte(value, 2), 0xbb);
  BOOST_CHECK_EQUAL(cpu::get_byte(value, 3), 0xaa);
#else
#error endianness not defined
#endif
}

/**
 * Getting bytes from a half word.
 */
BOOST_AUTO_TEST_CASE(word_get_bytes_half)
{
  cpu::HalfWord value = 0xaabb;
#if defined(IS_BIG_ENDIAN)
  BOOST_CHECK_EQUAL(cpu::get_byte(value, 0), 0xaa);
  BOOST_CHECK_EQUAL(cpu::get_byte(value, 1), 0xbb);
#elif defined(IS_LITTLE_ENDIAN)
  BOOST_CHECK_EQUAL(cpu::get_byte(value, 0), 0xbb);
  BOOST_CHECK_EQUAL(cpu::get_byte(value, 1), 0xaa);
#else
#error endianness not defined
#endif
}


/**
 * Setting bytes to a word.
 */
BOOST_AUTO_TEST_CASE(word_set_bytes)
{
  cpu::Word value = 0;
#if defined(IS_BIG_ENDIAN)
  cpu::set_byte(&value, 0, 0xaa);
  cpu::set_byte(&value, 1, 0xbb);
  cpu::set_byte(&value, 2, 0xcc);
  cpu::set_byte(&value, 3, 0xdd);
#elif defined(IS_LITTLE_ENDIAN)
  cpu::set_byte(&value, 0, 0xdd);
  cpu::set_byte(&value, 1, 0xcc);
  cpu::set_byte(&value, 2, 0xbb);
  cpu::set_byte(&value, 3, 0xaa);
#else
#error endianness not defined
#endif

  BOOST_CHECK_EQUAL(value, 0xaabbccdd);
}

/**
 * Setting bytes to a half word.
 */
BOOST_AUTO_TEST_CASE(word_set_bytes_half)
{
  cpu::HalfWord value = 0;
#if defined(IS_BIG_ENDIAN)
  cpu::set_byte(&value, 0, 0xaa);
  cpu::set_byte(&value, 1, 0xbb);
#elif defined(IS_LITTLE_ENDIAN)
  cpu::set_byte(&value, 0, 0xbb);
  cpu::set_byte(&value, 1, 0xaa);
#else
#error endianness not defined
#endif

  BOOST_CHECK_EQUAL(value, 0xaabb);
}


/**
 * Changing byte order.
 */
BOOST_AUTO_TEST_CASE(word_change_endianness)
{
  BOOST_CHECK_EQUAL(0x01234567,
		    cpu::change_byte_order(static_cast<cpu::Word>(0x67452301)));
}

/**
 * Changing byte order.
 */
BOOST_AUTO_TEST_CASE(word_change_endianness_half)
{
  BOOST_CHECK_EQUAL(0x0123,
		    cpu::change_byte_order(static_cast<cpu::HalfWord>(0x2301)));
}
