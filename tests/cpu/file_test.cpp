#define BOOST_TEST_MODULE Unit test for CPU::File
#define BOOST_TEST_DYN_LINK
#include <boost/filesystem/path.hpp>
#include <boost/test/unit_test.hpp>
namespace fs = boost::filesystem;

#include <simpleworld/config.hpp>
#include <simpleworld/ints.hpp>
#include <simpleworld/cpu/file.hpp>
namespace sw = SimpleWorld;
namespace cpu = SimpleWorld::CPU;


BOOST_AUTO_TEST_CASE(file_empty)
{
  BOOST_CHECKPOINT("Create a empty file");

  cpu::File file;
  BOOST_CHECK_EQUAL(file.lines(), 0);
}

BOOST_AUTO_TEST_CASE(file_new)
{
  BOOST_CHECKPOINT("Create a new file");

  cpu::File file;

  BOOST_CHECK_EQUAL(file.lines(), 0);
}

BOOST_AUTO_TEST_CASE(file_insert_lines)
{
  BOOST_CHECKPOINT("Insert lines into a new file");

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

BOOST_AUTO_TEST_CASE(file_remove)
{
  BOOST_CHECKPOINT("Remove lines from a file");

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

BOOST_AUTO_TEST_CASE(file_load)
{
  BOOST_CHECKPOINT("Load a file");

  cpu::File file;

  fs::path path(TESTDATA, fs::native);
  path /= "cpu";
  path /= "file_load.txt";
  file.load(path.string());
  BOOST_CHECK_EQUAL(file.lines(), 3);
  BOOST_CHECK(file.get_line(0) == "First line");
  BOOST_CHECK(file.get_line(1) == "Second line");
  BOOST_CHECK(file.get_line(2) == "Third line");
}

BOOST_AUTO_TEST_CASE(file_save)
{
  BOOST_CHECKPOINT("Save a file");

  cpu::File file;

  file.insert(0, "First line");
  file.insert(1, "Second line");
  file.insert(2, "Third line");

  fs::path path(TESTDATA, fs::native);
  path /= "cpu";
  path /= "file_save.txt";
  file.save(path.string());


  // Compare with the original file
  fs::path orig_path(TESTDATA, fs::native);
  orig_path /= "cpu";
  orig_path /= "file_load.txt";

  cpu::File file1(orig_path.string());
  cpu::File file2(path.string());

  BOOST_CHECK_EQUAL(file1.lines(), file2.lines());
  cpu::File::size_type i;
  for (i = 0; i < file1.lines(); i++) {
    BOOST_CHECK_EQUAL(file1.get_line(i), file2.get_line(i));
  }
}
