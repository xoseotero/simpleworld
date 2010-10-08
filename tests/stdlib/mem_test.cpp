/**
 * @file tests/stdlib/mem_test.cpp
 * Unit test for stdlib/mem.swl
 *
 *  Copyright (C) 2009-2010  Xosé Otero <xoseotero@gmail.com>
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

#define BOOST_TEST_MODULE Unit test for stdlib/mem.swl
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
#define CPU_SAVE (TESTOUTPUT "mem.swo")
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

  source.insert(line++, ".include \"stdlib/mem.swl\"");
  BOOST_CHECK_NO_THROW(compile(source));
}

/**
 * Check std_fill.
 */
BOOST_AUTO_TEST_CASE(std_fill)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/mem.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "loadi g2 0x4567");
  source.insert(line++, "loadhi g2 0x0123");
  source.insert(line++, "call std_fill");

  source.insert(line++, "loada g0 data");
  source.insert(line++, "loadri g1 g0 0x0");
  source.insert(line++, "loadri g2 g0 0x4");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label data");
  source.insert(line++, ".block 0x10");
  source.insert(line++, ".label data_end");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0x01234567);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g2")], 0x01234567);
}

/**
 * Check std_fillh.
 */
BOOST_AUTO_TEST_CASE(std_fillh)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/mem.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "loadi g2 0x0123");
  source.insert(line++, "call std_fillh");

  source.insert(line++, "loada g0 data");
  source.insert(line++, "loadri g1 g0 0x0");
  source.insert(line++, "loadri g2 g0 0x4");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label data");
  source.insert(line++, ".block 0x10");
  source.insert(line++, ".label data_end");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0x01230123);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g2")], 0x01230123);
}

/**
 * Check std_fillq.
 */
BOOST_AUTO_TEST_CASE(std_fillq)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/mem.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "loadi g2 0x01");
  source.insert(line++, "call std_fillq");

  source.insert(line++, "loada g0 data");
  source.insert(line++, "loadri g1 g0 0x0");
  source.insert(line++, "loadri g2 g0 0x4");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label data");
  source.insert(line++, ".block 0x10");
  source.insert(line++, ".label data_end");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0x01010101);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g2")], 0x01010101);
}

/**
 * Check std_copy.
 */
BOOST_AUTO_TEST_CASE(std_copy)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/mem.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 dst");
  source.insert(line++, "loada g1 src");
  source.insert(line++, "loadi g2 0x8");
  source.insert(line++, "call std_copy");

  source.insert(line++, "loada g0 dst");
  source.insert(line++, "loadri g1 g0 0x0");
  source.insert(line++, "loadri g2 g0 0x4");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label src");
  source.insert(line++, "0x01234567");
  source.insert(line++, "0x89ABCDEF");

  source.insert(line++, ".label dst");
  source.insert(line++, ".block 0x8");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0x01234567);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g2")], 0x89ABCDEF);
}

/**
 * Check std_cmp.
 */
BOOST_AUTO_TEST_CASE(std_cmp)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/mem.swl\"");

  // Test with the same data
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data");
  source.insert(line++, "loadi g2 0x4");
  source.insert(line++, "call std_cmp");
  source.insert(line++, "push g0");

  // Test with first > second
  source.insert(line++, "loada g1 data");
  source.insert(line++, "addi g0 g1 0x4");
  source.insert(line++, "loadi g2 0x4");
  source.insert(line++, "call std_cmp");
  source.insert(line++, "push g0");

  // Test with first < second
  source.insert(line++, "loada g0 data");
  source.insert(line++, "addi g1 g0 0x4");
  source.insert(line++, "loadi g2 0x4");
  source.insert(line++, "call std_cmp");

  source.insert(line++, "move g2 g0");
  source.insert(line++, "pop g1");
  source.insert(line++, "pop g0");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label data");
  source.insert(line++, "0x01234567");
  source.insert(line++, "0x01234789");

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
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0x2);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g2")], 0xFFFFFFFE);
}

/**
 * Check std_find.
 */
BOOST_AUTO_TEST_CASE(std_find)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/mem.swl\"");

  // Test with a element that is not in the block
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "loadi g2 0x0F0F");
  source.insert(line++, "call std_find");
  source.insert(line++, "push g1");

  // Test with a element that is in the block
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "loadi g2 0x5050");
  source.insert(line++, "call std_find");

  source.insert(line++, "move g3 g1");
  source.insert(line++, "move g2 g0");
  source.insert(line++, "loadi r0 STD_NOERROR");
  source.insert(line++, "pop g0");
  source.insert(line++, "loadi g1 STD_EINVAL");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label data");
  source.insert(line++, "0x01234567");
  source.insert(line++, "0x89ABCDEF");
  source.insert(line++, "0x0F05050F");
  source.insert(line++, "0x00005050");
  source.insert(line++, ".label data_end");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")],
                    registers[REGISTER(cpu, "g1")]);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g2")], 12);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g3")],
                    registers[REGISTER(cpu, "r0")]);
}

/**
 * Check std_findh.
 */
BOOST_AUTO_TEST_CASE(std_findh)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/mem.swl\"");

  // Test with a element that is not in the block
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "loadi g2 0x0F0F");
  source.insert(line++, "call std_findh");
  source.insert(line++, "push g1");

  // Test with a element that is in the block
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "loadi g2 0x5050");
  source.insert(line++, "call std_findh");

  source.insert(line++, "move g3 g1");
  source.insert(line++, "move g2 g0");
  source.insert(line++, "loadi r0 STD_NOERROR");
  source.insert(line++, "pop g0");
  source.insert(line++, "loadi g1 STD_EINVAL");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label data");
  source.insert(line++, "0x01234567");
  source.insert(line++, "0x89ABCDEF");
  source.insert(line++, "0x0F05050F");
  source.insert(line++, "0x00005050");
  source.insert(line++, ".label data_end");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")],
                    registers[REGISTER(cpu, "g1")]);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g2")], 14);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g3")],
                    registers[REGISTER(cpu, "r0")]);
}

/**
 * Check std_findq.
 */
BOOST_AUTO_TEST_CASE(std_findq)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/mem.swl\"");

  // Test with a element that is not in the block
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "loadi g2 0xFF");
  source.insert(line++, "call std_findq");
  source.insert(line++, "push g1");

  // Test with a element that is in the block
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "loadi g2 0x50");
  source.insert(line++, "call std_findq");

  source.insert(line++, "move g3 g1");
  source.insert(line++, "move g2 g0");
  source.insert(line++, "loadi r0 STD_NOERROR");
  source.insert(line++, "pop g0");
  source.insert(line++, "loadi g1 STD_EINVAL");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label data");
  source.insert(line++, "0x01234567");
  source.insert(line++, "0x89ABCDEF");
  source.insert(line++, "0x0F05050F");
  source.insert(line++, "0x00005050");
  source.insert(line++, ".label data_end");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")],
                    registers[REGISTER(cpu, "g1")]);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g2")], 14);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g3")],
                    registers[REGISTER(cpu, "r0")]);
}

/**
 * Check std_count.
 */
BOOST_AUTO_TEST_CASE(std_count)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/mem.swl\"");

  // Test with a element that is not in the block
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "loadi g2 0x0F0F");
  source.insert(line++, "call std_count");
  source.insert(line++, "push g0");

  // Test with a element that is in the block
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "loadi g2 0x5050");
  source.insert(line++, "call std_count");

  source.insert(line++, "move g1 g0");
  source.insert(line++, "pop g0");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label data");
  source.insert(line++, "0x00005050");
  source.insert(line++, "0x01234567");
  source.insert(line++, "0x89ABCDEF");
  source.insert(line++, "0x0F05050F");
  source.insert(line++, "0x00005050");
  source.insert(line++, ".label data_end");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 2);
}

/**
 * Check std_counth.
 */
BOOST_AUTO_TEST_CASE(std_counth)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/mem.swl\"");

  // Test with a element that is not in the block
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "loadi g2 0x0F0F");
  source.insert(line++, "call std_counth");
  source.insert(line++, "push g0");

  // Test with a element that is in the block
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "loadi g2 0x5050");
  source.insert(line++, "call std_counth");

  source.insert(line++, "move g1 g0");
  source.insert(line++, "pop g0");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label data");
  source.insert(line++, "0x00005050");
  source.insert(line++, "0x01234567");
  source.insert(line++, "0x89ABCDEF");
  source.insert(line++, "0x0F05050F");
  source.insert(line++, "0x50505050");
  source.insert(line++, ".label data_end");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 3);
}

/**
 * Check std_countq.
 */
BOOST_AUTO_TEST_CASE(std_countq)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/mem.swl\"");

  // Test with a element that is not in the block
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "loadi g2 0xFF");
  source.insert(line++, "call std_countq");
  source.insert(line++, "push g0");

  // Test with a element that is in the block
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "loadi g2 0x50");
  source.insert(line++, "call std_countq");

  source.insert(line++, "move g1 g0");
  source.insert(line++, "pop g0");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label data");
  source.insert(line++, "0x00005050");
  source.insert(line++, "0x01234567");
  source.insert(line++, "0x89ABCDEF");
  source.insert(line++, "0x0F05050F");
  source.insert(line++, "0x50505050");
  source.insert(line++, ".label data_end");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 6);
}
