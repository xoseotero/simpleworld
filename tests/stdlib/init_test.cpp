/**
 * @file tests/stdlib/init_test.cpp
 * Unit test for stdlib/init.swl
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

#define BOOST_TEST_MODULE Unit test for stdlib/init.swl
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
#define CPU_SAVE (TESTOUTPUT "init.swo")
#define MAX_CYCLES 4096


/**
 * Compile a file to CPU_SAVE.
 * @param file file to compile
 */
void compile(const cpu::File& file)
{
  cpu::Memory registers;
  FakeCPU cpu(&registers, NULL);

  cpu::Source source(cpu.isa());
  source.add_include_path(INCLUDE_DIR);

  source.insert(0, file);
  source.compile(CPU_SAVE);
}


/**
 * Compile the file.
 */
BOOST_AUTO_TEST_CASE(swl_compile)
{
  cpu::File source;

  source.insert(".include \"stdlib/init.swl\"");
  BOOST_CHECK_NO_THROW(compile(source));
}

/**
 * Call two times std_init.
 */
BOOST_AUTO_TEST_CASE(std_init)
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
  source.insert(".include \"stdlib/alloc/alloc.swl\"");

  // Test
  source.insert(".label main");
  // Allocate memory
  source.insert("loadi g0 0x4");
  source.insert("call std_alloc");
  source.insert("move r0 g0");

  // Second call to std_init
  source.insert("loada g0 heap");
  source.insert("loadi g1 0x400");
  source.insert("call std_init");

  // Allocate memory
  source.insert("loadi g0 0x4");
  source.insert("call std_alloc");
  source.insert("move r0 g0");

  source.insert("move g1 r0");

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
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")],
                    registers[REGISTER(cpu, "g1")]);
}
