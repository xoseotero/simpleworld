/**
 * @file tests/stdlib/list_test.cpp
 * Unit test for stdlib/list.swl
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

#define BOOST_TEST_MODULE Unit test for stdlib/list.swl
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

  source.insert(".include \"stdlib/list.swl\"");

  source.insert("STD_LIST_STRUCT");
  source.insert("std_list");
  source.insert("std_listfree");
  source.insert("std_listsize");
  source.insert("std_listget");
  source.insert("std_listinsert");
  source.insert("std_listremove");
  source.insert("std_listiterator");
  source.insert("std_listfind");
  source.insert("std_listcount");

  cpu::Memory memory;
  BOOST_CHECK_NO_THROW(source.compile(&memory));
}

/**
 * Check std_list.
 * The memory used for the list must be returned when freed.
 */
BOOST_AUTO_TEST_CASE(std_list)
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
  source.insert(".include \"stdlib/list/list.swl\"");
  source.insert(".include \"stdlib/list/free.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 minfo");
  source.insert("call std_minfo");
  source.insert("loadri g0 g0 STD_MINFO_FREE");
  source.insert("push g0");

  source.insert("call std_list");
  source.insert("push g0");

  source.insert("loada g0 minfo");
  source.insert("call std_minfo");
  source.insert("loadri g0 g0 STD_MINFO_FREE");
  source.insert("push g0");

  source.insert("loadri g0 fp 0x4");
  source.insert("call std_listfree");

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
 * Check std_listsize.
 */
BOOST_AUTO_TEST_CASE(std_listsize)
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
  source.insert(".include \"stdlib/list/list.swl\"");
  source.insert(".include \"stdlib/list/size.swl\"");


  // Test
  source.insert(".label main");
  source.insert("call std_list");

  source.insert("call std_listsize");

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
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x0);
}

/**
 * Check std_listinsert.
 */
BOOST_AUTO_TEST_CASE(std_listinsert)
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
  source.insert(".include \"stdlib/list/list.swl\"");
  source.insert(".include \"stdlib/list/size.swl\"");
  source.insert(".include \"stdlib/list/get.swl\"");
  source.insert(".include \"stdlib/list/insert.swl\"");
  

  // Test
  source.insert(".label main");
  source.insert("call std_list");
  source.insert("push g0");

  source.insert("loadi g1 0x0");
  source.insert("loadi g2 0xF3F");
  source.insert("call std_listinsert");

  source.insert("loadri g0 fp 0x0");
  source.insert("loadi g1 0x1");
  source.insert("loadi g2 0x3F3");
  source.insert("call std_listinsert");

  source.insert("loadri g0 fp 0x0");
  source.insert("loadi g1 0x0");
  source.insert("call std_listget");
  source.insert("push g0");

  source.insert("loadri g0 fp 0x0");
  source.insert("loadi g1 0x1");
  source.insert("call std_listget");
  source.insert("push g0");

  source.insert("loadri g0 fp 0x0");
  source.insert("call std_listsize");

  source.insert("move g2 g0");
  source.insert("pop g1");
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
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g2")], 0x2);
}

/**
 * Check std_listremove.
 */
BOOST_AUTO_TEST_CASE(std_listremove)
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
  source.insert(".include \"stdlib/list/list.swl\"");
  source.insert(".include \"stdlib/list/size.swl\"");
  source.insert(".include \"stdlib/list/get.swl\"");
  source.insert(".include \"stdlib/list/insert.swl\"");
  source.insert(".include \"stdlib/list/remove.swl\"");

  // Test
  source.insert(".label main");
  source.insert("call std_list");
  source.insert("push g0");

  source.insert("loadi g1 0x0");
  source.insert("loadi g2 0xF3F");
  source.insert("call std_listinsert");

  source.insert("loadri g0 fp 0x0");
  source.insert("loadi g1 0x1");
  source.insert("loadi g2 0x3F3");
  source.insert("call std_listinsert");

  source.insert("loadri g0 fp 0x0");
  source.insert("loadi g1 0x0");
  source.insert("call std_listremove");

  source.insert("loadri g0 fp 0x0");
  source.insert("loadi g1 0x0");
  source.insert("call std_listget");
  source.insert("push g0");

  source.insert("loadri g0 fp 0x0");
  source.insert("call std_listsize");

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
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x3F3);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0x1);
}

/**
 * Check std_listiterator.
 */
BOOST_AUTO_TEST_CASE(std_listiterator)
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
  source.insert(".include \"stdlib/list/list.swl\"");
  source.insert(".include \"stdlib/list/insert.swl\"");
  source.insert(".include \"stdlib/list/iterator.swl\"");
  

  // Test
  source.insert(".label main");
  source.insert("call std_list");
  source.insert("move r0 g0");

  source.insert("loadi g1 0x0");
  source.insert("loadi g2 0xF3F");
  source.insert("call std_listinsert");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x1");
  source.insert("loadi g2 0x3F3");
  source.insert("call std_listinsert");

  source.insert("move g0 r0");
  source.insert("call std_listiterator");
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

/**
 * Check std_listfind.
 */
BOOST_AUTO_TEST_CASE(std_listfind)
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
  source.insert(".include \"stdlib/list/list.swl\"");
  source.insert(".include \"stdlib/list/insert.swl\"");
  source.insert(".include \"stdlib/list/find.swl\"");

  // Test
  source.insert(".label main");
  source.insert("call std_list");
  source.insert("push g0");

  source.insert("loadi g1 0x0");
  source.insert("loadi g2 0x50");
  source.insert("call std_listinsert");

  source.insert("loadri g0 fp 0x0");
  source.insert("loadi g1 0x1");
  source.insert("loadi g2 0x40");
  source.insert("call std_listinsert");

  source.insert("loadri g0 fp 0x0");
  source.insert("loadi g1 0x0");
  source.insert("loadi g2 0x50");
  source.insert("call std_listfind");
  source.insert("push g0");

  source.insert("loadri g0 fp 0x0");
  source.insert("loadi g1 0x0");
  source.insert("loadi g2 0x40");
  source.insert("call std_listfind");

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
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0x1);
}

/**
 * Check std_listcount.
 */
BOOST_AUTO_TEST_CASE(std_listcount)
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
  source.insert(".include \"stdlib/list/list.swl\"");
  source.insert(".include \"stdlib/list/insert.swl\"");
  source.insert(".include \"stdlib/list/count.swl\"");

  // Test
  source.insert(".label main");
  source.insert("call std_list");
  source.insert("push g0");

  source.insert("loadi g1 0x0");
  source.insert("loadi g2 0x50");
  source.insert("call std_listinsert");

  source.insert("loadri g0 fp 0x0");
  source.insert("loadi g1 0x1");
  source.insert("loadi g2 0x40");
  source.insert("call std_listinsert");

  source.insert("loadri g0 fp 0x0");
  source.insert("loadi g1 0x40");
  source.insert("call std_listcount");
  source.insert("push g0");

  source.insert("loadri g0 fp 0x0");
  source.insert("loadi g1 0x20");
  source.insert("call std_listcount");

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
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x1);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0x0);
}
