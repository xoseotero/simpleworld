#include <boost/test/unit_test.hpp>
namespace ut = boost::unit_test;

#include <simple/config.h>
#include <simple/types.h>
#include <cpu/word.h>
#include <cpu/memory.h>
namespace sw = SimpleWorld;
namespace cpu = SimpleWorld::CPU;


void memory_exception()
{
  BOOST_CHECKPOINT("Getting memory exception");

  cpu::Memory memory(16 * 4);
  memory.get_word(15 * 4 + 1);
}

void memory_is_zeroed()
{
  BOOST_CHECKPOINT("Checking if memory is zeroed");

  cpu::Memory memory(16 * 4);
  sw::Uint8 i;
  for (i = 0; i < 16 * 4; i += 4)
    BOOST_CHECK_EQUAL(memory.get_word(i), 0);
}

void memory_checking_words()
{
  BOOST_CHECKPOINT("Getting words from memory");

  cpu::Memory memory(16 * 4);
  sw::Uint8 i;
  for (i = 0; i < 16 * 4; i += 4)
    memory.set_word(i, i);

  for (i = 0; i < 16 * 4; i += 4)
    BOOST_CHECK_EQUAL(memory.get_word(i), i);
}

#ifdef IS_LITTLE_ENDIAN
void memory_set_big_endian()
{
  BOOST_CHECKPOINT("Setting big endian words");

  cpu::Memory memory(16 * 4);
  sw::Uint8 i;
  for (i = 0; i < 16 * 4; i += 4)
    memory.set_word(i, cpu::change_byte_order(i), false);

  for (i = 0; i < 16 * 4; i += 4)
    BOOST_CHECK_EQUAL(memory.get_word(i), i);
}

void memory_get_big_endian()
{
  BOOST_CHECKPOINT("Getting big endian words");

  cpu::Memory memory(16 * 4);
  sw::Uint8 i;
  for (i = 0; i < 16 * 4; i += 4)
    memory.set_word(i, i);

  for (i = 0; i < 16 * 4; i += 4)
    BOOST_CHECK_EQUAL(memory.get_word(i, false), cpu::change_byte_order(i));
}
#endif

void memory_get_non_aligned()
{
  BOOST_CHECKPOINT("Getting a non aligned word");

  cpu::Memory memory(2 * 4);
  memory.set_word(0, 0x01234567);
  memory.set_word(4, 0x89abcdef);

  BOOST_CHECK_EQUAL(memory.get_word(2), 0x456789ab);
}

void memory_Set_non_aligned()
{
  BOOST_CHECKPOINT("Setting a non aligned word");

  cpu::Memory memory(2 * 4);
  memory.set_word(0, 0x01234567);
  memory.set_word(2, 0xaabbccdd);

  BOOST_CHECK_EQUAL(memory.get_word(2), 0xaabbccdd);
}


ut::test_suite* init_unit_test_suite(int argc, char* argv[])
{
  ut::test_suite* test = BOOST_TEST_SUITE("Unit test for memory.h");

  //test->add(BOOST_TEST_CASE(&memory_exception), 1);
  test->add(BOOST_TEST_CASE(&memory_is_zeroed));
  test->add(BOOST_TEST_CASE(&memory_checking_words));
#ifdef IS_LITTLE_ENDIAN
  test->add(BOOST_TEST_CASE(&memory_set_big_endian));
  test->add(BOOST_TEST_CASE(&memory_get_big_endian));
#endif
  test->add(BOOST_TEST_CASE(&memory_get_non_aligned));

  return test;
}
