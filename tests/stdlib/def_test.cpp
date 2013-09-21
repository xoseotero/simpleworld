/**
 * @file tests/stdlib/def_test.cpp
 * Unit test for stdlib/def.swl
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

#define BOOST_TEST_MODULE Unit test for stdlib/def.swl
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/cpu/memory.hpp>
#include <simpleworld/cpu/file.hpp>
#include <simpleworld/cpu/source.hpp>
namespace sw = simpleworld;
namespace cpu = simpleworld::cpu;

#include "src/common/fakecpu.hpp"


#define REGISTER(cpu, name) ADDRESS((cpu).isa().register_code(name))


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

  source.insert(".include \"stdlib/def.swl\"");
  BOOST_CHECK_NO_THROW(compile(source));
}

/**
 * Check the definitions.
 */
BOOST_AUTO_TEST_CASE(swl_definitions)
{
  cpu::File source;

  source.insert(".include \"stdlib/def.swl\"");
  source.insert("STD_FALSE");
  source.insert("STD_TRUE");

  source.insert("STD_NULL");

  source.insert("STD_WORDSIZE");
  source.insert("STD_HWORDSIZE");
  source.insert("STD_QWORDSIZE");

  source.insert("STD_WORDZERO");
  source.insert("STD_WORDFULL");

  source.insert("STD_WORDMIN");
  source.insert("STD_WORDMAX");
  source.insert("STD_WORDMINU");
  source.insert("STD_WORDMAXU");

  cpu::Memory registers;
  cpu::Memory memory(compile(source));
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
