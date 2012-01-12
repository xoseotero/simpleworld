/**
 * @file tests/stdlib/jmp_test.cpp
 * Unit test for stdlib/jmp.swl
 *
 *  Copyright (C) 2009-2012  Xosé Otero <xoseotero@gmail.com>
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

#define BOOST_TEST_MODULE Unit test for stdlib/jmp.swl
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
#define CPU_SAVE (TESTOUTPUT "jmp.swo")
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

  source.insert(".include \"stdlib/jmp.swl\"");

  source.insert("STD_JMP_STRUCT");
  source.insert("std_jmpset");
  source.insert("std_jmp");

  BOOST_CHECK_NO_THROW(compile(source));
}

/**
 * Check std_jmpset and std_jmp.
 */
BOOST_AUTO_TEST_CASE(std_jmp)
{
  cpu::File source;

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("b main");

  source.insert(".include \"stdlib/jmp/set.swl\"");
  source.insert(".include \"stdlib/jmp/jmp.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 jmp");
  source.insert("call std_jmpset");

  source.insert("bnz g0 _main_jmp");
  source.insert("loada g0 jmp");
  source.insert("loadi g1 0x0123");
  source.insert("call std_jmp");
  source.insert("b _main_exit");

  source.insert(".label _main_jmp");
  source.insert("store g0 data");

  source.insert(".label _main_exit");
  source.insert("load g0 data");
  source.insert("stop");

  // Space for jmp struct
  source.insert(".label jmp");
  source.insert(".block STD_JMP_STRUCT");

  // Data
  source.insert(".label data");
  source.insert(".block 0x4");

  // Space for 16 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x0123);
}
