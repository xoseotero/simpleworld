/**
 * @file tests/stdlib/set_test.cpp
 * Unit test for stdlib/set.swl
 *
 *  Copyright (C) 2009-2011  Xosé Otero <xoseotero@gmail.com>
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

#define BOOST_TEST_MODULE Unit test for stdlib/set.swl
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
#define CPU_SAVE (TESTOUTPUT "set.swo")
#define MAX_CYCLES 4096


/**
 * Compile a file to CPU_SAVE.
 * @param file file to compile
 */
void compile(const cpu::File& file)
{
  cpu::Memory registers;
  FakeCPU cpu(&registers, NULL);

  std::vector<std::string> include_path;
  include_path.push_back(INCLUDE_DIR);
  cpu::Source source(cpu.isa(), include_path);

  source.insert(0, file);
  source.compile(CPU_SAVE);
}


/**
 * Compile the file.
 */
BOOST_AUTO_TEST_CASE(swl_compile)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  source.insert(line++, ".include \"stdlib/set.swl\"");

  source.insert(line++, "STD_SET_STRUCT");
  source.insert(line++, "std_set");
  source.insert(line++, "std_setfree");
  source.insert(line++, "std_setsize");
  source.insert(line++, "std_setget");
  source.insert(line++, "std_setinsert");
  source.insert(line++, "std_setremove");
  source.insert(line++, "std_setcheck");

  BOOST_CHECK_NO_THROW(compile(source));
}

/**
 * Check std_set.
 * The memory used for the set must be returned when freed.
 */
BOOST_AUTO_TEST_CASE(std_set)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "move fp sp");
  source.insert(line++, "loada g0 heap");
  source.insert(line++, "loadi g1 0x400");
  source.insert(line++, "call std_init");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/init.swl\"");
  source.insert(line++, ".include \"stdlib/alloc/def.swl\"");
  source.insert(line++, ".include \"stdlib/alloc/info.swl\"");
  source.insert(line++, ".include \"stdlib/set/set.swl\"");
  source.insert(line++, ".include \"stdlib/set/free.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 minfo");
  source.insert(line++, "call std_minfo");
  source.insert(line++, "loadri g0 g0 STD_MINFO_FREE");
  source.insert(line++, "push g0");

  source.insert(line++, "call std_set");
  source.insert(line++, "push g0");

  source.insert(line++, "loada g0 minfo");
  source.insert(line++, "call std_minfo");
  source.insert(line++, "loadri g0 g0 STD_MINFO_FREE");
  source.insert(line++, "push g0");

  source.insert(line++, "loadri g0 fp 0x4");
  source.insert(line++, "call std_setfree");

  source.insert(line++, "loada g0 minfo");
  source.insert(line++, "call std_minfo");
  source.insert(line++, "loadri g2 g0 STD_MINFO_FREE");
  source.insert(line++, "loadri g0 fp 0x0");
  source.insert(line++, "loadri g1 fp 0x8");

  source.insert(line++, "stop");

  // Space for the minfo struct
  source.insert(line++, ".label minfo");
  source.insert(line++, ".block STD_MINFO_STRUCT");

  // Space for 256 words in the heap
  source.insert(line++, ".label heap");
  source.insert(line++, ".block 0x400");

  // Space for 32 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x80");

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
 * Check std_setsize.
 */
BOOST_AUTO_TEST_CASE(std_setsize)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "move fp sp");
  source.insert(line++, "loada g0 heap");
  source.insert(line++, "loadi g1 0x400");
  source.insert(line++, "call std_init");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/init.swl\"");
  source.insert(line++, ".include \"stdlib/set/set.swl\"");
  source.insert(line++, ".include \"stdlib/set/size.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "call std_set");

  source.insert(line++, "call std_setsize");

  source.insert(line++, "stop");

  // Space for 256 words in the heap
  source.insert(line++, ".label heap");
  source.insert(line++, ".block 0x400");

  // Space for 32 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x80");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x0);
}

/**
 * Check std_setinsert.
 */
BOOST_AUTO_TEST_CASE(std_setinsert)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "move fp sp");
  source.insert(line++, "loada g0 heap");
  source.insert(line++, "loadi g1 0x400");
  source.insert(line++, "call std_init");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/init.swl\"");
  source.insert(line++, ".include \"stdlib/set/set.swl\"");
  source.insert(line++, ".include \"stdlib/set/size.swl\"");
  source.insert(line++, ".include \"stdlib/set/insert.swl\"");
  source.insert(line++, ".include \"stdlib/set/check.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "call std_set");
  source.insert(line++, "push g0");

  source.insert(line++, "loadi g1 0xF3F");
  source.insert(line++, "call std_setinsert");

  source.insert(line++, "loadri g0 fp 0x0");
  source.insert(line++, "loadi g1 0x3F3");
  source.insert(line++, "call std_setinsert");

  source.insert(line++, "loadri g0 fp 0x0");
  source.insert(line++, "loadi g1 0xF3F");
  source.insert(line++, "call std_setinsert");

  source.insert(line++, "loadri g0 fp 0x0");
  source.insert(line++, "loadi g1 0x3F3");
  source.insert(line++, "call std_setcheck");
  source.insert(line++, "push g0");

  source.insert(line++, "loadri g0 fp 0x0");
  source.insert(line++, "loadi g1 0xF3F");
  source.insert(line++, "call std_setcheck");
  source.insert(line++, "push g0");

  source.insert(line++, "loadri g0 fp 0x0");
  source.insert(line++, "call std_setsize");

  source.insert(line++, "move g2 g0");
  source.insert(line++, "pop g1");
  source.insert(line++, "pop g0");

  source.insert(line++, "stop");

  // Space for 256 words in the heap
  source.insert(line++, ".label heap");
  source.insert(line++, ".block 0x400");

  // Space for 32 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x80");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_NE(registers[REGISTER(cpu, "g0")], 0x0);
  BOOST_CHECK_NE(registers[REGISTER(cpu, "g1")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g2")], 0x2);
}

/**
 * Check std_setremove.
 */
BOOST_AUTO_TEST_CASE(std_setremove)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "move fp sp");
  source.insert(line++, "loada g0 heap");
  source.insert(line++, "loadi g1 0x400");
  source.insert(line++, "call std_init");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/init.swl\"");
  source.insert(line++, ".include \"stdlib/set/set.swl\"");
  source.insert(line++, ".include \"stdlib/set/size.swl\"");
  source.insert(line++, ".include \"stdlib/set/insert.swl\"");
  source.insert(line++, ".include \"stdlib/set/remove.swl\"");
  source.insert(line++, ".include \"stdlib/set/check.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "call std_set");
  source.insert(line++, "push g0");

  source.insert(line++, "loadi g1 0xF3F");
  source.insert(line++, "call std_setinsert");

  source.insert(line++, "loadri g0 fp 0x0");
  source.insert(line++, "loadi g1 0x3F3");
  source.insert(line++, "call std_setinsert");

  source.insert(line++, "loadri g0 fp 0x0");
  source.insert(line++, "loadi g1 0xF3F");
  source.insert(line++, "call std_setremove");

  source.insert(line++, "loadri g0 fp 0x0");
  source.insert(line++, "loadi g1 0xF3F");
  source.insert(line++, "call std_setcheck");
  source.insert(line++, "push g0");

  source.insert(line++, "loadri g0 fp 0x0");
  source.insert(line++, "call std_setsize");

  source.insert(line++, "move g1 g0");
  source.insert(line++, "pop g0");

  source.insert(line++, "stop");

  // Space for 256 words in the heap
  source.insert(line++, ".label heap");
  source.insert(line++, ".block 0x400");

  // Space for 32 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x80");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0x1);
}

/**
 * Check std_setcheck.
 */
BOOST_AUTO_TEST_CASE(std_setcheck)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "move fp sp");
  source.insert(line++, "loada g0 heap");
  source.insert(line++, "loadi g1 0x400");
  source.insert(line++, "call std_init");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/init.swl\"");
  source.insert(line++, ".include \"stdlib/set/set.swl\"");
  source.insert(line++, ".include \"stdlib/set/insert.swl\"");
  source.insert(line++, ".include \"stdlib/set/check.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "call std_set");
  source.insert(line++, "push g0");

  source.insert(line++, "loadi g1 0x50");
  source.insert(line++, "call std_setinsert");

  source.insert(line++, "loadri g0 fp 0x0");
  source.insert(line++, "loadi g1 0x40");
  source.insert(line++, "call std_setinsert");

  source.insert(line++, "loadri g0 fp 0x0");
  source.insert(line++, "loadi g1 0x50");
  source.insert(line++, "call std_setcheck");
  source.insert(line++, "push g0");

  source.insert(line++, "loadri g0 fp 0x0");
  source.insert(line++, "loadi g1 0x20");
  source.insert(line++, "call std_setcheck");

  source.insert(line++, "move g1 g0");
  source.insert(line++, "pop g0");

  source.insert(line++, "stop");

  // Space for 256 words in the heap
  source.insert(line++, ".label heap");
  source.insert(line++, ".block 0x400");

  // Space for 32 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x80");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_NE(registers[REGISTER(cpu, "g0")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0x0);
}
