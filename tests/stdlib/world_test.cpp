/**
 * @file tests/stdlib/world_test.cpp
 * Unit test for stdlib/world.swl
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

#define BOOST_TEST_MODULE Unit test for stdlib/world.swl
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

#define CPU_SAVE (TESTOUTPUT "world.swo")


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

  source.insert(".include \"stdlib/world.swl\"");
  BOOST_CHECK_NO_THROW(compile(source));
}

/**
 * Check the definitions.
 */
BOOST_AUTO_TEST_CASE(swl_definitions)
{
  cpu::File source;

  source.insert(".include \"stdlib/world.swl\"");
  source.insert("STD_SUCCESS");
  source.insert("STD_FAILURE");

  source.insert("STD_NOTHING");

  source.insert("STD_MYID");
  source.insert("STD_MYSIZE");
  source.insert("STD_MYENERGY");
  source.insert("STD_MYPOSITION");
  source.insert("STD_MYORIENTATION");
  source.insert("STD_DETECT");
  source.insert("STD_INFOID");
  source.insert("STD_INFOSIZE");
  source.insert("STD_INFOENERGY");
  source.insert("STD_INFOPOSITION");
  source.insert("STD_INFOORIENTATION");
  source.insert("STD_MOVEFORWARD");
  source.insert("STD_MOVEBACKWARD");
  source.insert("STD_TURNLEFT");
  source.insert("STD_TURNRIGHT");
  source.insert("STD_ATTACK");
  source.insert("STD_EAT");
  source.insert("STD_EGG");

  source.insert("STD_ONORTH");
  source.insert("STD_OEAST");
  source.insert("STD_OSOUTH");
  source.insert("STD_OWEST");

  source.insert("STD_LNOTHING");
  source.insert("STD_LEGG");
  source.insert("STD_LBUG");
  source.insert("STD_LFOOD");

  source.insert("STD_IACTION");
  source.insert("STD_IEVENT");
  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);

  BOOST_CHECK_EQUAL(memory[ADDRESS(0)], 0x0000);
  BOOST_CHECK_EQUAL(memory[ADDRESS(1)], 0x0001);
  BOOST_CHECK_EQUAL(memory[ADDRESS(2)], 0x0000);
  BOOST_CHECK_EQUAL(memory[ADDRESS(3)], 0x0010);
  BOOST_CHECK_EQUAL(memory[ADDRESS(4)], 0x0011);
  BOOST_CHECK_EQUAL(memory[ADDRESS(5)], 0x0012);
  BOOST_CHECK_EQUAL(memory[ADDRESS(6)], 0x0013);
  BOOST_CHECK_EQUAL(memory[ADDRESS(7)], 0x0014);
  BOOST_CHECK_EQUAL(memory[ADDRESS(8)], 0x0020);
  BOOST_CHECK_EQUAL(memory[ADDRESS(9)], 0x0030);
  BOOST_CHECK_EQUAL(memory[ADDRESS(10)], 0x0031);
  BOOST_CHECK_EQUAL(memory[ADDRESS(11)], 0x0032);
  BOOST_CHECK_EQUAL(memory[ADDRESS(12)], 0x0033);
  BOOST_CHECK_EQUAL(memory[ADDRESS(13)], 0x0034);
  BOOST_CHECK_EQUAL(memory[ADDRESS(14)], 0x0040);
  BOOST_CHECK_EQUAL(memory[ADDRESS(15)], 0x0041);
  BOOST_CHECK_EQUAL(memory[ADDRESS(16)], 0x0042);
  BOOST_CHECK_EQUAL(memory[ADDRESS(17)], 0x0043);
  BOOST_CHECK_EQUAL(memory[ADDRESS(18)], 0x0050);
  BOOST_CHECK_EQUAL(memory[ADDRESS(19)], 0x0060);
  BOOST_CHECK_EQUAL(memory[ADDRESS(20)], 0x0070);
  BOOST_CHECK_EQUAL(memory[ADDRESS(21)], 0x0000);
  BOOST_CHECK_EQUAL(memory[ADDRESS(22)], 0x0001);
  BOOST_CHECK_EQUAL(memory[ADDRESS(23)], 0x0002);
  BOOST_CHECK_EQUAL(memory[ADDRESS(24)], 0x0003);
  BOOST_CHECK_EQUAL(memory[ADDRESS(25)], 0x0000);
  BOOST_CHECK_EQUAL(memory[ADDRESS(26)], 0x0001);
  BOOST_CHECK_EQUAL(memory[ADDRESS(27)], 0x0002);
  BOOST_CHECK_EQUAL(memory[ADDRESS(28)], 0x0003);
  BOOST_CHECK_EQUAL(memory[ADDRESS(29)], 0x0005);
  BOOST_CHECK_EQUAL(memory[ADDRESS(30)], 0x0006);
}
