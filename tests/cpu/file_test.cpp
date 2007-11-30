/**
 * @file tests/cpu/file_test.cpp
 * Unit test for CPU::File.
 *
 * begin:     Fri, 30 Nov 2007 18:22:08 +0100
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

#define BOOST_TEST_MODULE Unit test for CPU::File
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/config.hpp>
#include <simpleworld/ints.hpp>
#include <simpleworld/cpu/file.hpp>
namespace sw = SimpleWorld;
namespace cpu = SimpleWorld::CPU;


#define FILE_LOAD (TESTDATA "file_load.txt")
#define FILE_SAVE (TESTOUTPUT "file_save.txt")


/**
 * Create a empty file.
 */
BOOST_AUTO_TEST_CASE(file_empty)
{
  cpu::File file;
  BOOST_CHECK_EQUAL(file.lines(), 0);
}

/**
 * Create a new file.
 */
BOOST_AUTO_TEST_CASE(file_new)
{
  cpu::File file;

  BOOST_CHECK_EQUAL(file.lines(), 0);
}

/**
 * Insert lines into a new file.
 */
BOOST_AUTO_TEST_CASE(file_insert_lines)
{
  // insert at the begining
  cpu::File file1;

  file1.insert(0, "First line");
  BOOST_CHECK_EQUAL(file1.lines(), 1);
  file1.insert(0, "Second line");
  BOOST_CHECK_EQUAL(file1.lines(), 2);
  file1.insert(0, "Third line");
  BOOST_CHECK_EQUAL(file1.lines(), 3);

  BOOST_CHECK(file1.get_line(0) == "Third line");
  BOOST_CHECK(file1.get_line(1) == "Second line");
  BOOST_CHECK(file1.get_line(2) == "First line");


  // insert at the end
  cpu::File file2;

  file2.insert(0, "First line");
  BOOST_CHECK_EQUAL(file2.lines(), 1);
  file2.insert(1, "Second line");
  BOOST_CHECK_EQUAL(file2.lines(), 2);
  file2.insert(2, "Third line");
  BOOST_CHECK_EQUAL(file2.lines(), 3);

  BOOST_CHECK(file2.get_line(0) == "First line");
  BOOST_CHECK(file2.get_line(1) == "Second line");
  BOOST_CHECK(file2.get_line(2) == "Third line");


  // insert at the middle
  cpu::File file3;

  file3.insert(0, "First line");
  BOOST_CHECK_EQUAL(file3.lines(), 1);
  file3.insert(1, "Second line");
  BOOST_CHECK_EQUAL(file3.lines(), 2);
  file3.insert(1, "Third line");
  BOOST_CHECK_EQUAL(file3.lines(), 3);

  BOOST_CHECK(file3.get_line(0) == "First line");
  BOOST_CHECK(file3.get_line(1) == "Third line");
  BOOST_CHECK(file3.get_line(2) == "Second line");
}

/**
 * Remove lines from a file.
 */
BOOST_AUTO_TEST_CASE(file_remove)
{
  cpu::File file;

  file.insert(0, "First line");
  file.insert(1, "Second line");
  file.insert(2, "Third line");

  // Remove 0 elements
  file.remove(0, 0);
  file.remove(1, 0);
  file.remove(2, 0);
  BOOST_CHECK_EQUAL(file.lines(), 3);

  // Remove the first line
  file.remove(0, 1);
  BOOST_CHECK_EQUAL(file.lines(), 2);
  BOOST_CHECK(file.get_line(0) == "Second line");

  file.insert(0, "First line");

  // Remove the last line
  file.remove(2, 1);
  BOOST_CHECK_EQUAL(file.lines(), 2);
  BOOST_CHECK(file.get_line(1) == "Second line");

  file.insert(2, "Third line");

  // Remove the second line
  file.remove(1, 1);
  BOOST_CHECK_EQUAL(file.lines(), 2);
  BOOST_CHECK(file.get_line(0) == "First line");
  BOOST_CHECK(file.get_line(1) == "Third line");

  // Remove all the lines
  file.remove(0, file.lines());
  BOOST_CHECK_EQUAL(file.lines(), 0);
}

/**
 * Load a file.
 */
BOOST_AUTO_TEST_CASE(file_load)
{
  cpu::File file;

  file.load(FILE_LOAD);
  BOOST_CHECK_EQUAL(file.lines(), 3);
  BOOST_CHECK(file.get_line(0) == "First line");
  BOOST_CHECK(file.get_line(1) == "Second line");
  BOOST_CHECK(file.get_line(2) == "Third line");
}

/**
 * Save a file.
 */
BOOST_AUTO_TEST_CASE(file_save)
{
  cpu::File file;

  file.insert(0, "First line");
  file.insert(1, "Second line");
  file.insert(2, "Third line");

  file.save(FILE_SAVE);


  // Compare with the original file
  cpu::File file1(FILE_LOAD);
  cpu::File file2(FILE_SAVE);

  BOOST_CHECK_EQUAL(file1.lines(), file2.lines());
  cpu::File::size_type i;
  for (i = 0; i < file1.lines(); i++) {
    BOOST_CHECK_EQUAL(file1.get_line(i), file2.get_line(i));
  }
}
