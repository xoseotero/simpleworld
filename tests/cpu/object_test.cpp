/**
 * @file tests/cpu/object_test.cpp
 * Unit test for CPU::Object.
 *
 * begin:     Fri, 30 Nov 2007 18:23:08 +0100
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

#include <string>

#define BOOST_TEST_MODULE Unit test for CPU::Object
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/cpu/memory.hpp>
#include <simpleworld/cpu/cpu.hpp>
#include <simpleworld/cpu/file.hpp>
#include <simpleworld/cpu/source.hpp>
#include <simpleworld/cpu/object.hpp>
namespace sw = simpleworld;
namespace cpu = simpleworld::cpu;


#define OBJECT (TESTDATA "object.swo")
#define OBJECT_SOURCE (TESTDATA "object.swl")
#define OBJECT_SAVE (TESTOUTPUT "object_save.swl")


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
BOOST_AUTO_TEST_CASE(object_decompile)
{
  cpu::Memory registers;
  cpu::CPU cpu(&registers, NULL);
  cpu::Object decompiler(cpu.isa(), OBJECT);

  decompiler.decompile(OBJECT_SAVE);

  BOOST_CHECK(compare_swl(cpu::File(OBJECT_SAVE), cpu::File(OBJECT_SOURCE)));
}
