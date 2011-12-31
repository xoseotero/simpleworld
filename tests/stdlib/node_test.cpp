/**
 * @file tests/stdlib/node_test.cpp
 * Unit test for stdlib/node.swl
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

#define BOOST_TEST_MODULE Unit test for stdlib/node.swl
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
#define CPU_SAVE (TESTOUTPUT "node.swo")
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

  source.insert(line++, ".include \"stdlib/node.swl\"");

  source.insert(line++, "STD_NODE_STRUCT");
  source.insert(line++, "STD_NODE_DATA");
  source.insert(line++, "std_node");
  source.insert(line++, "std_nodefree");
  source.insert(line++, "std_prev");
  source.insert(line++, "std_next");
  source.insert(line++, "std_first");
  source.insert(line++, "std_last");

  BOOST_CHECK_NO_THROW(compile(source));
}

/**
 * Check std_node.
 * The memory used for the node must be returned when freed.
 */
BOOST_AUTO_TEST_CASE(std_node)
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
  source.insert(line++, ".include \"stdlib/node/node.swl\"");
  source.insert(line++, ".include \"stdlib/node/free.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 minfo");
  source.insert(line++, "call std_minfo");
  source.insert(line++, "loadri g0 g0 STD_MINFO_FREE");
  source.insert(line++, "push g0");

  source.insert(line++, "call std_node");
  source.insert(line++, "push g0");

  source.insert(line++, "loada g0 minfo");
  source.insert(line++, "call std_minfo");
  source.insert(line++, "loadri g0 g0 STD_MINFO_FREE");
  source.insert(line++, "push g0");

  source.insert(line++, "loadri g0 fp 0x4");
  source.insert(line++, "call std_nodefree");

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
 * Check std_prev.
 */
BOOST_AUTO_TEST_CASE(std_prev)
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

  source.insert(line++, ".include \"stdlib/def.swl\"");
  source.insert(line++, ".include \"stdlib/init.swl\"");
  source.insert(line++, ".include \"stdlib/node/node.swl\"");
  source.insert(line++, ".include \"stdlib/node/prev.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loadi g0 STD_NULL");
  source.insert(line++, "loadi g1 STD_NULL");
  source.insert(line++, "loadi g2 0x0");
  source.insert(line++, "call std_node");
  source.insert(line++, "push g0");

  source.insert(line++, "loadi g1 STD_NULL");
  source.insert(line++, "loadi g2 0x0");
  source.insert(line++, "call std_node");

  source.insert(line++, "loadi g1 0x1");
  source.insert(line++, "call std_prev");

  source.insert(line++, "pop g1");

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
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")],
                    registers[REGISTER(cpu, "g1")]);
}

/**
 * Check std_next.
 */
BOOST_AUTO_TEST_CASE(std_next)
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

  source.insert(line++, ".include \"stdlib/def.swl\"");
  source.insert(line++, ".include \"stdlib/init.swl\"");
  source.insert(line++, ".include \"stdlib/node/node.swl\"");
  source.insert(line++, ".include \"stdlib/node/next.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loadi g0 STD_NULL");
  source.insert(line++, "loadi g1 STD_NULL");
  source.insert(line++, "loadi g2 0x0");
  source.insert(line++, "call std_node");
  source.insert(line++, "push g0");

  source.insert(line++, "move g1 g0");
  source.insert(line++, "loadi g0 STD_NULL");
  source.insert(line++, "loadi g2 0x0");
  source.insert(line++, "call std_node");

  source.insert(line++, "loadi g1 0x1");
  source.insert(line++, "call std_next");

  source.insert(line++, "pop g1");

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
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")],
                    registers[REGISTER(cpu, "g1")]);
}

/**
 * Check std_first.
 */
BOOST_AUTO_TEST_CASE(std_first)
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

  source.insert(line++, ".include \"stdlib/def.swl\"");
  source.insert(line++, ".include \"stdlib/init.swl\"");
  source.insert(line++, ".include \"stdlib/node/node.swl\"");
  source.insert(line++, ".include \"stdlib/node/first.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loadi g0 STD_NULL");
  source.insert(line++, "loadi g1 STD_NULL");
  source.insert(line++, "loadi g2 0x0");
  source.insert(line++, "call std_node");
  source.insert(line++, "push g0");

  source.insert(line++, "loadi g1 STD_NULL");
  source.insert(line++, "loadi g2 0x0");
  source.insert(line++, "call std_node");

  source.insert(line++, "loadi g1 STD_NULL");
  source.insert(line++, "loadi g2 0x0");
  source.insert(line++, "call std_node");

  source.insert(line++, "call std_first");

  source.insert(line++, "pop g1");

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
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")],
                    registers[REGISTER(cpu, "g1")]);
}

/**
 * Check std_last.
 */
BOOST_AUTO_TEST_CASE(std_last)
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

  source.insert(line++, ".include \"stdlib/def.swl\"");
  source.insert(line++, ".include \"stdlib/init.swl\"");
  source.insert(line++, ".include \"stdlib/node/node.swl\"");
  source.insert(line++, ".include \"stdlib/node/last.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loadi g0 STD_NULL");
  source.insert(line++, "loadi g1 STD_NULL");
  source.insert(line++, "loadi g2 0x0");
  source.insert(line++, "call std_node");
  source.insert(line++, "push g0");

  source.insert(line++, "move g1 g0");
  source.insert(line++, "loadi g0 STD_NULL");
  source.insert(line++, "loadi g2 0x0");
  source.insert(line++, "call std_node");

  source.insert(line++, "move g1 g0");
  source.insert(line++, "loadi g0 STD_NULL");
  source.insert(line++, "loadi g2 0x0");
  source.insert(line++, "call std_node");

  source.insert(line++, "loadi g1 0x1");
  source.insert(line++, "call std_last");

  source.insert(line++, "pop g1");

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
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")],
                    registers[REGISTER(cpu, "g1")]);
}
