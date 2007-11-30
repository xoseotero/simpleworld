/**
 * @file tests/cpu/source_test.cpp
 * Unit test for CPU::Source.
 *
 * begin:     Fri, 30 Nov 2007 18:22:46 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2007  Xosé Otero <xoseotero@users.sourceforge.net>
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

#include <vector>
#include <string>

#define BOOST_TEST_MODULE Unit test for CPU::Source
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/cpu/memory.hpp>
#include <simpleworld/cpu/cpu.hpp>
#include <simpleworld/cpu/file.hpp>
#include <simpleworld/cpu/source.hpp>
namespace sw = SimpleWorld;
namespace cpu = SimpleWorld::CPU;


#define SOURCE (TESTDATA "source.swl")
#define SOURCE_INCLUDES (TESTDATA "source_includes.swl")
#define SOURCE_CONSTANTS (TESTDATA "source_constants.swl")
#define SOURCE_BLOCKS (TESTDATA "source_blocks.swl")
#define SOURCE_PREPROCESS (TESTDATA "source_preprocess.swl")
#define INCLUDE_DIR (TESTDATA "include")


/**
 * Check if two files are identical.
 * @param file1 the first file.
 * @param file2 the second file.
 * @return true if they are equal, false if not.
 */
bool compare_swl(const cpu::File& file1, const cpu::File& file2)
{
  if (file1.lines() != file2.lines())
    return false;

  cpu::File::size_type i;
  for (i = 0; i < file1.lines(); i++)
    if (file1[i] != file2[i])
      return false;

  return true;
}


/**
 * Replace the includes from the SWL.
 */
BOOST_AUTO_TEST_CASE(source_replace_includes)
{
  std::vector<std::string> include_path;
  include_path.push_back(INCLUDE_DIR);

  cpu::Memory registers;
  cpu::CPU cpu(&registers, NULL);
  cpu::Source compiler(cpu.isa(), include_path, SOURCE);

  compiler.replace_includes();

  BOOST_CHECK(compare_swl(compiler, cpu::File(SOURCE_INCLUDES)));
}

/**
 * Replace the constants from the SWL.
 */
BOOST_AUTO_TEST_CASE(source_replace_constants)
{
  std::vector<std::string> include_path;

  cpu::Memory registers;
  cpu::CPU cpu(&registers, NULL);
  cpu::Source compiler(cpu.isa(), include_path, SOURCE);

  compiler.replace_constants();

  BOOST_CHECK(compare_swl(compiler, cpu::File(SOURCE_CONSTANTS)));
}

/**
 * Replace the blocks from the SWL.
 */
BOOST_AUTO_TEST_CASE(source_replace_blocks)
{
  std::vector<std::string> include_path;

  cpu::Memory registers;
  cpu::CPU cpu(&registers, NULL);
  cpu::Source compiler(cpu.isa(), include_path, SOURCE);

  compiler.replace_blocks();

  BOOST_CHECK(compare_swl(compiler, cpu::File(SOURCE_BLOCKS)));
}

/**
 * Preprocess the SWL.
 */
BOOST_AUTO_TEST_CASE(source_preprocess)
{
  std::vector<std::string> include_path;
  include_path.push_back(INCLUDE_DIR);

  cpu::Memory registers;
  cpu::CPU cpu(&registers, NULL);
  cpu::Source compiler(cpu.isa(), include_path, SOURCE);

  compiler.replace_includes();
  compiler.replace_constants();
  compiler.replace_blocks();

  BOOST_CHECK(compare_swl(compiler, cpu::File(SOURCE_PREPROCESS)));
}

// A unit test for Source::compile() can't be made because, by design, a
// instruction in SWL can have random bits if not all the members of
// CPU::Instruction are not needed.
// For example, a instruction without parameters, stop for example, only uses
// the code and first, second and address won't be initialized to zero.
// This way, the bugs have random bits for the mutations.
