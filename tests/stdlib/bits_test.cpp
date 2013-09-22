/**
 * @file tests/stdlib/bits_test.cpp
 * Unit test for stdlib/bits.swl
 *
 *  Copyright (C) 2009-2013  Xosé Otero <xoseotero@gmail.com>
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

#define BOOST_TEST_MODULE Unit test for stdlib/bits.swl
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/cpu/memory.hpp>
#include <simpleworld/cpu/source.hpp>
#include <simpleworld/cpu/cpu.hpp>
namespace sw = simpleworld;
namespace cpu = simpleworld::cpu;

#include "src/common/fakeisa.hpp"


#define REGISTER(cpu, name) ADDRESS((cpu).isa().register_code(name))
#define MAX_CYCLES 4096


/**
 * Compile the file.
 */
BOOST_AUTO_TEST_CASE(swl_compile)
{
  cpu::Source source(fakeisa);
  source.add_include_path(INCLUDE_DIR);

  source.insert(".include \"stdlib/bits.swl\"");

  source.insert("std_clear");
  source.insert("std_comp");

  cpu::Memory memory;
  BOOST_CHECK_NO_THROW(source.compile(&memory));
}

/**
 * Check std_clear.
 */
BOOST_AUTO_TEST_CASE(std_clear)
{
  cpu::Source source(fakeisa);
  source.add_include_path(INCLUDE_DIR);

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("b main");

  source.insert(".include \"stdlib/bits/clear.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loadi g0 0xF050");
  source.insert("loadi g1 0x2222");
  source.insert("call std_clear");
  source.insert("stop");

  // Space for 16 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x40");

  cpu::Memory registers;
  cpu::Memory memory;
  source.compile(&memory);
  cpu::CPU cpu(fakeisa, &registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0xD050);
}

/**
 * Check std_comp.
 */
BOOST_AUTO_TEST_CASE(std_comp)
{
  cpu::Source source(fakeisa);
  source.add_include_path(INCLUDE_DIR);

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("b main");

  source.insert(".include \"stdlib/bits/comp.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loadi g0 0xF050");
  source.insert("call std_comp");
  source.insert("stop");

  // Space for 16 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x40");

  cpu::Memory registers;
  cpu::Memory memory;
  source.compile(&memory);
  cpu::CPU cpu(fakeisa, &registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0xFFFF0FAF);
}
