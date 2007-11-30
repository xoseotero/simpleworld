#include <algorithm>

#define BOOST_TEST_MODULE Unit test for CPU::Instruction
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/config.hpp>
#include <simpleworld/ints.hpp>
#include <simpleworld/cpu/instruction.hpp>
namespace sw = SimpleWorld;
namespace cpu = SimpleWorld::CPU;


/**
 * Encode a instruction.
 */
BOOST_AUTO_TEST_CASE(instruction_encode)
{
  cpu::Instruction inst;
  inst.code = 0x12;
  inst.first = 0x3;
  inst.second = 0x4;
  inst.address = 0x5678;
#ifdef IS_BIG_ENDIAN
  BOOST_CHECK_EQUAL(cpu::Instruction::encode(inst), 0x12345678);
#else
  BOOST_CHECK_EQUAL(cpu::Instruction::encode(inst), 0x78563412);
#endif
}

/**
 * Decode a instruction.
 */
BOOST_AUTO_TEST_CASE(instruction_decode)
{
#ifdef IS_BIG_ENDIAN
  sw::Uint32 inst_coded = 0x12345678;
#else
  sw::Uint32 inst_coded = 0x78563412;
#endif
  cpu::Instruction inst = cpu::Instruction::decode(inst_coded);

  BOOST_CHECK_EQUAL(inst.code, 0x12);
  BOOST_CHECK_EQUAL(inst.first, 0x3);
  BOOST_CHECK_EQUAL(inst.second, 0x4);
  BOOST_CHECK_EQUAL(inst.address, 0x5678);
}
