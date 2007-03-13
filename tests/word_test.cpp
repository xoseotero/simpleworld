#include <boost/test/unit_test.hpp>
namespace ut = boost::unit_test;

#include <simple/config.hpp>
#include <simple/types.hpp>
#include <cpu/word.hpp>
namespace sw = SimpleWorld;
namespace cpu = SimpleWorld::CPU;


void word_exception()
{
  BOOST_CHECKPOINT("Getting exception");

  cpu::get_byte(0, 4);
}

void word_get_bytes()
{
  BOOST_CHECKPOINT("Getting bytes");

  sw::Uint32 value = 0xAABBCCDD;
#ifdef IS_BIG_ENDIAN
  BOOST_CHECK_EQUAL(cpu::get_byte(value, 0), 0xaa);
  BOOST_CHECK_EQUAL(cpu::get_byte(value, 1), 0xbb);
  BOOST_CHECK_EQUAL(cpu::get_byte(value, 2), 0xcc);
  BOOST_CHECK_EQUAL(cpu::get_byte(value, 3), 0xdd);
#else
  BOOST_CHECK_EQUAL(cpu::get_byte(value, 0), 0xdd);
  BOOST_CHECK_EQUAL(cpu::get_byte(value, 1), 0xcc);
  BOOST_CHECK_EQUAL(cpu::get_byte(value, 2), 0xbb);
  BOOST_CHECK_EQUAL(cpu::get_byte(value, 3), 0xaa);
#endif
}

void word_set_bytes()
{
  BOOST_CHECKPOINT("Setting bytes");

  sw::Uint32 value = 0;
#ifdef IS_BIG_ENDIAN
  cpu::set_byte(&value, 0, 0xaa);
  cpu::set_byte(&value, 1, 0xbb);
  cpu::set_byte(&value, 2, 0xcc);
  cpu::set_byte(&value, 3, 0xdd);
#else
  cpu::set_byte(&value, 0, 0xdd);
  cpu::set_byte(&value, 1, 0xcc);
  cpu::set_byte(&value, 2, 0xbb);
  cpu::set_byte(&value, 3, 0xaa);
#endif

  BOOST_CHECK_EQUAL(value, 0xaabbccdd);
}

void word_change_endianness()
{
  BOOST_CHECKPOINT("Changing byte order");

  BOOST_CHECK_EQUAL(0x01234567, cpu::change_byte_order(0x67452301));
}

void word_change_endianness_middle()
{
  BOOST_CHECKPOINT("Changing byte order (middle)");

  BOOST_CHECK_EQUAL(0x01234567, cpu::change_byte_order_middle(0x23016745));
}


ut::test_suite* init_unit_test_suite(int argc, char* argv[])
{
  ut::test_suite* test = BOOST_TEST_SUITE("Unit test for word.h");

  //test->add(BOOST_TEST_CASE(&word_exception), 1);
  test->add(BOOST_TEST_CASE(&word_get_bytes));
  test->add(BOOST_TEST_CASE(&word_set_bytes));
  test->add(BOOST_TEST_CASE(&word_change_endianness));
  test->add(BOOST_TEST_CASE(&word_change_endianness_middle));

  return test;
}
