/**
 * @file tests/stdlib/address_test.cpp
 * Unit test for stdlib/address.swl
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

#define BOOST_TEST_MODULE Unit test for stdlib/address.swl
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
#define CPU_SAVE (TESTOUTPUT "address.swo")
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

  source.insert(line++, ".include \"stdlib/address.swl\"");
  BOOST_CHECK_NO_THROW(compile(source));
}

/**
 * Check std_address.
 *
 * Check a valid address.
 */
BOOST_AUTO_TEST_CASE(std_address)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialization code
  source.insert(line++, ".label init");
  // Initialize the stack pointer
  source.insert(line++, "loada sp stack");

  // Initialize the stdlib
  source.insert(line++, ".define HEAP_SIZE 0x400");
  source.insert(line++, "loada r0 heap");
  source.insert(line++, "loadi r1 HEAP_SIZE");
  source.insert(line++, "loadi r2 0x1");
  source.insert(line++, "call std_init");

  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/def.swl\"");
  source.insert(line++, ".include \"stdlib/init.swl\"");
  source.insert(line++, ".include \"stdlib/address.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada r0 main");
  source.insert(line++, "loadi r1 STD_WORDSIZE");
  source.insert(line++, "call std_address");
  source.insert(line++, "stop");

  // Space for the heap
  source.insert(line++, ".label heap");
  source.insert(line++, ".block HEAP_SIZE");

  // Space for 32 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x80");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_NE(registers[REGISTER(cpu, "r0")], 0x0);
}

/**
 * Check std_address.
 *
 * Check a address to a word in the limit of the memory.
 */
BOOST_AUTO_TEST_CASE(std_address_limit_word)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialization code
  source.insert(line++, ".label init");
  // Initialize the stack pointer
  source.insert(line++, "loada sp stack");

  // Initialize the stdlib
  source.insert(line++, ".define HEAP_SIZE 0x400");
  source.insert(line++, "loada r0 heap");
  source.insert(line++, "loadi r1 HEAP_SIZE");
  source.insert(line++, "loadi r2 0x1");
  source.insert(line++, "call std_init");

  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/def.swl\"");
  source.insert(line++, ".include \"stdlib/init.swl\"");
  source.insert(line++, ".include \"stdlib/address.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada r0 end");
  source.insert(line++, "subi r0 r0 STD_WORDSIZE");
  source.insert(line++, "loadi r1 STD_WORDSIZE");
  source.insert(line++, "call std_address");
  source.insert(line++, "stop");

  // Space for the heap
  source.insert(line++, ".label heap");
  source.insert(line++, ".block HEAP_SIZE");

  // Space for 32 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x80");

  // End of the memory
  source.insert(line++, ".label end");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_NE(registers[REGISTER(cpu, "r0")], 0x0);
}

/**
 * Check std_address.
 *
 * Check a address to a half word in the limit of the memory.
 */
BOOST_AUTO_TEST_CASE(std_address_limit_halfword)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialization code
  source.insert(line++, ".label init");
  // Initialize the stack pointer
  source.insert(line++, "loada sp stack");

  // Initialize the stdlib
  source.insert(line++, ".define HEAP_SIZE 0x400");
  source.insert(line++, "loada r0 heap");
  source.insert(line++, "loadi r1 HEAP_SIZE");
  source.insert(line++, "loadi r2 0x1");
  source.insert(line++, "call std_init");

  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/def.swl\"");
  source.insert(line++, ".include \"stdlib/init.swl\"");
  source.insert(line++, ".include \"stdlib/address.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada r0 end");
  source.insert(line++, "subi r0 r0 STD_HWORDSIZE");
  source.insert(line++, "loadi r1 STD_HWORDSIZE");
  source.insert(line++, "call std_address");
  source.insert(line++, "stop");

  // Space for the heap
  source.insert(line++, ".label heap");
  source.insert(line++, ".block HEAP_SIZE");

  // Space for 32 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x80");

  // End of the memory
  source.insert(line++, ".label end");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_NE(registers[REGISTER(cpu, "r0")], 0x0);
}

/**
 * Check std_address.
 *
 * Check a address to a quarter word in the limit of the memory.
 */
BOOST_AUTO_TEST_CASE(std_address_limit_quarterword)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialization code
  source.insert(line++, ".label init");
  // Initialize the stack pointer
  source.insert(line++, "loada sp stack");

  // Initialize the stdlib
  source.insert(line++, ".define HEAP_SIZE 0x400");
  source.insert(line++, "loada r0 heap");
  source.insert(line++, "loadi r1 HEAP_SIZE");
  source.insert(line++, "loadi r2 0x1");
  source.insert(line++, "call std_init");

  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/def.swl\"");
  source.insert(line++, ".include \"stdlib/init.swl\"");
  source.insert(line++, ".include \"stdlib/address.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada r0 end");
  source.insert(line++, "subi r0 r0 STD_QWORDSIZE");
  source.insert(line++, "loadi r1 STD_QWORDSIZE");
  source.insert(line++, "call std_address");
  source.insert(line++, "stop");

  // Space for the heap
  source.insert(line++, ".label heap");
  source.insert(line++, ".block HEAP_SIZE");

  // Space for 32 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x80");

  // End of the memory
  source.insert(line++, ".label end");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_NE(registers[REGISTER(cpu, "r0")], 0x0);
}

/**
 * Check std_address.
 *
 * Check STD_NULL.
 */
BOOST_AUTO_TEST_CASE(std_address_null)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialization code
  source.insert(line++, ".label init");
  // Initialize the stack pointer
  source.insert(line++, "loada sp stack");

  // Initialize the stdlib
  source.insert(line++, ".define HEAP_SIZE 0x400");
  source.insert(line++, "loada r0 heap");
  source.insert(line++, "loadi r1 HEAP_SIZE");
  source.insert(line++, "loadi r2 0x1");
  source.insert(line++, "call std_init");

  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/def.swl\"");
  source.insert(line++, ".include \"stdlib/init.swl\"");
  source.insert(line++, ".include \"stdlib/address.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loadi r0 STD_NULL");
  source.insert(line++, "loadi r1 STD_WORDSIZE");
  source.insert(line++, "call std_address");
  source.insert(line++, "stop");

  // Space for the heap
  source.insert(line++, ".label heap");
  source.insert(line++, ".block HEAP_SIZE");

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
}

/**
 * Check std_address.
 *
 * Check a invalid address.
 */
BOOST_AUTO_TEST_CASE(std_address_inval)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialization code
  source.insert(line++, ".label init");
  // Initialize the stack pointer
  source.insert(line++, "loada sp stack");

  // Initialize the stdlib
  source.insert(line++, ".define HEAP_SIZE 0x400");
  source.insert(line++, "loada r0 heap");
  source.insert(line++, "loadi r1 HEAP_SIZE");
  source.insert(line++, "loadi r2 0x1");
  source.insert(line++, "call std_init");

  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/init.swl\"");
  source.insert(line++, ".include \"stdlib/address.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada r0 end");
  source.insert(line++, "loadi r1 STD_WORDSIZE");
  source.insert(line++, "call std_address");
  source.insert(line++, "stop");

  // Space for the heap
  source.insert(line++, ".label heap");
  source.insert(line++, ".block HEAP_SIZE");

  // Space for 32 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x80");

  // End of the memory
  source.insert(line++, ".label end");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0x0);
}
