/**
 * @file tests/stdlib/queue_test.cpp
 * Unit test for stdlib/queue.swl
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

#define BOOST_TEST_MODULE Unit test for stdlib/queue.swl
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

  source.insert(".include \"stdlib/queue.swl\"");

  source.insert("STD_QUEUE_STRUCT");
  source.insert("std_queue");
  source.insert("std_queuefree");
  source.insert("std_queueempty");
  source.insert("std_queuepush");
  source.insert("std_queuepop");
  source.insert("std_queueiterator");

  cpu::Memory memory;
  BOOST_CHECK_NO_THROW(source.compile(&memory));
}

/**
 * Check std_queue.
 * The memory used for the queue must be returned when freed.
 */
BOOST_AUTO_TEST_CASE(std_queue)
{
  cpu::Source source(fakeisa);
  source.add_include_path(INCLUDE_DIR);

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("move fp sp");
  source.insert("loada g0 heap");
  source.insert("loadi g1 0x400");
  source.insert("call std_init");
  source.insert("b main");

  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"stdlib/alloc/def.swl\"");
  source.insert(".include \"stdlib/alloc/info.swl\"");
  source.insert(".include \"stdlib/queue/queue.swl\"");
  source.insert(".include \"stdlib/queue/free.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 minfo");
  source.insert("call std_minfo");
  source.insert("loadri g0 g0 STD_MINFO_FREE");
  source.insert("push g0");

  source.insert("call std_queue");
  source.insert("push g0");

  source.insert("loada g0 minfo");
  source.insert("call std_minfo");
  source.insert("loadri g0 g0 STD_MINFO_FREE");
  source.insert("push g0");

  source.insert("loadri g0 fp 0x4");
  source.insert("call std_queuefree");

  source.insert("loada g0 minfo");
  source.insert("call std_minfo");
  source.insert("loadri g2 g0 STD_MINFO_FREE");
  source.insert("loadri g0 fp 0x0");
  source.insert("loadri g1 fp 0x8");

  source.insert("stop");

  // Space for the minfo struct
  source.insert(".label minfo");
  source.insert(".block STD_MINFO_STRUCT");

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

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")],
                    registers[REGISTER(cpu, "g2")]);
  BOOST_CHECK_GT(registers[REGISTER(cpu, "g0")],
                 registers[REGISTER(cpu, "g1")]);
  BOOST_CHECK_LT(registers[REGISTER(cpu, "g1")],
                 registers[REGISTER(cpu, "g2")]);
}

/**
 * Check std_queueempty.
 */
BOOST_AUTO_TEST_CASE(std_queueempty)
{
  cpu::Source source(fakeisa);
  source.add_include_path(INCLUDE_DIR);

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("move fp sp");
  source.insert("loada g0 heap");
  source.insert("loadi g1 0x400");
  source.insert("call std_init");
  source.insert("b main");

  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"stdlib/queue/queue.swl\"");
  source.insert(".include \"stdlib/queue/empty.swl\"");
  source.insert(".include \"stdlib/queue/push.swl\"");

  // Test
  source.insert(".label main");
  source.insert("call std_queue");
  source.insert("push g0");

  source.insert("call std_queueempty");
  source.insert("push g0");

  source.insert("loadri g0 fp 0x0");
  source.insert("loadi g1 0x1");
  source.insert("call std_queuepush");

  source.insert("loadri g0 fp 0x0");
  source.insert("call std_queueempty");

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
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_NE(registers[REGISTER(cpu, "g0")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0x0);
}

/**
 * Check std_queuepush and std_queuepop.
 */
BOOST_AUTO_TEST_CASE(std_queuepush)
{
  cpu::Source source(fakeisa);
  source.add_include_path(INCLUDE_DIR);

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("move fp sp");
  source.insert("loada g0 heap");
  source.insert("loadi g1 0x400");
  source.insert("call std_init");
  source.insert("b main");

  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"stdlib/queue/queue.swl\"");
  source.insert(".include \"stdlib/queue/push.swl\"");
  source.insert(".include \"stdlib/queue/pop.swl\"");

  // Test
  source.insert(".label main");
  source.insert("call std_queue");
  source.insert("push g0");

  source.insert("loadi g1 0xF3F");
  source.insert("call std_queuepush");

  source.insert("loadri g0 fp 0x0");
  source.insert("loadi g1 0x3F3");
  source.insert("call std_queuepush");

  source.insert("loadri g0 fp 0x0");
  source.insert("call std_queuepop");
  source.insert("push g0");

  source.insert("loadri g0 fp 0x0");
  source.insert("call std_queuepop");

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
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0xF3F);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0x3F3);
}

/**
 * Check std_queueiterator.
 */
BOOST_AUTO_TEST_CASE(std_queueiterator)
{
  cpu::Source source(fakeisa);
  source.add_include_path(INCLUDE_DIR);

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("move fp sp");
  source.insert("loada g0 heap");
  source.insert("loadi g1 0x400");
  source.insert("call std_init");
  source.insert("b main");

  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"stdlib/node/def.swl\"");
  source.insert(".include \"stdlib/node/next.swl\"");
  source.insert(".include \"stdlib/queue/queue.swl\"");
  source.insert(".include \"stdlib/queue/push.swl\"");
  source.insert(".include \"stdlib/queue/iterator.swl\"");

  // Test
  source.insert(".label main");
  source.insert("call std_queue");
  source.insert("move r0 g0");

  source.insert("loadi g1 0xF3F");
  source.insert("call std_queuepush");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x3F3");
  source.insert("call std_queuepush");

  source.insert("move g0 r0");
  source.insert("call std_queueiterator");
  source.insert("move r1 g0");

  source.insert("loadri r2 g0 STD_NODE_DATA");

  source.insert("loadi g1 0x1");
  source.insert("call std_next");
  source.insert("loadri r3 g0 STD_NODE_DATA");

  source.insert("move g0 r2");
  source.insert("move g1 r3");

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

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0xF3F);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0x3F3);
}
