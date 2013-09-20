/**
 * @file tests/stdlib/multimap_test.cpp
 * Unit test for stdlib/multimap.swl
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

#define BOOST_TEST_MODULE Unit test for stdlib/multimap.swl
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
#define CPU_SAVE (TESTOUTPUT "multimap.swo")
#define MAX_CYCLES 6144


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

  source.insert(".include \"stdlib/multimap.swl\"");

  source.insert("STD_MULTIMAP_STRUCT");
  source.insert("std_multimap");
  source.insert("std_multimapfree");
  source.insert("std_multimapinsert");
  source.insert("std_multimapget");
  source.insert("std_multimapremove");
  source.insert("std_multimapiterator");
  source.insert("std_multimapcheck");

  BOOST_CHECK_NO_THROW(compile(source));
}

/**
 * Check std_multimap.
 * The memory used for the multimap must be returned when freed.
 */
BOOST_AUTO_TEST_CASE(std_multimap)
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
  source.insert(".include \"stdlib/multimap/multimap.swl\"");
  source.insert(".include \"stdlib/multimap/free.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 minfo");
  source.insert("call std_minfo");
  source.insert("loadri g0 g0 STD_MINFO_FREE");
  source.insert("push g0");

  source.insert("call std_multimap");
  source.insert("push g0");

  source.insert("loada g0 minfo");
  source.insert("call std_minfo");
  source.insert("loadri g0 g0 STD_MINFO_FREE");
  source.insert("push g0");

  source.insert("loadri g0 fp 0x4");
  source.insert("call std_multimapfree");

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
 * Check std_multimapinsert.
 */
BOOST_AUTO_TEST_CASE(std_multimapinsert)
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
  source.insert(".include \"stdlib/list/size.swl\"");
  source.insert(".include \"stdlib/multimap/multimap.swl\"");
  source.insert(".include \"stdlib/multimap/insert.swl\"");
  source.insert(".include \"stdlib/multimap/get.swl\"");
  

  // Test
  source.insert(".label main");
  source.insert("call std_multimap");
  source.insert("move r0 g0");

  source.insert("loadi g1 0x0");
  source.insert("loadi g2 0xF3F");
  source.insert("call std_multimapinsert");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x1");
  source.insert("loadi g2 0x3F3");
  source.insert("call std_multimapinsert");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x1");
  source.insert("loadi g2 0x4F4");
  source.insert("call std_multimapinsert");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x0");
  source.insert("call std_multimapget");
  source.insert("call std_listsize");
  source.insert("push g0");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x1");
  source.insert("call std_multimapget");
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

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x1);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0x2);
}

/**
 * Check std_multimapremove.
 */
BOOST_AUTO_TEST_CASE(std_multimapremove)
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
  source.insert(".include \"stdlib/multimap/multimap.swl\"");
  source.insert(".include \"stdlib/multimap/insert.swl\"");
  source.insert(".include \"stdlib/multimap/get.swl\"");
  source.insert(".include \"stdlib/multimap/remove.swl\"");

  // Test
  source.insert(".label main");
  source.insert("call std_multimap");
  source.insert("move r0 g0");

  source.insert("loadi g1 0x0");
  source.insert("loadi g2 0xF3F");
  source.insert("call std_multimapinsert");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x1");
  source.insert("loadi g2 0x3F3");
  source.insert("call std_multimapinsert");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x1");
  source.insert("loadi g2 0x4F4");
  source.insert("call std_multimapinsert");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x0");
  source.insert("loadi g2 0xF3F");
  source.insert("call std_multimapremove");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x0");
  source.insert("call std_multimapget");
  source.insert("push g1");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x1");
  source.insert("loadi g2 0x3F3");
  source.insert("call std_multimapremove");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x1");
  source.insert("call std_multimapget");

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
 * Check std_multimapcheck.
 */
BOOST_AUTO_TEST_CASE(std_multimapcheck)
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
  source.insert(".include \"stdlib/multimap/multimap.swl\"");
  source.insert(".include \"stdlib/multimap/insert.swl\"");
  source.insert(".include \"stdlib/multimap/check.swl\"");

  // Test
  source.insert(".label main");
  source.insert("call std_multimap");
  source.insert("move r0 g0");

  source.insert("loadi g1 0x0");
  source.insert("loadi g2 0xF3F");
  source.insert("call std_multimapinsert");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x1");
  source.insert("loadi g2 0x3F3");
  source.insert("call std_multimapinsert");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x0");
  source.insert("call std_multimapcheck");
  source.insert("push g0");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x1");
  source.insert("call std_multimapcheck");
  source.insert("push g0");

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
  BOOST_CHECK_NE(registers[REGISTER(cpu, "g1")], 0x0);
}
