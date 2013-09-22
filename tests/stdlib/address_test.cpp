/**
 * @file tests/stdlib/address_test.cpp
 * Unit test for stdlib/address.swl
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

#define BOOST_TEST_MODULE Unit test for stdlib/address.swl
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

  source.insert(".include \"stdlib/address.swl\"");

  source.insert("std_address");

  cpu::Memory memory;
  BOOST_CHECK_NO_THROW(source.compile(&memory));
}

/**
 * Check std_address.
 *
 * Check a valid address.
 */
BOOST_AUTO_TEST_CASE(std_address)
{
  cpu::Source source(fakeisa);
  source.add_include_path(INCLUDE_DIR);

  // Initialization code
  source.insert(".label init");
  // Initialize the stack pointer
  source.insert("loada sp stack");

  // Initialize the stdlib
  source.insert(".define HEAP_SIZE 0x400");
  source.insert("loada g0 heap");
  source.insert("loadi g1 HEAP_SIZE");
  source.insert("call std_init");

  source.insert("b main");

  source.insert(".include \"stdlib/def.swl\"");
  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"stdlib/address/address.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 main");
  source.insert("loadi g1 STD_WORDSIZE");
  source.insert("call std_address");
  source.insert("stop");

  // Space for the heap
  source.insert(".label heap");
  source.insert(".block HEAP_SIZE");

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
}

/**
 * Check std_address.
 *
 * Check a address to a word in the limit of the memory.
 */
BOOST_AUTO_TEST_CASE(std_address_limit_word)
{
  cpu::Source source(fakeisa);
  source.add_include_path(INCLUDE_DIR);

  // Initialization code
  source.insert(".label init");
  // Initialize the stack pointer
  source.insert("loada sp stack");

  // Initialize the stdlib
  source.insert(".define HEAP_SIZE 0x400");
  source.insert("loada g0 heap");
  source.insert("loadi g1 HEAP_SIZE");
  source.insert("call std_init");

  source.insert("b main");

  source.insert(".include \"stdlib/def.swl\"");
  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"stdlib/address/address.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 end");
  source.insert("subi g0 g0 STD_WORDSIZE");
  source.insert("loadi g1 STD_WORDSIZE");
  source.insert("call std_address");
  source.insert("stop");

  // Space for the heap
  source.insert(".label heap");
  source.insert(".block HEAP_SIZE");

  // Space for 32 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x80");

  // End of the memory
  source.insert(".label end");

  cpu::Memory registers;
  cpu::Memory memory;
  source.compile(&memory);
  cpu::CPU cpu(fakeisa, &registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_NE(registers[REGISTER(cpu, "g0")], 0x0);
}

/**
 * Check std_address.
 *
 * Check a address to a half word in the limit of the memory.
 */
BOOST_AUTO_TEST_CASE(std_address_limit_halfword)
{
  cpu::Source source(fakeisa);
  source.add_include_path(INCLUDE_DIR);

  // Initialization code
  source.insert(".label init");
  // Initialize the stack pointer
  source.insert("loada sp stack");

  // Initialize the stdlib
  source.insert(".define HEAP_SIZE 0x400");
  source.insert("loada g0 heap");
  source.insert("loadi g1 HEAP_SIZE");
  source.insert("call std_init");

  source.insert("b main");

  source.insert(".include \"stdlib/def.swl\"");
  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"stdlib/address/address.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 end");
  source.insert("subi g0 g0 STD_HWORDSIZE");
  source.insert("loadi g1 STD_HWORDSIZE");
  source.insert("call std_address");
  source.insert("stop");

  // Space for the heap
  source.insert(".label heap");
  source.insert(".block HEAP_SIZE");

  // Space for 32 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x80");

  // End of the memory
  source.insert(".label end");

  cpu::Memory registers;
  cpu::Memory memory;
  source.compile(&memory);
  cpu::CPU cpu(fakeisa, &registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_NE(registers[REGISTER(cpu, "g0")], 0x0);
}

/**
 * Check std_address.
 *
 * Check a address to a quarter word in the limit of the memory.
 */
BOOST_AUTO_TEST_CASE(std_address_limit_quarterword)
{
  cpu::Source source(fakeisa);
  source.add_include_path(INCLUDE_DIR);

  // Initialization code
  source.insert(".label init");
  // Initialize the stack pointer
  source.insert("loada sp stack");

  // Initialize the stdlib
  source.insert(".define HEAP_SIZE 0x400");
  source.insert("loada g0 heap");
  source.insert("loadi g1 HEAP_SIZE");
  source.insert("call std_init");

  source.insert("b main");

  source.insert(".include \"stdlib/def.swl\"");
  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"stdlib/address/address.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 end");
  source.insert("subi g0 g0 STD_QWORDSIZE");
  source.insert("loadi g1 STD_QWORDSIZE");
  source.insert("call std_address");
  source.insert("stop");

  // Space for the heap
  source.insert(".label heap");
  source.insert(".block HEAP_SIZE");

  // Space for 32 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x80");

  // End of the memory
  source.insert(".label end");

  cpu::Memory registers;
  cpu::Memory memory;
  source.compile(&memory);
  cpu::CPU cpu(fakeisa, &registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_NE(registers[REGISTER(cpu, "g0")], 0x0);
}

/**
 * Check std_address.
 *
 * Check STD_NULL.
 */
BOOST_AUTO_TEST_CASE(std_address_null)
{
  cpu::Source source(fakeisa);
  source.add_include_path(INCLUDE_DIR);

  // Initialization code
  source.insert(".label init");
  // Initialize the stack pointer
  source.insert("loada sp stack");

  // Initialize the stdlib
  source.insert(".define HEAP_SIZE 0x400");
  source.insert("loada g0 heap");
  source.insert("loadi g1 HEAP_SIZE");
  source.insert("call std_init");

  source.insert("b main");

  source.insert(".include \"stdlib/def.swl\"");
  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"stdlib/address/address.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loadi g0 STD_NULL");
  source.insert("loadi g1 STD_WORDSIZE");
  source.insert("call std_address");
  source.insert("stop");

  // Space for the heap
  source.insert(".label heap");
  source.insert(".block HEAP_SIZE");

  // Space for 32 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x80");

  cpu::Memory registers;
  cpu::Memory memory;
  source.compile(&memory);
  cpu::CPU cpu(fakeisa, &registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x0);
}

/**
 * Check std_address.
 *
 * Check a invalid address.
 */
BOOST_AUTO_TEST_CASE(std_address_inval)
{
  cpu::Source source(fakeisa);
  source.add_include_path(INCLUDE_DIR);

  // Initialization code
  source.insert(".label init");
  // Initialize the stack pointer
  source.insert("loada sp stack");

  // Initialize the stdlib
  source.insert(".define HEAP_SIZE 0x400");
  source.insert("loada g0 heap");
  source.insert("loadi g1 HEAP_SIZE");
  source.insert("call std_init");

  source.insert("b main");

  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"stdlib/address/address.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 end");
  source.insert("loadi g1 STD_WORDSIZE");
  source.insert("call std_address");
  source.insert("stop");

  // Space for the heap
  source.insert(".label heap");
  source.insert(".block HEAP_SIZE");

  // Space for 32 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x80");

  // End of the memory
  source.insert(".label end");

  cpu::Memory registers;
  cpu::Memory memory;
  source.compile(&memory);
  cpu::CPU cpu(fakeisa, &registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x0);
}
