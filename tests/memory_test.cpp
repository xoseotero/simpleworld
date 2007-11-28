#define BOOST_TEST_MODULE Unit test for memory.hpp
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/config.hpp>
#include <simpleworld/ints.hpp>
#include <simpleworld/cpu/word.hpp>
#include <simpleworld/cpu/memory.hpp>
namespace sw = SimpleWorld;
namespace cpu = SimpleWorld::CPU;


BOOST_AUTO_TEST_CASE(memory_exception)
{
  BOOST_CHECKPOINT("Getting memory exception");

  cpu::Memory memory(16 * 4);
  memory.get_word(15 * 4 + 1);
}

BOOST_AUTO_TEST_CASE(memory_is_zeroed)
{
  BOOST_CHECKPOINT("Checking if memory is zeroed");

  cpu::Memory memory(16 * 4);
  sw::Uint8 i;
  for (i = 0; i < 16 * 4; i += 4)
    BOOST_CHECK_EQUAL(memory.get_word(i), 0);
}

BOOST_AUTO_TEST_CASE(memory_checking_words)
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
BOOST_AUTO_TEST_CASE(memory_set_big_endian)
{
  BOOST_CHECKPOINT("Setting big endian words");

  cpu::Memory memory(16 * 4);
  sw::Uint8 i;
  for (i = 0; i < 16 * 4; i += 4)
    memory.set_word(i, cpu::change_byte_order(i), false);

  for (i = 0; i < 16 * 4; i += 4)
    BOOST_CHECK_EQUAL(memory.get_word(i), i);
}

BOOST_AUTO_TEST_CASE(memory_get_big_endian)
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

BOOST_AUTO_TEST_CASE(memory_get_non_aligned)
{
  BOOST_CHECKPOINT("Getting a non aligned word");

  cpu::Memory memory(2 * 4);
  memory.set_word(0, 0x01234567);
  memory.set_word(4, 0x89abcdef);

  BOOST_CHECK_EQUAL(memory.get_word(2), 0x456789ab);
}

BOOST_AUTO_TEST_CASE(memory_Set_non_aligned)
{
  BOOST_CHECKPOINT("Setting a non aligned word");

  cpu::Memory memory(2 * 4);
  memory.set_word(0, 0x01234567);
  memory.set_word(2, 0xaabbccdd);

  BOOST_CHECK_EQUAL(memory.get_word(2), 0xaabbccdd);
}
