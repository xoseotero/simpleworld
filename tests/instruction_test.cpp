#include <algorithm>

#include <boost/test/unit_test.hpp>
namespace ut = boost::unit_test;

#include <simple/config.h>
#include <simple/types.h>
#include <cpu/instruction.h>
namespace sw = SimpleWorld;
namespace cpu = SimpleWorld::CPU;


void instruction_encode()
{
  BOOST_CHECKPOINT("Encode a instruction");

  cpu::Instruction inst;
  inst.code = 0x12;
  inst.first = 0x3;
  inst.second = 0x4;
  inst.address = 0x5678;
#ifdef IS_BIG_ENDIAN
  BOOST_CHECK_EQUAL(cpu::InstructionSet::encode(inst), 0x12345678);
#else
  BOOST_CHECK_EQUAL(cpu::InstructionSet::encode(inst), 0x78563412);
#endif
}

void instruction_decode()
{
  BOOST_CHECKPOINT("Decode a instruction");

#ifdef IS_BIG_ENDIAN
  sw::Uint32 inst_coded = 0x12345678;
#else
  sw::Uint32 inst_coded = 0x78563412;
#endif
  cpu::Instruction inst = cpu::InstructionSet::decode(inst_coded);

  BOOST_CHECK_EQUAL(inst.code, 0x12);
  BOOST_CHECK_EQUAL(inst.first, 0x3);
  BOOST_CHECK_EQUAL(inst.second, 0x4);
  BOOST_CHECK_EQUAL(inst.address, 0x5678);
}

void instruction_add_register()
{
  BOOST_CHECKPOINT("Add a register");

  cpu::InstructionSet set;

  sw::Uint32 code = 0xfd;
  std::string name = "test";

  set.add_register(code, name);

  BOOST_CHECK_EQUAL(set.register_code(name), code);
  BOOST_CHECK_EQUAL(set.register_name(code), name);
  std::vector<sw::Uint8> registers = set.register_codes();
  BOOST_CHECK(find(registers.begin(), registers.end(), code) !=
              registers.end());

  set.remove_register(code);
}

void instruction_add_instruction()
{
  BOOST_CHECKPOINT("Add a instruction");

  cpu::InstructionSet set;
  cpu::InstructionInfo info;
  info.code = 0xfd;
  info.name = "test";
  info.nregs = 2;
  info.has_inmediate = true;

  set.add_instruction(info);

  BOOST_CHECK_EQUAL(set.instruction_code(info.name), info.code);
  BOOST_CHECK_EQUAL(set.instruction_info(info.code).code, info.code);
  BOOST_CHECK_EQUAL(set.instruction_info(info.code).name, info.name);
  BOOST_CHECK_EQUAL(set.instruction_info(info.code).nregs, info.nregs);
  BOOST_CHECK_EQUAL(set.instruction_info(info.code).has_inmediate,
                    info.has_inmediate);
  BOOST_CHECK_EQUAL(set.instruction_info(info.code).func, info.func);
  std::vector<sw::Uint8> instructions = set.instruction_codes();
  BOOST_CHECK(find(instructions.begin(), instructions.end(), info.code) !=
              instructions.end());

  set.remove_instruction(info.code);
}


ut::test_suite* init_unit_test_suite(int argc, char* argv[])
{
  ut::test_suite* test = BOOST_TEST_SUITE("Unit test for instruction.h");

  test->add(BOOST_TEST_CASE(&instruction_encode));
  test->add(BOOST_TEST_CASE(&instruction_decode));
  test->add(BOOST_TEST_CASE(&instruction_add_register));
  test->add(BOOST_TEST_CASE(&instruction_add_instruction));

  return test;
}
