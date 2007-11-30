#include <string>

#define BOOST_TEST_MODULE Unit test for CPU::Object
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/cpu/memory.hpp>
#include <simpleworld/cpu/cpu.hpp>
#include <simpleworld/cpu/file.hpp>
#include <simpleworld/cpu/source.hpp>
#include <simpleworld/cpu/object.hpp>
namespace sw = SimpleWorld;
namespace cpu = SimpleWorld::CPU;


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
