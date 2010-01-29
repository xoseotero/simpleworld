/**
 * @file tests/stdlib/world_test.cpp
 * Unit test for stdlib/world.swl
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

  BOOST_CHECK_NO_THROW(source.insert(line++, ".include \"stdlib/world.swl\""));
}

/**
 * Compile the file.
 */
BOOST_AUTO_TEST_CASE(swl_compile)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  source.insert(line++, ".include \"stdlib/world.swl\"");
  BOOST_CHECK_NO_THROW(compile(source));
}

/**
 * Check the definitions.
 */
BOOST_AUTO_TEST_CASE(swl_definitions)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  source.insert(line++, ".include \"stdlib/world.swl\"");
  source.insert(line++, "STD_SUCCESS");
  source.insert(line++, "STD_FAILURE");

  source.insert(line++, "STD_NOTHING");

  source.insert(line++, "STD_MYID");
  source.insert(line++, "STD_MYSIZE");
  source.insert(line++, "STD_MYENERGY");
  source.insert(line++, "STD_MYPOSITION");
  source.insert(line++, "STD_MYORIENTATION");
  source.insert(line++, "STD_DETECT");
  source.insert(line++, "STD_INFOID");
  source.insert(line++, "STD_INFOSIZE");
  source.insert(line++, "STD_INFOENERGY");
  source.insert(line++, "STD_INFOPOSITION");
  source.insert(line++, "STD_INFOORIENTATION");
  source.insert(line++, "STD_MOVEFORWARD");
  source.insert(line++, "STD_MOVEBACKWARD");
  source.insert(line++, "STD_TURNLEFT");
  source.insert(line++, "STD_TURNRIGHT");
  source.insert(line++, "STD_ATTACK");
  source.insert(line++, "STD_EAT");
  source.insert(line++, "STD_EGG");

  source.insert(line++, "STD_ONORTH");
  source.insert(line++, "STD_OEAST");
  source.insert(line++, "STD_OSOUTH");
  source.insert(line++, "STD_OWEST");

  source.insert(line++, "STD_LNOTHING");
  source.insert(line++, "STD_LEGG");
  source.insert(line++, "STD_LBUG");
  source.insert(line++, "STD_LFOOD");

  source.insert(line++, "STD_IACTION");
  source.insert(line++, "STD_IEVENT");
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
