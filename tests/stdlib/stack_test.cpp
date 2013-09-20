/**
 * @file tests/stdlib/stack_test.cpp
 * Unit test for stdlib/stack.swl
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

#define BOOST_TEST_MODULE Unit test for stdlib/stack.swl
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/cpu/memory.hpp>
#include <simpleworld/cpu/memory_file.hpp>
#include <simpleworld/cpu/file.hpp>
#include <simpleworld/cpu/source.hpp>
namespace sw = simpleworld;
namespace cpu = simpleworld::cpu;

#include "src/common/fakecpu.hpp"


#define REGISTER(cpu, name) ADDRESS((cpu).isa().register_code(name))
#define CPU_SAVE (TESTOUTPUT "stack.swo")
#define MAX_CYCLES 4096


/**
 * Compile a file to CPU_SAVE.
 * @param file file to compile
 */
void compile(const cpu::File& file)
{
  cpu::Memory registers;
  FakeCPU cpu(&registers, NULL);

  cpu::Source source(cpu.isa(), file);
  source.add_include_path(INCLUDE_DIR);
  source.compile(CPU_SAVE);
}


/**
 * Compile the file.
 */
BOOST_AUTO_TEST_CASE(swl_compile)
{
  cpu::File source;

  source.insert(".include \"stdlib/stack.swl\"");

  source.insert("STD_STACK_STRUCT");
  source.insert("std_stack");
  source.insert("std_stackfree");
  source.insert("std_stackempty");
  source.insert("std_stackpush");
  source.insert("std_stackpop");
  source.insert("std_stackiterator");

  BOOST_CHECK_NO_THROW(compile(source));
}

/**
 * Check std_stack.
 * The memory used for the stack must be returned when freed.
 */
BOOST_AUTO_TEST_CASE(std_stack)
{
  cpu::File source;

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
  source.insert(".include \"stdlib/stack/stack.swl\"");
  source.insert(".include \"stdlib/stack/free.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 minfo");
  source.insert("call std_minfo");
  source.insert("loadri g0 g0 STD_MINFO_FREE");
  source.insert("push g0");

  source.insert("call std_stack");
  source.insert("push g0");

  source.insert("loada g0 minfo");
  source.insert("call std_minfo");
  source.insert("loadri g0 g0 STD_MINFO_FREE");
  source.insert("push g0");

  source.insert("loadri g0 fp 0x4");
  source.insert("call std_stackfree");

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

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
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
 * Check std_stackempty.
 */
BOOST_AUTO_TEST_CASE(std_stackempty)
{
  cpu::File source;

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("move fp sp");
  source.insert("loada g0 heap");
  source.insert("loadi g1 0x400");
  source.insert("call std_init");
  source.insert("b main");

  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"stdlib/stack/stack.swl\"");
  source.insert(".include \"stdlib/stack/empty.swl\"");
  source.insert(".include \"stdlib/stack/push.swl\"");

  // Test
  source.insert(".label main");
  source.insert("call std_stack");
  source.insert("push g0");

  source.insert("call std_stackempty");
  source.insert("push g0");

  source.insert("loadri g0 fp 0x0");
  source.insert("loadi g1 0x1");
  source.insert("call std_stackpush");

  source.insert("loadri g0 fp 0x0");
  source.insert("call std_stackempty");

  source.insert("move g1 g0");
  source.insert("pop g0");

  source.insert("stop");

  // Space for 256 words in the heap
  source.insert(".label heap");
  source.insert(".block 0x400");

  // Space for 32 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x80");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_NE(registers[REGISTER(cpu, "g0")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0x0);
}

/**
 * Check std_stackpush and std_stackpop.
 */
BOOST_AUTO_TEST_CASE(std_stackpush)
{
  cpu::File source;

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("move fp sp");
  source.insert("loada g0 heap");
  source.insert("loadi g1 0x400");
  source.insert("call std_init");
  source.insert("b main");

  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"stdlib/stack/stack.swl\"");
  source.insert(".include \"stdlib/stack/push.swl\"");
  source.insert(".include \"stdlib/stack/pop.swl\"");

  // Test
  source.insert(".label main");
  source.insert("call std_stack");
  source.insert("push g0");

  source.insert("loadi g1 0xF3F");
  source.insert("call std_stackpush");

  source.insert("loadri g0 fp 0x0");
  source.insert("loadi g1 0x3F3");
  source.insert("call std_stackpush");

  source.insert("loadri g0 fp 0x0");
  source.insert("call std_stackpop");
  source.insert("push g0");

  source.insert("loadri g0 fp 0x0");
  source.insert("call std_stackpop");

  source.insert("move g1 g0");
  source.insert("pop g0");

  source.insert("stop");

  // Space for 256 words in the heap
  source.insert(".label heap");
  source.insert(".block 0x400");

  // Space for 32 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x80");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x3F3);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0xF3F);
}

/**
 * Check std_stackiterator.
 */
BOOST_AUTO_TEST_CASE(std_stackiterator)
{
  cpu::File source;

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
  source.insert(".include \"stdlib/stack/stack.swl\"");
  source.insert(".include \"stdlib/stack/push.swl\"");
  source.insert(".include \"stdlib/stack/iterator.swl\"");

  // Test
  source.insert(".label main");
  source.insert("call std_stack");
  source.insert("move r0 g0");

  source.insert("loadi g1 0xF3F");
  source.insert("call std_stackpush");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x3F3");
  source.insert("call std_stackpush");

  source.insert("move g0 r0");
  source.insert("call std_stackiterator");
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

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x3F3);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0xF3F);
}
