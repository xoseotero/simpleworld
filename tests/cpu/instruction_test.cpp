/**
 * @file tests/cpu/instruction_test.cpp
 * Unit test for CPU::Instruction.
 *
 * begin:     Fri, 30 Nov 2007 18:20:52 +0100
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

#include <algorithm>

#define BOOST_TEST_MODULE Unit test for CPU::Instruction
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/config.hpp>
#include <simpleworld/ints.hpp>
#include <simpleworld/cpu/instruction.hpp>
namespace sw = SimpleWorld;
namespace cpu = SimpleWorld::CPU;


/**
 * Encode a instruction.
 */
BOOST_AUTO_TEST_CASE(instruction_encode)
{
  cpu::Instruction inst;
  inst.code = 0x12;
  inst.first = 0x3;
  inst.second = 0x4;
  inst.address = 0x5678;
#ifdef IS_BIG_ENDIAN
  BOOST_CHECK_EQUAL(cpu::Instruction::encode(inst), 0x12345678);
#else
  BOOST_CHECK_EQUAL(cpu::Instruction::encode(inst), 0x78563412);
#endif
}

/**
 * Decode a instruction.
 */
BOOST_AUTO_TEST_CASE(instruction_decode)
{
#ifdef IS_BIG_ENDIAN
  sw::Uint32 inst_coded = 0x12345678;
#else
  sw::Uint32 inst_coded = 0x78563412;
#endif
  cpu::Instruction inst = cpu::Instruction::decode(inst_coded);

  BOOST_CHECK_EQUAL(inst.code, 0x12);
  BOOST_CHECK_EQUAL(inst.first, 0x3);
  BOOST_CHECK_EQUAL(inst.second, 0x4);
  BOOST_CHECK_EQUAL(inst.address, 0x5678);
}
