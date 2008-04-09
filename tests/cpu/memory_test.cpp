/**
 * @file tests/cpu/memory_test.cpp
 * Unit test for CPU::Memory.
 *
 * begin:     Fri, 30 Nov 2007 18:20:04 +0100
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

#ifdef IS_LITTLE_ENDIAN
/**
 * Setting big endian words.
 */
BOOST_AUTO_TEST_CASE(memory_set_big_endian)
{
  cpu::Memory memory(16 * 4);
  for (sw::Uint8 i = 0; i < 16 * 4; i += 4)
    memory.set_word(i, cpu::change_byte_order(i), false);

  for (sw::Uint8 i = 0; i < 16 * 4; i += 4)
    BOOST_CHECK_EQUAL(memory.get_word(i), i);
}

/**
 * Getting big endian words.
 */
BOOST_AUTO_TEST_CASE(memory_get_big_endian)
{
  cpu::Memory memory(16 * 4);
  for (sw::Uint8 i = 0; i < 16 * 4; i += 4)
    memory.set_word(i, i);

  for (sw::Uint8 i = 0; i < 16 * 4; i += 4)
    BOOST_CHECK_EQUAL(memory.get_word(i, false), cpu::change_byte_order(i));
}
#endif

/**
 * Getting a non aligned word.
 */
BOOST_AUTO_TEST_CASE(memory_get_non_aligned)
{
  cpu::Memory memory(2 * 4);
  memory.set_word(0, 0x01234567);
  memory.set_word(4, 0x89abcdef);

  BOOST_CHECK_EQUAL(memory.get_word(2), 0x456789ab);
}

/**
 * Setting a non aligned word.
 */
BOOST_AUTO_TEST_CASE(memory_Set_non_aligned)
{
  cpu::Memory memory(2 * 4);
  memory.set_word(0, 0x01234567);
  memory.set_word(2, 0xaabbccdd);

  BOOST_CHECK_EQUAL(memory.get_word(2), 0xaabbccdd);
}
