/**
 * @file tests/stdlib/node_test.cpp
 * Unit test for stdlib/node.swl
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

#define BOOST_TEST_MODULE Unit test for stdlib/node.swl
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/cpu/memory.hpp>
#include <simpleworld/cpu/file.hpp>
#include <simpleworld/cpu/source.hpp>
namespace sw = simpleworld;
namespace cpu = simpleworld::cpu;

#include "src/common/fakecpu.hpp"


#define REGISTER(cpu, name) ADDRESS((cpu).isa().register_code(name))
#define MAX_CYCLES 4096


/**
 * Compile a file.
 * @param file file to compile
 * @return object code
 */
cpu::Memory compile(const cpu::File& file)
{
  cpu::Memory registers;
  FakeCPU cpu(&registers, NULL);

  cpu::Source source(cpu.isa(), file);
  source.add_include_path(INCLUDE_DIR);
  cpu::Memory mem;
  source.compile(&mem);

  return mem;
}


/**
 * Compile the file.
 */
BOOST_AUTO_TEST_CASE(swl_compile)
{
  cpu::File source;

  source.insert(".include \"stdlib/node.swl\"");

  source.insert("STD_NODE_STRUCT");
  source.insert("STD_NODE_DATA");
  source.insert("std_node");
  source.insert("std_nodebefore");
  source.insert("std_nodeafter");
  source.insert("std_vnode");
  source.insert("std_vnodebefore");
  source.insert("std_vnodeafter");
  source.insert("std_nodefree");
  source.insert("std_noderemove");
  source.insert("std_prev");
  source.insert("std_next");
  source.insert("std_first");
  source.insert("std_last");

  BOOST_CHECK_NO_THROW(compile(source));
}

/**
 * Check std_node.
 * The memory used for the node must be returned when freed.
 */
BOOST_AUTO_TEST_CASE(std_node)
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
  source.insert(".include \"stdlib/node/node.swl\"");
  source.insert(".include \"stdlib/node/free.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 minfo");
  source.insert("call std_minfo");
  source.insert("loadri g0 g0 STD_MINFO_FREE");
  source.insert("push g0");

  source.insert("call std_node");
  source.insert("push g0");

  source.insert("loada g0 minfo");
  source.insert("call std_minfo");
  source.insert("loadri g0 g0 STD_MINFO_FREE");
  source.insert("push g0");

  source.insert("loadri g0 fp 0x4");
  source.insert("call std_nodefree");

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
  cpu::Memory memory(compile(source));
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
 * Check std_nodebefore.
 */
BOOST_AUTO_TEST_CASE(std_nodebefore)
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
  source.insert(".include \"stdlib/node/node.swl\"");
  source.insert(".include \"stdlib/node/before.swl\"");
  source.insert(".include \"stdlib/node/free.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loadi g0 0x0");
  source.insert("call std_node");
  source.insert("move r0 g0");

  source.insert("loadi g1 0x1");
  source.insert("call std_nodebefore");
  source.insert("move r1 g0");

  source.insert("loadri g0 r1 _STD_NODE_PREV");
  source.insert("loadri g1 r1 _STD_NODE_NEXT");
  source.insert("loadri g2 r1 STD_NODE_DATA");

  source.insert("stop");

  // Space for 256 words in the heap
  source.insert(".label heap");
  source.insert(".block 0x400");

  // Space for 32 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x80");

  cpu::Memory registers;
  cpu::Memory memory(compile(source));
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")],
                    registers[REGISTER(cpu, "r0")]);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g2")], 0x1);
}

/**
 * Check std_nodeafter.
 */
BOOST_AUTO_TEST_CASE(std_nodeafter)
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
  source.insert(".include \"stdlib/node/node.swl\"");
  source.insert(".include \"stdlib/node/after.swl\"");
  source.insert(".include \"stdlib/node/free.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loadi g0 0x0");
  source.insert("call std_node");
  source.insert("move r0 g0");

  source.insert("loadi g1 0x1");
  source.insert("call std_nodeafter");
  source.insert("move r1 g0");

  source.insert("loadri g0 r1 _STD_NODE_PREV");
  source.insert("loadri g1 r1 _STD_NODE_NEXT");
  source.insert("loadri g2 r1 STD_NODE_DATA");

  source.insert("stop");

  // Space for 256 words in the heap
  source.insert(".label heap");
  source.insert(".block 0x400");

  // Space for 32 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x80");

  cpu::Memory registers;
  cpu::Memory memory(compile(source));
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")],
                    registers[REGISTER(cpu, "r0")]);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g2")], 0x1);
}

/**
 * Check std_vnode.
 * The memory used for the node must be returned when freed.
 */
BOOST_AUTO_TEST_CASE(std_vnode)
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

  source.insert(".include \"stdlib/def.swl\"");
  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"stdlib/alloc/def.swl\"");
  source.insert(".include \"stdlib/alloc/info.swl\"");
  source.insert(".include \"stdlib/node/vnode.swl\"");
  source.insert(".include \"stdlib/node/free.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 minfo");
  source.insert("call std_minfo");
  source.insert("loadri g0 g0 STD_MINFO_FREE");
  source.insert("push g0");

  source.insert("loada g0 main");
  source.insert("loadi g1 STD_WORDSIZE");
  source.insert("call std_vnode");
  source.insert("push g0");

  source.insert("loada g0 minfo");
  source.insert("call std_minfo");
  source.insert("loadri g0 g0 STD_MINFO_FREE");
  source.insert("push g0");

  source.insert("loadri g0 fp 0x4");
  source.insert("call std_nodefree");

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
  cpu::Memory memory(compile(source));
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
 * Check std_vnodebefore.
 */
BOOST_AUTO_TEST_CASE(std_vnodebefore)
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

  source.insert(".include \"stdlib/def.swl\"");
  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"stdlib/node/def.swl\"");
  source.insert(".include \"stdlib/node/vnode.swl\"");
  source.insert(".include \"stdlib/node/vbefore.swl\"");
  source.insert(".include \"stdlib/node/free.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 main");
  source.insert("loadi g1 STD_WORDSIZE");
  source.insert("call std_vnode");
  source.insert("move r0 g0");

  source.insert("loada g1 data");
  source.insert("loadi g2 STD_WORDSIZE");
  source.insert("call std_vnodebefore");
  source.insert("move r1 g0");

  source.insert("loadri g0 r1 _STD_NODE_PREV");
  source.insert("loadri g1 r1 _STD_NODE_NEXT");
  source.insert("loadri g2 r1 STD_NODE_DATA");

  source.insert("stop");

  source.insert(".label data");
  source.insert("0x1");

  // Space for 256 words in the heap
  source.insert(".label heap");
  source.insert(".block 0x400");

  // Space for 32 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x80");

  cpu::Memory registers;
  cpu::Memory memory(compile(source));
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")],
                    registers[REGISTER(cpu, "r0")]);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g2")], 0x1);
}

/**
 * Check std_vnodeafter.
 */
BOOST_AUTO_TEST_CASE(std_vnodeafter)
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

  source.insert(".include \"stdlib/def.swl\"");
  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"stdlib/node/def.swl\"");
  source.insert(".include \"stdlib/node/vnode.swl\"");
  source.insert(".include \"stdlib/node/vafter.swl\"");
  source.insert(".include \"stdlib/node/free.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 0x0");
  source.insert("loadi g1 STD_WORDSIZE");
  source.insert("call std_vnode");
  source.insert("move r0 g0");

  source.insert("loada g1 data");
  source.insert("loadi g2 STD_WORDSIZE");
  source.insert("call std_vnodeafter");
  source.insert("move r1 g0");

  source.insert("loadri g0 r1 _STD_NODE_PREV");
  source.insert("loadri g1 r1 _STD_NODE_NEXT");
  source.insert("loadri g2 r1 STD_NODE_DATA");

  source.insert("stop");

  source.insert(".label data");
  source.insert("0x1");

  // Space for 256 words in the heap
  source.insert(".label heap");
  source.insert(".block 0x400");

  // Space for 32 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x80");

  cpu::Memory registers;
  cpu::Memory memory(compile(source));
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")],
                    registers[REGISTER(cpu, "r0")]);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g2")], 0x1);
}

/**
 * Check std_noderemove.
 * The memory used for the node must be returned when freed.
 */
BOOST_AUTO_TEST_CASE(std_noderemove)
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
  source.insert(".include \"stdlib/node/def.swl\"");
  source.insert(".include \"stdlib/node/node.swl\"");
  source.insert(".include \"stdlib/node/before.swl\"");
  source.insert(".include \"stdlib/node/after.swl\"");
  source.insert(".include \"stdlib/node/free.swl\"");
  source.insert(".include \"stdlib/node/remove.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loadi g0 0x0");
  source.insert("call std_node");
  source.insert("move r0 g0");

  source.insert("loadi g1 0x1");
  source.insert("call std_nodebefore");
  source.insert("move r1 g0");

  source.insert("move g0 r0");
  source.insert("loadi g1 0x2");
  source.insert("call std_nodeafter");
  source.insert("move r2 g0");

  source.insert("loada g0 minfo");
  source.insert("call std_minfo");
  source.insert("loadri r3 g0 STD_MINFO_FREE");

  source.insert("move g0 r0");
  source.insert("call std_noderemove");

  source.insert("loada g0 minfo");
  source.insert("call std_minfo");
  source.insert("loadri r4 g0 STD_MINFO_FREE");

  source.insert("loadri g0 r1 _STD_NODE_NEXT");
  source.insert("loadri g1 r2 _STD_NODE_PREV");

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
  cpu::Memory memory(compile(source));
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_LT(registers[REGISTER(cpu, "r3")],
                 registers[REGISTER(cpu, "r4")]);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")],
                    registers[REGISTER(cpu, "r2")]);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")],
                    registers[REGISTER(cpu, "r1")]);
}

/**
 * Check std_prev.
 */
BOOST_AUTO_TEST_CASE(std_prev)
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
  source.insert(".include \"stdlib/node/node.swl\"");
  source.insert(".include \"stdlib/node/after.swl\"");
  source.insert(".include \"stdlib/node/prev.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loadi g0 0x0");
  source.insert("call std_node");
  source.insert("push g0");

  source.insert("loadi g1 0x0");
  source.insert("call std_nodeafter");

  source.insert("loadi g1 0x1");
  source.insert("call std_prev");

  source.insert("pop g1");

  source.insert("stop");

  // Space for 256 words in the heap
  source.insert(".label heap");
  source.insert(".block 0x400");

  // Space for 32 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x80");

  cpu::Memory registers;
  cpu::Memory memory(compile(source));
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

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("move fp sp");
  source.insert("loada g0 heap");
  source.insert("loadi g1 0x400");
  source.insert("call std_init");
  source.insert("b main");

  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"stdlib/node/node.swl\"");
  source.insert(".include \"stdlib/node/before.swl\"");
  source.insert(".include \"stdlib/node/next.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loadi g0 0x0");
  source.insert("call std_node");
  source.insert("push g0");

  source.insert("loadi g1 0x0");
  source.insert("call std_nodebefore");

  source.insert("loadi g1 0x1");
  source.insert("call std_next");

  source.insert("pop g1");

  source.insert("stop");

  // Space for 256 words in the heap
  source.insert(".label heap");
  source.insert(".block 0x400");

  // Space for 32 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x80");

  cpu::Memory registers;
  cpu::Memory memory(compile(source));
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

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("move fp sp");
  source.insert("loada g0 heap");
  source.insert("loadi g1 0x400");
  source.insert("call std_init");
  source.insert("b main");

  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"stdlib/node/node.swl\"");
  source.insert(".include \"stdlib/node/after.swl\"");
  source.insert(".include \"stdlib/node/first.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loadi g0 0x0");
  source.insert("call std_node");
  source.insert("push g0");

  source.insert("loadi g1 0x0");
  source.insert("call std_nodeafter");

  source.insert("loadi g1 0x0");
  source.insert("call std_nodeafter");

  source.insert("call std_first");

  source.insert("pop g1");

  source.insert("stop");

  // Space for 256 words in the heap
  source.insert(".label heap");
  source.insert(".block 0x400");

  // Space for 32 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x80");

  cpu::Memory registers;
  cpu::Memory memory(compile(source));
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

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("move fp sp");
  source.insert("loada g0 heap");
  source.insert("loadi g1 0x400");
  source.insert("call std_init");
  source.insert("b main");

  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"stdlib/node/node.swl\"");
  source.insert(".include \"stdlib/node/before.swl\"");
  source.insert(".include \"stdlib/node/last.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loadi g0 0x0");
  source.insert("call std_node");
  source.insert("push g0");

  source.insert("loadi g1 0x0");
  source.insert("call std_nodebefore");

  source.insert("loadi g1 0x0");
  source.insert("call std_nodebefore");

  source.insert("loadi g1 0x1");
  source.insert("call std_last");

  source.insert("pop g1");

  source.insert("stop");

  // Space for 256 words in the heap
  source.insert(".label heap");
  source.insert(".block 0x400");

  // Space for 32 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x80");

  cpu::Memory registers;
  cpu::Memory memory(compile(source));
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")],
                    registers[REGISTER(cpu, "g1")]);
}
