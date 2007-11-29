#include <string>

#define BOOST_TEST_MODULE Unit test for CPU::Object
#define BOOST_TEST_DYN_LINK
#include <boost/filesystem/path.hpp>
#include <boost/test/unit_test.hpp>
namespace fs = boost::filesystem;

#include <simpleworld/cpu/memory.hpp>
#include <simpleworld/cpu/cpu.hpp>
#include <simpleworld/cpu/file.hpp>
#include <simpleworld/cpu/source.hpp>
#include <simpleworld/cpu/object.hpp>
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
  if (file1.lines() != file2.lines())
    return false;

  cpu::File::size_type i;
  for (i = 0; i < file1.lines(); i++)
    if (file1[i] != file2[i])
      return false;

  return true;
}


BOOST_AUTO_TEST_CASE(object_decompile)
{
  BOOST_CHECKPOINT("Replace the includes from the SWL");

  fs::path object(TESTDATA, fs::native);
  object /= "cpu";
  object /= "object.swo";

  fs::path object_save(TESTDATA, fs::native);
  object_save /= "cpu";
  object_save /= "object_save.swl";

  fs::path object_source(TESTDATA, fs::native);
  object_source /= "cpu";
  object_source /= "object.swl";

  cpu::Memory registers;
  cpu::CPU cpu(&registers, NULL);
  cpu::Object decompiler(cpu.isa(), object.string());

  decompiler.decompile(object_save.string());

  BOOST_CHECK(compare_swl(cpu::File(object_save.string()),
                          cpu::File(object_source.string())));
}
