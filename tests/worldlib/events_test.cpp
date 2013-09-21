/**
 * @file tests/worldlib/events_test.cpp
 * Unit test for worldlib/events.swl
 *
 *  Copyright (C) 2012-2013  Xosé Otero <xoseotero@gmail.com>
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

#define BOOST_TEST_MODULE Unit test for worldlib/events.swl
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/cpu/memory.hpp>
#include <simpleworld/cpu/file.hpp>
#include <simpleworld/cpu/source.hpp>
namespace sw = simpleworld;
namespace cpu = simpleworld::cpu;

#include "src/common/fakecpu.hpp"


#define REGISTER(cpu, name) ADDRESS((cpu).isa().register_code(name))
#define MAX_CYCLES 6144


/**
 * Compile a file.
 * @param file file to compile
 * @return object code
 */
cpu::Memory compile(const cpu::File& file)
{
  cpu::Memory registers;
  FakeCPU cpu(&registers, NULL);

  cpu::Source source(cpu.isa(), file);
  source.add_include_path(INCLUDE_DIR);
  cpu::Memory mem;
  source.compile(&mem);

  return mem;
}


/**
 * Compile the file.
 */
BOOST_AUTO_TEST_CASE(swl_compile)
{
  cpu::File source;

  source.insert(".include \"worldlib/events.swl\"");

  source.insert("world_connect");
  source.insert("world_disconnect");
  source.insert("world_events");
  source.insert("world_throw");

  BOOST_CHECK_NO_THROW(compile(source));
}

/**
 * Check WORLD_INIT.
 */
BOOST_AUTO_TEST_CASE(world_init)
{
  cpu::File source;

  // Initialize
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("move fp sp");
  source.insert("loada g0 heap");
  source.insert("loadi g1 0x400");
  source.insert("call std_init");
  source.insert("loadi g0 0x10");
  source.insert("call world_init");
  source.insert("b main");

  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"worldlib/init.swl\"");
  source.insert(".include \"worldlib/def.swl\"");
  source.insert(".include \"worldlib/events/connect.swl\"");
  source.insert(".include \"worldlib/events/loop.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loadi g0 WORLD_INIT");
  source.insert("loada g1 handler");
  source.insert("call world_connect");

  source.insert("call world_loop");

  // Event handler
  source.insert(".label handler");
  source.insert("loadi g0 0xDABC");
  source.insert("stop");

  // Space for 256 words in the heap
  source.insert(".label heap");
  source.insert(".block 0x400");

  // Space for 32 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x80");

  cpu::Memory registers;
  cpu::Memory memory(compile(source));
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0xDABC);
}

/**
 * Check WORLD_IDLE.
 */
BOOST_AUTO_TEST_CASE(world_idle)
{
  cpu::File source;

  // Initialize
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("move fp sp");
  source.insert("loada g0 heap");
  source.insert("loadi g1 0x400");
  source.insert("call std_init");
  source.insert("loadi g0 0x10");
  source.insert("call world_init");
  source.insert("b main");

  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"worldlib/init.swl\"");
  source.insert(".include \"worldlib/def.swl\"");
  source.insert(".include \"worldlib/events/connect.swl\"");
  source.insert(".include \"worldlib/events/loop.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loadi g0 WORLD_IDLE");
  source.insert("loada g1 handler");
  source.insert("call world_connect");

  source.insert("call world_loop");

  // Event handler
  source.insert(".label handler");
  source.insert("loadi g0 0xDABC");
  source.insert("stop");

  // Space for 256 words in the heap
  source.insert(".label heap");
  source.insert(".block 0x400");

  // Space for 32 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x80");

  cpu::Memory registers;
  cpu::Memory memory(compile(source));
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0xDABC);
}

/**
 * Check world_throw.
 */
BOOST_AUTO_TEST_CASE(world_throw)
{
  cpu::File source;

  // Initialize
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("move fp sp");
  source.insert("loada g0 heap");
  source.insert("loadi g1 0x400");
  source.insert("call std_init");
  source.insert("loadi g0 0x10");
  source.insert("call world_init");
  source.insert("b main");

  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"stdlib/def.swl\"");
  source.insert(".include \"worldlib/init.swl\"");
  source.insert(".include \"worldlib/def.swl\"");
  source.insert(".include \"worldlib/events/connect.swl\"");
  source.insert(".include \"worldlib/events/loop.swl\"");
  source.insert(".include \"worldlib/events/throw.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loadi g0 0x911");
  source.insert("loada g1 handler1");
  source.insert("call world_connect");

  source.insert("loadi g0 0x911");
  source.insert("loada g1 handler2");
  source.insert("call world_connect");

  source.insert("loadi g0 WORLD_IDLE");
  source.insert("loada g1 handlerstop");
  source.insert("call world_connect");

  source.insert("loadi g0 0x911");
  source.insert("call world_throw");

  source.insert("call world_loop");

  // Event handlers
  source.insert(".label handler1");
  source.insert("loadi g0 0xDABC");
  source.insert("store g0 data1");
  source.insert("ret");

  source.insert(".label handler2");
  source.insert("loadi g0 0xABCD");
  source.insert("store g0 data2");
  source.insert("ret");

  source.insert(".label handlerstop");
  source.insert("load g0 data1");
  source.insert("load g1 data2");
  source.insert("stop");

  // Data
  source.insert(".label data1");
  source.insert(".block STD_WORDSIZE");
  source.insert(".label data2");
  source.insert(".block STD_WORDSIZE");

  // Space for 256 words in the heap
  source.insert(".label heap");
  source.insert(".block 0x400");

  // Space for 32 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x80");

  cpu::Memory registers;
  cpu::Memory memory(compile(source));
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0xDABC);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0xABCD);
}

/**
 * Check world_disconnect.
 */
BOOST_AUTO_TEST_CASE(world_disconnect)
{
  cpu::File source;
  
  // Initialize
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("move fp sp");
  source.insert("loada g0 heap");
  source.insert("loadi g1 0x400");
  source.insert("call std_init");
  source.insert("loadi g0 0x10");
  source.insert("call world_init");
  source.insert("b main");
  
  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"worldlib/init.swl\"");
  source.insert(".include \"worldlib/def.swl\"");
  source.insert(".include \"worldlib/events/connect.swl\"");
  source.insert(".include \"worldlib/events/disconnect.swl\"");
  source.insert(".include \"worldlib/events/loop.swl\"");
  source.insert(".include \"worldlib/events/throw.swl\"");
  
  // Test
  source.insert(".label main");
  source.insert("loadi g0 0x911");
  source.insert("loada g1 handler");
  source.insert("call world_connect");

  source.insert("loadi g0 WORLD_IDLE");
  source.insert("loada g1 handlerstop");
  source.insert("call world_connect");

  source.insert("loadi g0 0x911");
  source.insert("loada g1 handler");
  source.insert("call world_disconnect");

  source.insert("loadi g0 0x911");
  source.insert("call world_throw");
  
  source.insert("call world_loop");
  
  // Event handlers
  source.insert(".label handler");
  source.insert("loadi g0 0xDABC");
  source.insert("store g0 data");
  source.insert("stop");

  source.insert(".label handlerstop");
  source.insert("load g0 data");
  source.insert("stop");

  // Data
  source.insert(".label data");
  source.insert(".block STD_WORDSIZE");

  // Space for 256 words in the heap
  source.insert(".label heap");
  source.insert(".block 0x400");
  
  // Space for 32 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x80");
  
  cpu::Memory registers;
  cpu::Memory memory(compile(source));
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);
  
  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_NE(registers[REGISTER(cpu, "g0")], 0xDABC);
}
