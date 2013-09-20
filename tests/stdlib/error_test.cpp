/**
 * @file tests/stdlib/error_test.cpp
 * Unit test for stdlib/error.swl
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

#define BOOST_TEST_MODULE Unit test for stdlib/error.swl
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

#define CPU_SAVE (TESTOUTPUT "error.swo")


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

  source.insert(".include \"stdlib/error.swl\"");
  BOOST_CHECK_NO_THROW(compile(source));
}

/**
 * Check the definitions.
 */
BOOST_AUTO_TEST_CASE(swl_definitions)
{
  cpu::File source;

  source.insert(".include \"stdlib/error.swl\"");
  source.insert("STD_NOERROR");
  source.insert("STD_EINVAL");
  source.insert("STD_EFAULT");
  source.insert("STD_EOVERFLOW");
  source.insert("STD_ENOMEM");
  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);

  BOOST_CHECK_EQUAL(memory[ADDRESS(0)], 0x0000);
  BOOST_CHECK_EQUAL(memory[ADDRESS(1)], 0x0001);
  BOOST_CHECK_EQUAL(memory[ADDRESS(2)], 0x0002);
  BOOST_CHECK_EQUAL(memory[ADDRESS(3)], 0x0003);
  BOOST_CHECK_EQUAL(memory[ADDRESS(4)], 0x0004);
}
