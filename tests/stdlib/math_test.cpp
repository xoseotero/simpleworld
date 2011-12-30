/**
 * @file tests/stdlib/math_test.cpp
 * Unit test for stdlib/math.swl
 *
 *  Copyright (C) 2009-2011  Xosé Otero <xoseotero@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define BOOST_TEST_MODULE Unit test for stdlib/math.swl
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/cpu/memory.hpp>
#include <simpleworld/cpu/memory_file.hpp>
#include <simpleworld/cpu/file.hpp>
#include <simpleworld/cpu/source.hpp>
namespace sw = simpleworld;
namespace cpu = simpleworld::cpu;

#include "src/common/fakecpu.hpp"


#define REGISTER(cpu, name) ADDRESS((cpu).isa().register_code(name))
#define CPU_SAVE (TESTOUTPUT "math.swo")
#define MAX_CYCLES 4096


/**
 * Compile a file to CPU_SAVE.
 * @param file file to compile
 */
void compile(const cpu::File& file)
{
  cpu::Memory registers;
  FakeCPU cpu(&registers, NULL);

  std::vector<std::string> include_path;
  include_path.push_back(INCLUDE_DIR);
  cpu::Source source(cpu.isa(), include_path);

  source.insert(0, file);
  source.compile(CPU_SAVE);
}


/**
 * Compile the file.
 */
BOOST_AUTO_TEST_CASE(swl_compile)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  source.insert(line++, ".include \"stdlib/math.swl\"");
  BOOST_CHECK_NO_THROW(compile(source));
}

/**
 * Check std_neg.
 */
BOOST_AUTO_TEST_CASE(std_neg)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/math/neg.swl\"");

  // Test with a positive value
  source.insert(line++, ".label main");
  source.insert(line++, "loadi g0 0xF");
  source.insert(line++, "call std_neg");
  source.insert(line++, "push g0");

  // Test with a negative value
  source.insert(line++, "loadi g0 0xFFF1");
  source.insert(line++, "loadhi g0 0xFFFF");
  source.insert(line++, "call std_neg");
  source.insert(line++, "push g0");

  // Test with zero
  source.insert(line++, "loadi g0 0x0");
  source.insert(line++, "call std_neg");
  source.insert(line++, "push g0");

  // Test with 0x80000000
  source.insert(line++, "loadi g0 0x0000");
  source.insert(line++, "loadhi g0 0x8000");
  source.insert(line++, "call std_neg");
  source.insert(line++, "move g3 g0");

  // Put the results in registers
  source.insert(line++, "pop g2");
  source.insert(line++, "pop g1");
  source.insert(line++, "pop g0");

  source.insert(line++, "stop");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0xFFFFFFF1);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0xF);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g2")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g3")], 0x80000000);
}

/**
 * Check std_abs.
 */
BOOST_AUTO_TEST_CASE(std_abs)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/math/abs.swl\"");

  // Test with a positive value
  source.insert(line++, ".label main");
  source.insert(line++, "loadi g0 0xF");
  source.insert(line++, "call std_abs");
  source.insert(line++, "push g0");

  // Test with a negative value
  source.insert(line++, "loadi g0 0xFFF1");
  source.insert(line++, "loadhi g0 0xFFFF");
  source.insert(line++, "call std_abs");
  source.insert(line++, "push g0");

  // Test with zero
  source.insert(line++, "loadi g0 0x0");
  source.insert(line++, "call std_abs");
  source.insert(line++, "push g0");

  // Test with 0x80000000
  source.insert(line++, "loadi g0 0x0000");
  source.insert(line++, "loadhi g0 0x8000");
  source.insert(line++, "call std_abs");
  source.insert(line++, "move g3 g0");

  // Put the results in registers
  source.insert(line++, "pop g2");
  source.insert(line++, "pop g1");
  source.insert(line++, "pop g0");

  source.insert(line++, "stop");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0xF);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0xF);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g2")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g3")], 0x80000000);
}

/**
 * Check std_pow.
 */
BOOST_AUTO_TEST_CASE(std_pow)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/math/pow.swl\"");

  // Test with a base zero
  source.insert(line++, ".label main");
  source.insert(line++, "loadi g0 0x0");
  source.insert(line++, "loadi g1 0x3");
  source.insert(line++, "call std_pow");
  source.insert(line++, "push g0");

  // Test with a base of 1
  source.insert(line++, "loadi g0 0x1");
  source.insert(line++, "loadi g1 0x4");
  source.insert(line++, "call std_pow");
  source.insert(line++, "push g0");

  // Test with a exponent zero
  source.insert(line++, "loadi g0 0x3");
  source.insert(line++, "loadi g1 0x0");
  source.insert(line++, "call std_pow");
  source.insert(line++, "push g0");

  // Test with a exponent 1
  source.insert(line++, "loadi g0 0x5");
  source.insert(line++, "loadi g1 0x1");
  source.insert(line++, "call std_pow");
  source.insert(line++, "push g0");

  // Test with other base and exponent
  source.insert(line++, "loadi g0 0x5");
  source.insert(line++, "loadi g1 0x7");
  source.insert(line++, "call std_pow");
  source.insert(line++, "move r0 g0");

  // Put the results in registers
  source.insert(line++, "pop g3");
  source.insert(line++, "pop g2");
  source.insert(line++, "pop g1");
  source.insert(line++, "pop g0");

  source.insert(line++, "stop");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0x1);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g2")], 0x1);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g3")], 0x5);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0x1312D);
}

/**
 * Check std_min.
 */
BOOST_AUTO_TEST_CASE(STD_MIN)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/math/def.swl\"");

  // Test with 2 positive values
  source.insert(line++, ".label main");
  source.insert(line++, "loadi g0 0x0");
  source.insert(line++, "loadi g1 0x3");
  source.insert(line++, "STD_MIN g0 g0 g1");
  source.insert(line++, "push g0");

  // Test with 2 negative values
  source.insert(line++, "loadi g0 0x1");
  source.insert(line++, "loadhi g0 0xFFFF");
  source.insert(line++, "loadi g1 0x4");
  source.insert(line++, "loadhi g1 0xFFFF");
  source.insert(line++, "STD_MIN_SHORT g0 g1");
  source.insert(line++, "push g0");

  // Test with a positive and a negative value
  source.insert(line++, "loadi g0 0x3");
  source.insert(line++, "loadi g1 0x0");
  source.insert(line++, "loadhi g1 0xFFFF");
  source.insert(line++, "STD_MIN_SHORT g0 g1");
  source.insert(line++, "move g2 g0");

  // Put the results in registers
  source.insert(line++, "pop g1");
  source.insert(line++, "pop g0");

  source.insert(line++, "stop");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0xFFFF0001);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g2")], 0xFFFF0000);
}

/**
 * Check std_bmin.
 */
BOOST_AUTO_TEST_CASE(std_bmin)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/math/bmin.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "call std_bmin");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label data");
  source.insert(line++, "0x01800500");
  source.insert(line++, "0x0029D06A");
  source.insert(line++, "0x00E300B7");
  source.insert(line++, "0x8F0040C0");
  source.insert(line++, ".label data_end");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x8F0040C0);
}

/**
 * Check std_bminh.
 */
BOOST_AUTO_TEST_CASE(std_bminh)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/math/bminh.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "call std_bminh");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label data");
  source.insert(line++, "0x01800500");
  source.insert(line++, "0x0029D06A");
  source.insert(line++, "0x00E300B7");
  source.insert(line++, "0x8F0040C0");
  source.insert(line++, ".label data_end");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0xFFFF8F00);
}

/**
 * Check std_bminq.
 */
BOOST_AUTO_TEST_CASE(std_bminq)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/math/bminq.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "call std_bminq");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label data");
  source.insert(line++, "0x01800500");
  source.insert(line++, "0x0029D06A");
  source.insert(line++, "0x00E300B7");
  source.insert(line++, "0x8F0040C0");
  source.insert(line++, ".label data_end");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0xFFFFFF80);
}

/**
 * Check std_minu.
 */
BOOST_AUTO_TEST_CASE(STD_MINU)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/math/def.swl\"");

  // Test with 2 positive values
  source.insert(line++, ".label main");
  source.insert(line++, "loadi g0 0x0");
  source.insert(line++, "loadi g1 0x3");
  source.insert(line++, "STD_MINU g0 g0 g1");
  source.insert(line++, "push g0");

  // Test with 2 negative values
  source.insert(line++, "loadi g0 0x1");
  source.insert(line++, "loadhi g0 0xFFFF");
  source.insert(line++, "loadi g1 0x4");
  source.insert(line++, "loadhi g1 0xFFFF");
  source.insert(line++, "STD_MINU_SHORT g0 g1");
  source.insert(line++, "push g0");

  // Test with a positive and a negative value
  source.insert(line++, "loadi g0 0x3");
  source.insert(line++, "loadi g1 0x0");
  source.insert(line++, "loadhi g1 0xFFFF");
  source.insert(line++, "STD_MINU_SHORT g0 g1");
  source.insert(line++, "move g2 g0");

  // Put the results in registers
  source.insert(line++, "pop g1");
  source.insert(line++, "pop g0");

  source.insert(line++, "stop");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0xFFFF0001);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g2")], 0x3);
}

/**
 * Check std_bminu.
 */
BOOST_AUTO_TEST_CASE(std_bminu)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/math/bminu.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "call std_bminu");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label data");
  source.insert(line++, "0x01800500");
  source.insert(line++, "0x0029D06A");
  source.insert(line++, "0x00E300B7");
  source.insert(line++, "0x8F0040C0");
  source.insert(line++, ".label data_end");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x0029D06A);
}

/**
 * Check std_bminuh.
 */
BOOST_AUTO_TEST_CASE(std_bminuh)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/math/bminuh.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "call std_bminuh");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label data");
  source.insert(line++, "0x01800500");
  source.insert(line++, "0x0029D06A");
  source.insert(line++, "0x00E300B7");
  source.insert(line++, "0x8F0040C0");
  source.insert(line++, ".label data_end");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x0029);
}

/**
 * Check std_bminuq.
 */
BOOST_AUTO_TEST_CASE(std_bminuq)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/math/bminuq.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "call std_bminuq");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label data");
  source.insert(line++, "0x01800500");
  source.insert(line++, "0x0029D06A");
  source.insert(line++, "0x00E300B7");
  source.insert(line++, "0x8F0040C0");
  source.insert(line++, ".label data_end");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x00);
}

/**
 * Check std_max.
 */
BOOST_AUTO_TEST_CASE(STD_MAX)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/math/def.swl\"");

  // Test with 2 positive values
  source.insert(line++, ".label main");
  source.insert(line++, "loadi g0 0x0");
  source.insert(line++, "loadi g1 0x3");
  source.insert(line++, "STD_MAX g0 g0 g1");
  source.insert(line++, "push g0");

  // Test with 2 negative values
  source.insert(line++, "loadi g0 0x1");
  source.insert(line++, "loadhi g0 0xFFFF");
  source.insert(line++, "loadi g1 0x4");
  source.insert(line++, "loadhi g1 0xFFFF");
  source.insert(line++, "STD_MAX_SHORT g0 g1");
  source.insert(line++, "push g0");

  // Test with a positive and a negative value
  source.insert(line++, "loadi g0 0x3");
  source.insert(line++, "loadi g1 0x0");
  source.insert(line++, "loadhi g1 0xFFFF");
  source.insert(line++, "STD_MAX_SHORT g0 g1");
  source.insert(line++, "move g2 g0");

  // Put the results in registers
  source.insert(line++, "pop g1");
  source.insert(line++, "pop g0");

  source.insert(line++, "stop");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x3);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0xFFFF0004);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g2")], 0x3);
}

/**
 * Check std_bmax.
 */
BOOST_AUTO_TEST_CASE(std_bmax)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/math/bmax.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "call std_bmax");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label data");
  source.insert(line++, "0x01800500");
  source.insert(line++, "0x0029D06A");
  source.insert(line++, "0x00E300B7");
  source.insert(line++, "0x8F0040C0");
  source.insert(line++, ".label data_end");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x01800500);
}

/**
 * Check std_bmaxh.
 */
BOOST_AUTO_TEST_CASE(std_bmaxh)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/math/bmaxh.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "call std_bmaxh");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label data");
  source.insert(line++, "0x01800500");
  source.insert(line++, "0x0029D06A");
  source.insert(line++, "0x00E300B7");
  source.insert(line++, "0x8F0040C0");
  source.insert(line++, ".label data_end");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x40C0);
}

/**
 * Check std_bmaxq.
 */
BOOST_AUTO_TEST_CASE(std_bmaxq)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/math/bmaxq.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "call std_bmaxq");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label data");
  source.insert(line++, "0x01800500");
  source.insert(line++, "0x0029D06A");
  source.insert(line++, "0x00E300B7");
  source.insert(line++, "0x8F0040C0");
  source.insert(line++, ".label data_end");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x6A);
}

/**
 * Check std_maxu.
 */
BOOST_AUTO_TEST_CASE(STD_MAXU)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/math/def.swl\"");

  // Test with 2 positive values
  source.insert(line++, ".label main");
  source.insert(line++, "loadi g0 0x0");
  source.insert(line++, "loadi g1 0x3");
  source.insert(line++, "STD_MAXU g0 g0 g1");
  source.insert(line++, "push g0");

  // Test with 2 negative values
  source.insert(line++, "loadi g0 0x1");
  source.insert(line++, "loadhi g0 0xFFFF");
  source.insert(line++, "loadi g1 0x4");
  source.insert(line++, "loadhi g1 0xFFFF");
  source.insert(line++, "STD_MAXU_SHORT g0 g1");
  source.insert(line++, "push g0");

  // Test with a positive and a negative value
  source.insert(line++, "loadi g0 0x3");
  source.insert(line++, "loadi g1 0x0");
  source.insert(line++, "loadhi g1 0xFFFF");
  source.insert(line++, "STD_MAXU_SHORT g0 g1");
  source.insert(line++, "move g2 g0");

  // Put the results in registers
  source.insert(line++, "pop g1");
  source.insert(line++, "pop g0");

  source.insert(line++, "stop");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x3);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0xFFFF0004);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g2")], 0xFFFF0000);
}

/**
 * Check std_bmaxu.
 */
BOOST_AUTO_TEST_CASE(std_bmaxu)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/math/bmaxu.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "call std_bmaxu");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label data");
  source.insert(line++, "0x01800500");
  source.insert(line++, "0x0029D06A");
  source.insert(line++, "0x00E300B7");
  source.insert(line++, "0x8F0040C0");
  source.insert(line++, ".label data_end");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x8F0040C0);
}

/**
 * Check std_bmaxuh.
 */
BOOST_AUTO_TEST_CASE(std_bmaxuh)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/math/bmaxuh.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "call std_bmaxuh");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label data");
  source.insert(line++, "0x01800500");
  source.insert(line++, "0x0029D06A");
  source.insert(line++, "0x00E300B7");
  source.insert(line++, "0x8F0040C0");
  source.insert(line++, ".label data_end");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0xD06A);
}

/**
 * Check std_bmaxuq.
 */
BOOST_AUTO_TEST_CASE(std_bmaxuq)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/math/bmaxuq.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "call std_bmaxuq");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label data");
  source.insert(line++, "0x01800500");
  source.insert(line++, "0x0029D06A");
  source.insert(line++, "0x00E300B7");
  source.insert(line++, "0x8F0040C0");
  source.insert(line++, ".label data_end");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0xE3);
}

/**
 * Check std_bsum.
 */
BOOST_AUTO_TEST_CASE(std_bsum)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/math/bsum.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "call std_bsum");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label data");
  source.insert(line++, "0x01800500");
  source.insert(line++, "0x0029D06A");
  source.insert(line++, "0x00E300B7");
  source.insert(line++, "0x8F0040C0");
  source.insert(line++, ".label data_end");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x918D16E1);
}

/**
 * Check std_bsumh.
 */
BOOST_AUTO_TEST_CASE(std_bsumh)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/math/bsumh.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "call std_bsumh");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label data");
  source.insert(line++, "0x01800500");
  source.insert(line++, "0x0029D06A");
  source.insert(line++, "0x00E300B7");
  source.insert(line++, "0x8F0040C0");
  source.insert(line++, ".label data_end");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x1A86D);
}

/**
 * Check std_bsumq.
 */
BOOST_AUTO_TEST_CASE(std_bsumq)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/math/bsumq.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "call std_bsumq");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label data");
  source.insert(line++, "0x01800500");
  source.insert(line++, "0x0029D06A");
  source.insert(line++, "0x00E300B7");
  source.insert(line++, "0x8F0040C0");
  source.insert(line++, ".label data_end");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x512);
}

/**
 * Check std_bavg.
 */
BOOST_AUTO_TEST_CASE(std_bavg)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/math/bavg.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "call std_bavg");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label data");
  source.insert(line++, "0x01800500");
  source.insert(line++, "0x0029D06A");
  source.insert(line++, "0x00E300B7");
  source.insert(line++, "0x8F0040C0");
  source.insert(line++, ".label data_end");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x246345B8);
}

/**
 * Check std_bavgh.
 */
BOOST_AUTO_TEST_CASE(std_bavgh)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/math/bavgh.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "call std_bavgh");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label data");
  source.insert(line++, "0x01800500");
  source.insert(line++, "0x0029D06A");
  source.insert(line++, "0x00E300B7");
  source.insert(line++, "0x8F0040C0");
  source.insert(line++, ".label data_end");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x350D);
}

/**
 * Check std_bavgq.
 */
BOOST_AUTO_TEST_CASE(std_bavgq)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/math/bavgq.swl\"");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loada g1 data_end");
  source.insert(line++, "sub g1 g1 g0");
  source.insert(line++, "call std_bavgq");

  source.insert(line++, "stop");

  // Data
  source.insert(line++, ".label data");
  source.insert(line++, "0x01800500");
  source.insert(line++, "0x0029D06A");
  source.insert(line++, "0x00E300B7");
  source.insert(line++, "0x8F0040C0");
  source.insert(line++, ".label data_end");

  // Space for 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x51);
}
