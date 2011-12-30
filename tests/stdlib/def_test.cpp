/**
 * @file tests/stdlib/def_test.cpp
 * Unit test for stdlib/def.swl
 *
 *  Copyright (C) 2009-2011  Xosé Otero <xoseotero@gmail.com>
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

#define BOOST_TEST_MODULE Unit test for stdlib/def.swl
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

#define CPU_SAVE (TESTOUTPUT "def.swo")


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

  source.insert(line++, ".include \"stdlib/def.swl\"");
  BOOST_CHECK_NO_THROW(compile(source));
}

/**
 * Check the definitions.
 */
BOOST_AUTO_TEST_CASE(swl_definitions)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  source.insert(line++, ".include \"stdlib/def.swl\"");
  source.insert(line++, "STD_FALSE");
  source.insert(line++, "STD_TRUE");

  source.insert(line++, "STD_NULL");

  source.insert(line++, "STD_WORDSIZE");
  source.insert(line++, "STD_HWORDSIZE");
  source.insert(line++, "STD_QWORDSIZE");

  source.insert(line++, "STD_WORDZERO");
  source.insert(line++, "STD_WORDFULL");

  source.insert(line++, "STD_WORDMIN");
  source.insert(line++, "STD_WORDMAX");
  source.insert(line++, "STD_WORDMINU");
  source.insert(line++, "STD_WORDMAXU");
  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);

  BOOST_CHECK_EQUAL(memory[ADDRESS(0)], 0x0000);
  BOOST_CHECK_EQUAL(memory[ADDRESS(1)], 0x0001);
  BOOST_CHECK_EQUAL(memory[ADDRESS(2)], 0x0000);
  BOOST_CHECK_EQUAL(memory[ADDRESS(3)], 0x4);
  BOOST_CHECK_EQUAL(memory[ADDRESS(4)], 0x2);
  BOOST_CHECK_EQUAL(memory[ADDRESS(5)], 0x1);
  BOOST_CHECK_EQUAL(memory[ADDRESS(6)], 0x00000000);
  BOOST_CHECK_EQUAL(memory[ADDRESS(7)], 0xFFFFFFFF);
  BOOST_CHECK_EQUAL(memory[ADDRESS(8)], 0x80000000);
  BOOST_CHECK_EQUAL(memory[ADDRESS(9)], 0x7FFFFFFF);
  BOOST_CHECK_EQUAL(memory[ADDRESS(10)], 0x00000000);
  BOOST_CHECK_EQUAL(memory[ADDRESS(11)], 0xFFFFFFFF);
}
