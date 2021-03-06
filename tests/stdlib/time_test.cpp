/**
 * @file tests/stdlib/time_test.cpp
 * Unit test for stdlib/time.swl
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

#define BOOST_TEST_MODULE Unit test for stdlib/time.swl
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

  source.insert(".include \"stdlib/time.swl\"");

  source.insert("std_time");
  source.insert("std_sleep");

  cpu::Memory memory;
  BOOST_CHECK_NO_THROW(source.compile(&memory));
}

/**
 * Check std_time.
 */
BOOST_AUTO_TEST_CASE(std_time)
{
  cpu::Source source(fakeisa);
  source.add_include_path(INCLUDE_DIR);

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("loada g0 heap");
  source.insert("loadi g1 0x400");
  source.insert("call std_init");
  source.insert("b main");

  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"stdlib/time/time.swl\"");

  // Test
  source.insert(".label main");
  source.insert("call std_time");
  source.insert("push g0");

  // Flag
  source.insert("loadi r5 0xF894");

  source.insert("call std_time");

  source.insert("move g1 g0");
  source.insert("pop g0");

  source.insert("stop");

  // Space for 256 words in the heap
  source.insert(".label heap");
  source.insert(".block 0x400");

  // Space for 32 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x80");

  cpu::Memory registers;
  cpu::Memory memory;
  source.compile(&memory);
  cpu::CPU cpu(fakeisa, &registers, &memory);
  int i = 0;
  while (registers[REGISTER(cpu, "r5")] != 0xF894) {
    cpu.execute(1);

    if (++i == MAX_CYCLES)
      break;
  }
  cpu.timer_interrupt();
  cpu.execute(MAX_CYCLES - i);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0x1);
}

/**
 * Check std_sleep.
 */
BOOST_AUTO_TEST_CASE(std_sleep)
{
  cpu::Source source(fakeisa);
  source.add_include_path(INCLUDE_DIR);

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("loada g0 heap");
  source.insert("loadi g1 0x400");
  source.insert("call std_init");
  source.insert("b main");

  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"stdlib/time/time.swl\"");
  source.insert(".include \"stdlib/time/sleep.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loadi g0 0x1");
  source.insert("call std_sleep");

  source.insert("call std_time");

  source.insert("stop");

  // Space for 256 words in the heap
  source.insert(".label heap");
  source.insert(".block 0x400");

  // Space for 32 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x80");

  cpu::Memory registers;
  cpu::Memory memory;
  source.compile(&memory);
  cpu::CPU cpu(fakeisa, &registers, &memory);
  cpu.execute(MAX_CYCLES);
  cpu.timer_interrupt();
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x1);
}
