/**
 * @file tests/stdlib/random_test.cpp
 * Unit test for stdlib/random.swl
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

#define BOOST_TEST_MODULE Unit test for stdlib/random.swl
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
#define CPU_SAVE (TESTOUTPUT "random.swo")
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

  BOOST_CHECK_NO_THROW(source.insert(line++,
				     ".include \"stdlib/random.swl\""));
}

/**
 * Compile the file.
 */
BOOST_AUTO_TEST_CASE(swl_compile)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  source.insert(line++, ".include \"stdlib/random.swl\"");
  BOOST_CHECK_NO_THROW(compile(source));
}

/**
 * Check std_random.
 *
 * Two calls to std_random must return different values.
 */
BOOST_AUTO_TEST_CASE(std_random)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/random.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "call std_random");
  source.insert(line++, "push r0");
  source.insert(line++, "call std_random");
  source.insert(line++, "pop r1");
  source.insert(line++, "stop");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_NE(registers[REGISTER(cpu, "r0")],
                 registers[REGISTER(cpu, "r1")]);
}

/**
 * Check std_seed.
 *
 * The same value must be returned by std_random with the same seed.
 */
BOOST_AUTO_TEST_CASE(std_seed)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/random.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loadi r0 0x0");
  source.insert(line++, "call std_seed");
  source.insert(line++, "call std_random");
  source.insert(line++, "push r0");

  source.insert(line++, "loadi r0 0x0");
  source.insert(line++, "call std_seed");
  source.insert(line++, "call std_random");
  source.insert(line++, "pop r1");

  source.insert(line++, "stop");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")],
                    registers[REGISTER(cpu, "r1")]);
}