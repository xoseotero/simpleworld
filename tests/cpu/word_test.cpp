#define BOOST_TEST_MODULE Unit test for cpu/word.hpp
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/config.hpp>
#include <simpleworld/ints.hpp>
#include <simpleworld/cpu/word.hpp>
namespace sw = SimpleWorld;
namespace cpu = SimpleWorld::CPU;


//BOOST_AUTO_TEST_CASE(word_exception)
//{
//  BOOST_CHECKPOINT("Getting exception");
//
//  cpu::get_byte(0, 4);
//}

BOOST_AUTO_TEST_CASE(word_get_bytes)
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

BOOST_AUTO_TEST_CASE(word_set_bytes)
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

BOOST_AUTO_TEST_CASE(word_change_endianness)
{
  BOOST_CHECKPOINT("Changing byte order");

  BOOST_CHECK_EQUAL(0x01234567, cpu::change_byte_order(0x67452301));
}

BOOST_AUTO_TEST_CASE(word_change_endianness_middle)
{
  BOOST_CHECKPOINT("Changing byte order (middle)");

  BOOST_CHECK_EQUAL(0x01234567, cpu::change_byte_order_middle(0x23016745));
}
