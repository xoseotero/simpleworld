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
#include <simpleworld/cpu/source.hpp>
#include <simpleworld/cpu/cpu.hpp>
namespace sw = simpleworld;
namespace cpu = simpleworld::cpu;

#include "src/common/fakeisa.hpp"


#define REGISTER(cpu, name) ADDRESS((cpu).isa().register_code(name))


/**
 * Compile the file.
 */
BOOST_AUTO_TEST_CASE(swl_compile)
{
  cpu::Source source(fakeisa);
  source.add_include_path(INCLUDE_DIR);

  source.insert(".include \"stdlib/error.swl\"");

  cpu::Memory memory;
  BOOST_CHECK_NO_THROW(source.compile(&memory));
}

/**
 * Check the definitions.
 */
BOOST_AUTO_TEST_CASE(swl_definitions)
{
  cpu::Source source(fakeisa);
  source.add_include_path(INCLUDE_DIR);

  source.insert(".include \"stdlib/error.swl\"");
  source.insert("STD_NOERROR");
  source.insert("STD_EINVAL");
  source.insert("STD_EFAULT");
  source.insert("STD_EOVERFLOW");
  source.insert("STD_ENOMEM");

  cpu::Memory registers;
  cpu::Memory memory;
  source.compile(&memory);
  cpu::CPU cpu(fakeisa, &registers, &memory);

  BOOST_CHECK_EQUAL(memory[ADDRESS(0)], 0x0000);
  BOOST_CHECK_EQUAL(memory[ADDRESS(1)], 0x0001);
  BOOST_CHECK_EQUAL(memory[ADDRESS(2)], 0x0002);
  BOOST_CHECK_EQUAL(memory[ADDRESS(3)], 0x0003);
  BOOST_CHECK_EQUAL(memory[ADDRESS(4)], 0x0004);
}
