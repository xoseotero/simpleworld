/**
 * @file tests/cpu/source_test.cpp
 * Unit test for CPU::Source.
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

#define BOOST_TEST_MODULE Unit test for cpu::Source
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/cpu/types.hpp>
#include <simpleworld/cpu/instruction.hpp>
#include <simpleworld/cpu/memory.hpp>
#include <simpleworld/cpu/memory_file.hpp>
#include <simpleworld/cpu/isa.hpp>
#include <simpleworld/cpu/cpu.hpp>
#include <simpleworld/cpu/file.hpp>
#include <simpleworld/cpu/source.hpp>
namespace sw = simpleworld;
namespace cpu = simpleworld::cpu;


#define SOURCE (TESTDATA "source.swl")
#define SOURCE_PREPROCESS (TESTDATA "source_preprocess.swl")
#define SOURCE_SWO (TESTDATA "source.swo")
#define SOURCE_SAVE (TESTOUTPUT "source_save.swo")
#define INCLUDE_DIR (TESTDATA "include")


/**
 * Check if two source files are identical.
 * @param file1 the first file.
 * @param file2 the second file.
 * @return true if they are equal, false if not.
 */
bool compare_swl(const cpu::File& file1, const cpu::File& file2)
{
  if (file1.lines() != file2.lines())
    return false;

  for (cpu::File::size_type i = 0; i < file1.lines(); i++)
    if (file1[i] != file2[i])
      return false;

  return true;
}

/**
 * Check if two object files are identical.
 * @param file1 the file name of the first file.
 * @param file2 the file name of the second file.
 * @return true if they are equal, false if not.
 */
bool compare_swo(const std::string& file1, const std::string& file2)
{
  cpu::Memory registers;
  cpu::CPU cpu(&registers, NULL);
  cpu::ISA isa = cpu.isa();

  cpu::MemoryFile memory1(file1);
  cpu::MemoryFile memory2(file2);

  if (memory1.size() != memory2.size())
    return false;

  for (cpu::Address i = 0; i < memory1.size(); i += sizeof(cpu::Word)) {
    cpu::Instruction inst1(memory1.get_word(i, false));
    cpu::Instruction inst2(memory2.get_word(i, false));

    if (inst1.code != inst2.code)
      return false;

    cpu::InstructionInfo info = isa.instruction_info(inst1.code);
    if (info.nregs >= 1)
      if (inst1.first != inst2.first)
        return false;
    if (info.nregs >= 2)
      if (inst1.second != inst2.second)
        return false;
    if (info.nregs == 3 or info.has_inmediate)
      if (inst1.data != inst2.data)
        return false;
  }

  return true;
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

  compiler.preprocess();

  BOOST_CHECK(compare_swl(compiler, cpu::File(SOURCE_PREPROCESS)));
}

/**
 * Create object code.
 */
BOOST_AUTO_TEST_CASE(source_swo)
{
  std::vector<std::string> include_path;
  include_path.push_back(INCLUDE_DIR);

  cpu::Memory registers;
  cpu::CPU cpu(&registers, NULL);
  cpu::Source compiler(cpu.isa(), include_path, SOURCE);

  compiler.compile(SOURCE_SAVE);

  BOOST_CHECK(compare_swo(SOURCE_SWO, SOURCE_SAVE));
}
