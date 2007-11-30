/**
 * @file tests/cpu/isa_test.cpp
 * Unit test for CPU::ISA.
 *
 * begin:     Fri, 30 Nov 2007 18:21:30 +0100
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

#define BOOST_TEST_MODULE Unit test for CPU::ISA
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/config.hpp>
#include <simpleworld/ints.hpp>
#include <simpleworld/cpu/isa.hpp>
namespace sw = SimpleWorld;
namespace cpu = SimpleWorld::CPU;


/**
 * .Add a register.
 */
BOOST_AUTO_TEST_CASE(instruction_add_register)
{
  cpu::ISA isa;

  sw::Uint32 code = 0xfd;
  std::string name = "test";

  isa.add_register(code, name);

  BOOST_CHECK_EQUAL(isa.register_code(name), code);
  BOOST_CHECK_EQUAL(isa.register_name(code), name);
  std::vector<sw::Uint8> registers = isa.register_codes();
  BOOST_CHECK(find(registers.begin(), registers.end(), code) !=
              registers.end());

  isa.remove_register(code);
}

/**
 * Add a interrupt.
 */
BOOST_AUTO_TEST_CASE(instruction_add_interrupt)
{
  cpu::ISA isa;

  sw::Uint32 code = 0xfd;
  std::string name = "test";

  isa.add_interrupt(code, name);

  BOOST_CHECK_EQUAL(isa.interrupt_code(name), code);
  BOOST_CHECK_EQUAL(isa.interrupt_name(code), name);
  std::vector<sw::Uint8> interrupts = isa.interrupt_codes();
  BOOST_CHECK(find(interrupts.begin(), interrupts.end(), code) !=
              interrupts.end());

  isa.remove_interrupt(code);
}

/**
 * Add a instruction.
 */
BOOST_AUTO_TEST_CASE(instruction_add_instruction)
{
  cpu::ISA isa;
  cpu::InstructionInfo info;
  info.code = 0xfd;
  info.name = "test";
  info.nregs = 2;
  info.has_inmediate = true;

  isa.add_instruction(info);

  BOOST_CHECK_EQUAL(isa.instruction_code(info.name), info.code);
  BOOST_CHECK_EQUAL(isa.instruction_info(info.code).code, info.code);
  BOOST_CHECK_EQUAL(isa.instruction_info(info.code).name, info.name);
  BOOST_CHECK_EQUAL(isa.instruction_info(info.code).nregs, info.nregs);
  BOOST_CHECK_EQUAL(isa.instruction_info(info.code).has_inmediate,
                    info.has_inmediate);
  BOOST_CHECK_EQUAL(isa.instruction_info(info.code).func, info.func);
  std::vector<sw::Uint8> instructions = isa.instruction_codes();
  BOOST_CHECK(find(instructions.begin(), instructions.end(), info.code) !=
              instructions.end());

  isa.remove_instruction(info.code);
}
