/**
 * @file tests/stdlib/array_test.cpp
 * Unit test for stdlib/array.swl
 *
 *  Copyright (C) 2009-2010  Xosé Otero <xoseotero@users.sourceforge.net>
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

#define BOOST_TEST_MODULE Unit test for stdlib/array.swl
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
#define CPU_SAVE (TESTOUTPUT "array.swo")
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
 * Include the file.
 */
BOOST_AUTO_TEST_CASE(swl_include)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  BOOST_CHECK_NO_THROW(source.insert(line++, ".include \"stdlib/array.swl\""));
}

/**
 * Compile the file.
 */
BOOST_AUTO_TEST_CASE(swl_compile)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  source.insert(line++, ".include \"stdlib/array.swl\"");
  BOOST_CHECK_NO_THROW(compile(source));
}

/**
 * Check std_array.
 * The memory used for the array must be returned when freed.
 */
BOOST_AUTO_TEST_CASE(std_array)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "move fp sp");
  source.insert(line++, "loada r0 heap");
  source.insert(line++, "loadi r1 0x400");
  source.insert(line++, "loadi r2 0x1");
  source.insert(line++, "call std_init");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/init.swl\"");
  source.insert(line++, ".include \"stdlib/alloc.swl\"");
  source.insert(line++, ".include \"stdlib/array.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada r0 minfo");
  source.insert(line++, "call std_minfo");
  source.insert(line++, "loadri r0 r0 STD_MINFO_FREE");
  source.insert(line++, "push r0");

  source.insert(line++, "loadi r0 0x8");
  source.insert(line++, "call std_array");
  source.insert(line++, "push r0");

  source.insert(line++, "loada r0 minfo");
  source.insert(line++, "call std_minfo");
  source.insert(line++, "loadri r0 r0 STD_MINFO_FREE");
  source.insert(line++, "push r0");

  source.insert(line++, "loadri r0 fp 0x4");
  source.insert(line++, "call std_arrayfree");

  source.insert(line++, "loada r0 minfo");
  source.insert(line++, "call std_minfo");
  source.insert(line++, "loadri r2 r0 STD_MINFO_FREE");
  source.insert(line++, "loadri r0 fp 0x0");
  source.insert(line++, "loadri r1 fp 0x8");

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
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")],
                    registers[REGISTER(cpu, "r2")]);
  BOOST_CHECK_GT(registers[REGISTER(cpu, "r0")],
                 registers[REGISTER(cpu, "r1")]);
  BOOST_CHECK_LT(registers[REGISTER(cpu, "r1")],
                 registers[REGISTER(cpu, "r2")]);
}

/**
 * Check std_arraysize and std_arrayresize.
 */
BOOST_AUTO_TEST_CASE(std_arraysize)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "move fp sp");
  source.insert(line++, "loada r0 heap");
  source.insert(line++, "loadi r1 0x400");
  source.insert(line++, "loadi r2 0x1");
  source.insert(line++, "call std_init");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/init.swl\"");
  source.insert(line++, ".include \"stdlib/array.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loadi r0 0x8");
  source.insert(line++, "call std_array");
  source.insert(line++, "push r0");

  source.insert(line++, "call std_arraysize");
  source.insert(line++, "push r0");

  source.insert(line++, "loadri r0 fp 0x0");
  source.insert(line++, "loadi r1 0x4");
  source.insert(line++, "call std_arrayresize");

  source.insert(line++, "loadri r0 fp 0x0");
  source.insert(line++, "call std_arraysize");

  source.insert(line++, "move r1 r0");
  source.insert(line++, "pop r0");

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
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0x8);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r1")], 0x4);
}

/**
 * Check std_arrayget and std_arrayset.
 */
BOOST_AUTO_TEST_CASE(std_arrayget)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "move fp sp");
  source.insert(line++, "loada r0 heap");
  source.insert(line++, "loadi r1 0x400");
  source.insert(line++, "loadi r2 0x1");
  source.insert(line++, "call std_init");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/init.swl\"");
  source.insert(line++, ".include \"stdlib/array.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loadi r0 0x2");
  source.insert(line++, "call std_array");
  source.insert(line++, "push r0");

  source.insert(line++, "loadi r1 0x0");
  source.insert(line++, "loadi r2 0xF3F");
  source.insert(line++, "call std_arrayset");

  source.insert(line++, "loadri r0 fp 0x0");
  source.insert(line++, "loadi r1 0x1");
  source.insert(line++, "loadi r2 0x3F3");
  source.insert(line++, "call std_arrayset");

  source.insert(line++, "loadri r0 fp 0x0");
  source.insert(line++, "loadi r1 0x0");
  source.insert(line++, "call std_arrayget");
  source.insert(line++, "push r0");

  source.insert(line++, "loadri r0 fp 0x0");
  source.insert(line++, "loadi r1 0x1");
  source.insert(line++, "call std_arrayget");

  source.insert(line++, "move r1 r0");
  source.insert(line++, "pop r0");

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
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0xF3F);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r1")], 0x3F3);
}

/**
 * Check std_arrayfill.
 */
BOOST_AUTO_TEST_CASE(std_arrayfill)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "move fp sp");
  source.insert(line++, "loada r0 heap");
  source.insert(line++, "loadi r1 0x400");
  source.insert(line++, "loadi r2 0x1");
  source.insert(line++, "call std_init");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/init.swl\"");
  source.insert(line++, ".include \"stdlib/array.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loadi r0 0x2");
  source.insert(line++, "call std_array");
  source.insert(line++, "push r0");

  source.insert(line++, "loadi r1 0xF3F");
  source.insert(line++, "call std_arrayfill");

  source.insert(line++, "loadri r0 fp 0x0");
  source.insert(line++, "loadi r1 0x0");
  source.insert(line++, "call std_arrayget");
  source.insert(line++, "push r0");

  source.insert(line++, "loadri r0 fp 0x0");
  source.insert(line++, "loadi r1 0x1");
  source.insert(line++, "call std_arrayget");

  source.insert(line++, "move r1 r0");
  source.insert(line++, "pop r0");

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
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0xF3F);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r1")], 0xF3F);
}

/**
 * Check std_arrayfind.
 */
BOOST_AUTO_TEST_CASE(std_arrayfind)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "move fp sp");
  source.insert(line++, "loada r0 heap");
  source.insert(line++, "loadi r1 0x400");
  source.insert(line++, "loadi r2 0x1");
  source.insert(line++, "call std_init");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/init.swl\"");
  source.insert(line++, ".include \"stdlib/array.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loadi r0 0x2");
  source.insert(line++, "call std_array");
  source.insert(line++, "push r0");

  source.insert(line++, "loadi r1 0x0");
  source.insert(line++, "loadi r2 0x50");
  source.insert(line++, "call std_arrayset");

  source.insert(line++, "loadri r0 fp 0x0");
  source.insert(line++, "loadi r1 0x1");
  source.insert(line++, "loadi r2 0x40");
  source.insert(line++, "call std_arrayset");

  source.insert(line++, "loadri r0 fp 0x0");
  source.insert(line++, "loadi r1 0x0");
  source.insert(line++, "loadi r2 0x50");
  source.insert(line++, "call std_arrayfind");
  source.insert(line++, "push r0");

  source.insert(line++, "loadri r0 fp 0x0");
  source.insert(line++, "loadi r1 0x0");
  source.insert(line++, "loadi r2 0x40");
  source.insert(line++, "call std_arrayfind");

  source.insert(line++, "move r1 r0");
  source.insert(line++, "pop r0");

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
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r1")], 0x1);
}

/**
 * Check std_arraycount.
 */
BOOST_AUTO_TEST_CASE(std_arraycount)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "move fp sp");
  source.insert(line++, "loada r0 heap");
  source.insert(line++, "loadi r1 0x400");
  source.insert(line++, "loadi r2 0x1");
  source.insert(line++, "call std_init");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/init.swl\"");
  source.insert(line++, ".include \"stdlib/array.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loadi r0 0x2");
  source.insert(line++, "call std_array");
  source.insert(line++, "push r0");

  source.insert(line++, "loadi r1 0x0");
  source.insert(line++, "loadi r2 0x50");
  source.insert(line++, "call std_arrayset");

  source.insert(line++, "loadri r0 fp 0x0");
  source.insert(line++, "loadi r1 0x1");
  source.insert(line++, "loadi r2 0x40");
  source.insert(line++, "call std_arrayset");

  source.insert(line++, "loadri r0 fp 0x0");
  source.insert(line++, "loadi r1 0x40");
  source.insert(line++, "call std_arraycount");
  source.insert(line++, "push r0");

  source.insert(line++, "loadri r0 fp 0x0");
  source.insert(line++, "loadi r1 0x20");
  source.insert(line++, "call std_arraycount");

  source.insert(line++, "move r1 r0");
  source.insert(line++, "pop r0");

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
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0x1);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r1")], 0x0);
}
