/**
 * @file tests/stdlib/map_test.cpp
 * Unit test for stdlib/map.swl
 *
 *  Copyright (C) 2012  Xosé Otero <xoseotero@gmail.com>
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

#define BOOST_TEST_MODULE Unit test for stdlib/map.swl
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
#define CPU_SAVE (TESTOUTPUT "map.swo")
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

  source.insert(".include \"stdlib/map.swl\"");

  source.insert("STD_MAP_STRUCT");
  source.insert("std_map");
  source.insert("std_mapfree");
  source.insert("std_mapset");
  source.insert("std_mapget");
  source.insert("std_mapremove");
  source.insert("std_mapiterator");
  source.insert("std_mapcheck");

  BOOST_CHECK_NO_THROW(compile(source));
}

/**
 * Check std_map.
 * The memory used for the map must be returned when freed.
 */
BOOST_AUTO_TEST_CASE(std_map)
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
  source.insert(".include \"stdlib/map/map.swl\"");
  source.insert(".include \"stdlib/map/free.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 minfo");
  source.insert("call std_minfo");
  source.insert("loadri g0 g0 STD_MINFO_FREE");
  source.insert("push g0");

  source.insert("call std_map");
  source.insert("push g0");

  source.insert("loada g0 minfo");
  source.insert("call std_minfo");
  source.insert("loadri g0 g0 STD_MINFO_FREE");
  source.insert("push g0");

  source.insert("loadri g0 fp 0x4");
  source.insert("call std_mapfree");

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
 * Check std_mapset.
 */
BOOST_AUTO_TEST_CASE(std_mapset)
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
  source.insert(".include \"stdlib/map/map.swl\"");
  source.insert(".include \"stdlib/map/set.swl\"");
  source.insert(".include \"stdlib/map/get.swl\"");
  

  // Test
  source.insert(".label main");
  source.insert("call std_map");
  source.insert("move r0 g0");

  source.insert("loadi g1 0x0");
  source.insert("loadi g2 0xF3F");
  source.insert("call std_mapset");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x1");
  source.insert("loadi g2 0x3F3");
  source.insert("call std_mapset");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x1");
  source.insert("loadi g2 0x4F4");
  source.insert("call std_mapset");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x0");
  source.insert("call std_mapget");
  source.insert("push g0");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x1");
  source.insert("call std_mapget");

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
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0xF3F);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0x4F4);
}

/**
 * Check std_mapremove.
 */
BOOST_AUTO_TEST_CASE(std_mapremove)
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
  source.insert(".include \"stdlib/map/map.swl\"");
  source.insert(".include \"stdlib/map/set.swl\"");
  source.insert(".include \"stdlib/map/get.swl\"");
  source.insert(".include \"stdlib/map/remove.swl\"");

  // Test
  source.insert(".label main");
  source.insert("call std_map");
  source.insert("move r0 g0");

  source.insert("loadi g1 0x0");
  source.insert("loadi g2 0xF3F");
  source.insert("call std_mapset");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x1");
  source.insert("loadi g2 0x3F3");
  source.insert("call std_mapset");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x0");
  source.insert("call std_mapremove");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x1");
  source.insert("call std_mapget");
  source.insert("push g0");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x1");
  source.insert("call std_mapremove");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x1");
  source.insert("call std_mapget");

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
  BOOST_CHECK_NE(registers[REGISTER(cpu, "g1")], 0x0);
}

/**
 * Check std_mapiterator.
 */
BOOST_AUTO_TEST_CASE(std_mapiterator)
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
  source.insert(".include \"stdlib/map/def.swl\"");
  source.insert(".include \"stdlib/map/map.swl\"");
  source.insert(".include \"stdlib/map/set.swl\"");
  source.insert(".include \"stdlib/map/iterator.swl\"");

  // Test
  source.insert(".label main");
  source.insert("call std_map");
  source.insert("move r0 g0");

  source.insert("loadi g1 0x1");
  source.insert("loadi g2 0xF3F");
  source.insert("call std_mapset");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x0");
  source.insert("loadi g2 0x3F3");
  source.insert("call std_mapset");

  source.insert("move g0 r0");
  source.insert("call std_mapiterator");
  source.insert("move r1 g0");

  source.insert("loadi g1 STD_NODE_DATA");
  source.insert("addi r2 g1 _STD_MAPNODE_KEY");
  source.insert("addi r3 g1 _STD_MAPNODE_VALUE");

  source.insert("loadrr g1 r1 r2");
  source.insert("push g1");
  source.insert("loadrr g1 r1 r3");
  source.insert("push g1");

  source.insert("loadi g1 0x1");
  source.insert("call std_next");

  source.insert("loadrr g2 g0 r2");
  source.insert("loadrr g3 g0 r3");

  source.insert("pop g1");
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
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0x3F3);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g2")], 0x1);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g3")], 0xF3F);
}

/**
 * Check std_mapcheck.
 */
BOOST_AUTO_TEST_CASE(std_mapcheck)
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
  source.insert(".include \"stdlib/map/map.swl\"");
  source.insert(".include \"stdlib/map/set.swl\"");
  source.insert(".include \"stdlib/map/check.swl\"");

  // Test
  source.insert(".label main");
  source.insert("call std_map");
  source.insert("move r0 g0");

  source.insert("loadi g1 0x0");
  source.insert("loadi g2 0xF3F");
  source.insert("call std_mapset");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x1");
  source.insert("loadi g2 0x3F3");
  source.insert("call std_mapset");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x0");
  source.insert("call std_mapcheck");
  source.insert("push g0");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x1");
  source.insert("call std_mapcheck");
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
