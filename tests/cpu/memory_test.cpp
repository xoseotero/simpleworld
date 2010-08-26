/**
 * @file tests/cpu/memory_test.cpp
 * Unit test for CPU::Memory.
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

#define BOOST_TEST_MODULE Unit test for cpu::Memory
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/config.hpp>
#include <simpleworld/ints.hpp>
#include <simpleworld/cpu/memoryerror.hpp>
#include <simpleworld/cpu/word.hpp>
#include <simpleworld/cpu/memory.hpp>
namespace sw = simpleworld;
namespace cpu = simpleworld::cpu;


/**
 * Getting memory exception.
 */
BOOST_AUTO_TEST_CASE(memory_exception)
{
  cpu::Memory memory(16 * 4);
  BOOST_CHECK_THROW(memory.get_word(15 * 4 + 1), cpu::MemoryError);
  BOOST_CHECK_THROW(memory.set_word(15 * 4 + 1, 0), cpu::MemoryError);
}

/**
 * Checking if memory is zeroed.
 */
BOOST_AUTO_TEST_CASE(memory_is_zeroed)
{
  cpu::Memory memory(16 * 4);
  for (sw::Uint8 i = 0; i < 16 * 4; i += 4)
    BOOST_CHECK_EQUAL(memory.get_word(i), 0);
}

/**
 * Getting words from memory.
 */
BOOST_AUTO_TEST_CASE(memory_checking_words)
{
  cpu::Memory memory(16 * 4);
  for (sw::Uint8 i = 0; i < 16 * 4; i += 4)
    memory.set_word(i, i);

  for (sw::Uint8 i = 0; i < 16 * 4; i += 4)
    BOOST_CHECK_EQUAL(memory.get_word(i), i);
}

/**
 * Getting half words from memory.
 */
BOOST_AUTO_TEST_CASE(memory_checking_halfwords)
{
  cpu::Memory memory(16 * 2);
  for (sw::Uint8 i = 0; i < 16 * 2; i += 2)
    memory.set_halfword(i, i);

  for (sw::Uint8 i = 0; i < 16 * 2; i += 2)
    BOOST_CHECK_EQUAL(memory.get_halfword(i), i);
}

/**
 * Getting quarter words from memory.
 */
BOOST_AUTO_TEST_CASE(memory_checking_quarterwords)
{
  cpu::Memory memory(16);
  for (sw::Uint8 i = 0; i < 16; i++)
    memory.set_quarterword(i, i);

  for (sw::Uint8 i = 0; i < 16; i++)
    BOOST_CHECK_EQUAL(memory.get_quarterword(i), i);
}

#if defined(IS_LITTLE_ENDIAN)
/**
 * Getting big endian words.
 */
BOOST_AUTO_TEST_CASE(memory_get_big_endian)
{
  cpu::Memory memory(16 * 4);
  for (sw::Uint8 i = 0; i < 16 * 4; i += 4)
    memory.set_word(i, i);

  for (sw::Uint8 i = 0; i < 16 * 4; i += 4)
    BOOST_CHECK_EQUAL(memory.get_word(i, false),
		      cpu::change_byte_order(static_cast<cpu::Word>(i)));
}

/**
 * Getting big endian half words.
 */
BOOST_AUTO_TEST_CASE(memory_get_big_endian_half)
{
  cpu::Memory memory(16 * 2);
  for (sw::Uint8 i = 0; i < 16 * 2; i += 2)
    memory.set_halfword(i, i);

  for (sw::Uint8 i = 0; i < 16 * 2; i += 2)
    BOOST_CHECK_EQUAL(memory.get_halfword(i, false),
                      cpu::change_byte_order(static_cast<cpu::HalfWord>(i)));
}

/**
 * Setting big endian words.
 */
BOOST_AUTO_TEST_CASE(memory_set_big_endian)
{
  cpu::Memory memory(16 * 4);
  for (sw::Uint8 i = 0; i < 16 * 4; i += 4)
    memory.set_word(i, cpu::change_byte_order(static_cast<cpu::Word>(i)),
		    false);

  for (sw::Uint8 i = 0; i < 16 * 4; i += 4)
    BOOST_CHECK_EQUAL(memory.get_word(i), i);
}

/**
 * Setting big endian half words.
 */
BOOST_AUTO_TEST_CASE(memory_set_big_endian_half)
{
  cpu::Memory memory(16 * 2);
  for (sw::Uint8 i = 0; i < 16 * 2; i += 2)
    memory.set_halfword(i,
			cpu::change_byte_order(static_cast<cpu::HalfWord>(i)),
			false);

  for (sw::Uint8 i = 0; i < 16 * 2; i += 2)
    BOOST_CHECK_EQUAL(memory.get_halfword(i), i);
}
#endif // defined(IS_LITTLE_ENDIAN)

/**
 * Getting a non aligned word.
 */
BOOST_AUTO_TEST_CASE(memory_get_non_aligned)
{
  cpu::Memory memory(2 * 4);
  memory.set_word(0, 0x01234567);
  memory.set_word(4, 0x89abcdef);

  BOOST_CHECK_EQUAL(memory.get_word(2), 0x456789ab);
  BOOST_CHECK_EQUAL(memory.get_word(3), 0x6789abcd);
}

/**
 * Getting a non aligned half word.
 */
BOOST_AUTO_TEST_CASE(memory_get_non_aligned_half)
{
  cpu::Memory memory(2 * 4);
  memory.set_word(0, 0x01234567);
  memory.set_word(4, 0x89abcdef);

  BOOST_CHECK_EQUAL(memory.get_halfword(1), 0x2345);
  BOOST_CHECK_EQUAL(memory.get_halfword(3), 0x6789);
}

/**
 * Getting a non aligned quarter word.
 */
BOOST_AUTO_TEST_CASE(memory_get_non_aligned_quarter)
{
  cpu::Memory memory(1 * 4);
  memory.set_word(0, 0x01234567);

  BOOST_CHECK_EQUAL(memory.get_quarterword(1), 0x23);
  BOOST_CHECK_EQUAL(memory.get_quarterword(3), 0x67);
}

/**
 * Setting a non aligned word.
 */
BOOST_AUTO_TEST_CASE(memory_set_non_aligned)
{
  cpu::Memory memory(2 * 4);
  memory.set_word(0, 0x01234567);
  memory.set_word(2, 0xaabbccdd);
  memory.set_word(3, 0xeeff0000);

  BOOST_CHECK_EQUAL(memory.get_word(0), 0x0123aaee);
  BOOST_CHECK_EQUAL(memory.get_word(4), 0xff000000);
}

/**
 * Setting a non aligned half word.
 */
BOOST_AUTO_TEST_CASE(memory_set_non_aligned_half)
{
  cpu::Memory memory(1 * 4);
  memory.set_halfword(1, 0x0123);
  memory.set_halfword(2, 0x4567);

  BOOST_CHECK_EQUAL(memory.get_word(0), 0x00014567);
}

/**
 * Setting a non aligned quarter word.
 */
BOOST_AUTO_TEST_CASE(memory_set_non_aligned_quarter)
{
  cpu::Memory memory(1 * 4);
  memory.set_quarterword(1, 0x01);
  memory.set_quarterword(2, 0x23);
  memory.set_quarterword(3, 0x45);

  BOOST_CHECK_EQUAL(memory.get_word(0), 0x012345);
}
