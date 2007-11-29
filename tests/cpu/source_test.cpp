#include <iostream>
#include <vector>
#include <string>

#define BOOST_TEST_MODULE Unit test for cpu.hpp
#define BOOST_TEST_DYN_LINK
#include <boost/filesystem/path.hpp>
#include <boost/test/unit_test.hpp>
namespace fs = boost::filesystem;

#include <simpleworld/cpu/memory.hpp>
#include <simpleworld/cpu/cpu.hpp>
#include <simpleworld/cpu/file.hpp>
#include <simpleworld/cpu/source.hpp>
namespace sw = SimpleWorld;
namespace cpu = SimpleWorld::CPU;


/**
 * Check if two files are identical.
 * @param file1 the first file.
 * @param file2 the second file.
 * @return true if they are equal, false if not.
 */
bool compare_swl(const cpu::File& file1, const cpu::File& file2)
{
  if (file1.lines() != file2.lines()) {
    std::cout << "file1.lines() = " << file1.lines() << std::endl;
    std::cout << "file2.lines() = " << file2.lines() << std::endl;
    return false;
  }

  cpu::File::size_type i;
  for (i = 0; i < file1.lines(); i++)
    if (file1[i] != file2[i]) {
      std::cout << "file1[" << i << "] = " << file1[i] << std::endl;
      std::cout << "file2[" << i << "] = " << file2[i] << std::endl;
      return false;
    }

  return true;
}


BOOST_AUTO_TEST_CASE(source_replace_includes)
{
  BOOST_CHECKPOINT("Replace the includes from the SWL");

  fs::path source(TESTDATA, fs::native);
  source /= "cpu";
  source /= "source.swl";

  fs::path source_includes(TESTDATA, fs::native);
  source_includes /= "cpu";
  source_includes /= "source_includes.swl";

  std::vector<std::string> include_path;
  fs::path include_dir(TESTDATA, fs::native);
  include_dir /= "cpu";
  include_dir /= "include";
  include_path.push_back(include_dir.string());

  cpu::Memory registers;
  cpu::CPU cpu(&registers, NULL);
  cpu::Source compiler(cpu.isa(), include_path, source.string());

  compiler.replace_includes();

  BOOST_CHECK(compare_swl(compiler,
                          cpu::File(source_includes.string())));
}

BOOST_AUTO_TEST_CASE(source_replace_constants)
{
  BOOST_CHECKPOINT("Replace the constants from the SWL");

  fs::path source(TESTDATA, fs::native);
  source /= "cpu";
  source /= "source.swl";

  fs::path source_constants(TESTDATA, fs::native);
  source_constants /= "cpu";
  source_constants /= "source_constants.swl";

  std::vector<std::string> include_path;

  cpu::Memory registers;
  cpu::CPU cpu(&registers, NULL);
  cpu::Source compiler(cpu.isa(), include_path, source.string());

  compiler.replace_constants();

  BOOST_CHECK(compare_swl(compiler,
                          cpu::File(source_constants.string())));
}

BOOST_AUTO_TEST_CASE(source_replace_blocks)
{
  BOOST_CHECKPOINT("Replace the blocks from the SWL");

  fs::path source(TESTDATA, fs::native);
  source /= "cpu";
  source /= "source.swl";

  fs::path source_blocks(TESTDATA, fs::native);
  source_blocks /= "cpu";
  source_blocks /= "source_blocks.swl";

  std::vector<std::string> include_path;

  cpu::Memory registers;
  cpu::CPU cpu(&registers, NULL);
  cpu::Source compiler(cpu.isa(), include_path, source.string());

  compiler.replace_blocks();

  BOOST_CHECK(compare_swl(compiler,
                          cpu::File(source_blocks.string())));
}

BOOST_AUTO_TEST_CASE(source_preprocess)
{
  BOOST_CHECKPOINT("Preprocess the SWL");

  fs::path source(TESTDATA, fs::native);
  source /= "cpu";
  source /= "source.swl";

  fs::path source_preprocess(TESTDATA, fs::native);
  source_preprocess /= "cpu";
  source_preprocess /= "source_preprocess.swl";

  std::vector<std::string> include_path;
  fs::path include_dir(TESTDATA, fs::native);
  include_dir /= "cpu";
  include_dir /= "include";
  include_path.push_back(include_dir.string());

  cpu::Memory registers;
  cpu::CPU cpu(&registers, NULL);
  cpu::Source compiler(cpu.isa(), include_path, source.string());

  compiler.replace_includes();
  compiler.replace_constants();
  compiler.replace_blocks();

  BOOST_CHECK(compare_swl(compiler,
                          cpu::File(source_preprocess.string())));
}

// A unit test for Source::compile() can't be made because, by design, a
// instruction in SWL can have random bits if not all the members of
// CPU::Instruction are not needed.
// For example, a instruction without parameters, stop for example, only uses
// the code and first, second and address won't be initialized to zero.
// This way, the bugs have random bits for the mutations.
