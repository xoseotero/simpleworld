/**
 * @file tests/stdlib/math_test.cpp
 * Unit test for stdlib/math.swl
 *
 *  Copyright (C) 2009-2013  Xosé Otero <xoseotero@gmail.com>
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

  cpu::Source source(cpu.isa(), file);
  source.add_include_path(INCLUDE_DIR);
  source.compile(CPU_SAVE);
}


/**
 * Compile the file.
 */
BOOST_AUTO_TEST_CASE(swl_compile)
{
  cpu::File source;

  source.insert(".include \"stdlib/math.swl\"");

  source.insert("STD_MIN g0 g1 g2");
  source.insert("STD_MIN_SHORT g0 g1");
  source.insert("STD_MINU g0 g1 g2");
  source.insert("STD_MINU_SHORT g0 g1");
  source.insert("STD_MAX g0 g1 g2");
  source.insert("STD_MAX_SHORT g0 g1");
  source.insert("STD_MAXU g0 g1 g2");
  source.insert("STD_MAXU_SHORT g0 g1");
  source.insert("std_neg");
  source.insert("std_abs");
  source.insert("std_pow");
  source.insert("std_min");
  source.insert("std_minh");
  source.insert("std_minq");
  source.insert("std_minu");
  source.insert("std_minuh");
  source.insert("std_minuq");
  source.insert("std_max");
  source.insert("std_maxh");
  source.insert("std_maxq");
  source.insert("std_maxu");
  source.insert("std_maxuh");
  source.insert("std_maxuq");
  source.insert("std_sum");
  source.insert("std_sumh");
  source.insert("std_sumq");
  source.insert("std_avg");
  source.insert("std_avgh");
  source.insert("std_avgq");

  BOOST_CHECK_NO_THROW(compile(source));
}

/**
 * Check std_neg.
 */
BOOST_AUTO_TEST_CASE(std_neg)
{
  cpu::File source;

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("b main");

  source.insert(".include \"stdlib/math/neg.swl\"");

  // Test with a positive value
  source.insert(".label main");
  source.insert("loadi g0 0xF");
  source.insert("call std_neg");
  source.insert("push g0");

  // Test with a negative value
  source.insert("loadi g0 0xFFF1");
  source.insert("loadhi g0 0xFFFF");
  source.insert("call std_neg");
  source.insert("push g0");

  // Test with zero
  source.insert("loadi g0 0x0");
  source.insert("call std_neg");
  source.insert("push g0");

  // Test with 0x80000000
  source.insert("loadi g0 0x0000");
  source.insert("loadhi g0 0x8000");
  source.insert("call std_neg");
  source.insert("move g3 g0");

  // Put the results in registers
  source.insert("pop g2");
  source.insert("pop g1");
  source.insert("pop g0");

  source.insert("stop");

  // Space for 16 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x40");

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

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("b main");

  source.insert(".include \"stdlib/math/abs.swl\"");

  // Test with a positive value
  source.insert(".label main");
  source.insert("loadi g0 0xF");
  source.insert("call std_abs");
  source.insert("push g0");

  // Test with a negative value
  source.insert("loadi g0 0xFFF1");
  source.insert("loadhi g0 0xFFFF");
  source.insert("call std_abs");
  source.insert("push g0");

  // Test with zero
  source.insert("loadi g0 0x0");
  source.insert("call std_abs");
  source.insert("push g0");

  // Test with 0x80000000
  source.insert("loadi g0 0x0000");
  source.insert("loadhi g0 0x8000");
  source.insert("call std_abs");
  source.insert("move g3 g0");

  // Put the results in registers
  source.insert("pop g2");
  source.insert("pop g1");
  source.insert("pop g0");

  source.insert("stop");

  // Space for 16 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x40");

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

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("b main");

  source.insert(".include \"stdlib/math/pow.swl\"");

  // Test with a base zero
  source.insert(".label main");
  source.insert("loadi g0 0x0");
  source.insert("loadi g1 0x3");
  source.insert("call std_pow");
  source.insert("push g0");

  // Test with a base of 1
  source.insert("loadi g0 0x1");
  source.insert("loadi g1 0x4");
  source.insert("call std_pow");
  source.insert("push g0");

  // Test with a exponent zero
  source.insert("loadi g0 0x3");
  source.insert("loadi g1 0x0");
  source.insert("call std_pow");
  source.insert("push g0");

  // Test with a exponent 1
  source.insert("loadi g0 0x5");
  source.insert("loadi g1 0x1");
  source.insert("call std_pow");
  source.insert("push g0");

  // Test with other base and exponent
  source.insert("loadi g0 0x5");
  source.insert("loadi g1 0x7");
  source.insert("call std_pow");
  source.insert("move r0 g0");

  // Put the results in registers
  source.insert("pop g3");
  source.insert("pop g2");
  source.insert("pop g1");
  source.insert("pop g0");

  source.insert("stop");

  // Space for 16 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x40");

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

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("b main");

  source.insert(".include \"stdlib/math/def.swl\"");

  // Test with 2 positive values
  source.insert(".label main");
  source.insert("loadi g0 0x0");
  source.insert("loadi g1 0x3");
  source.insert("STD_MIN g0 g0 g1");
  source.insert("push g0");

  // Test with 2 negative values
  source.insert("loadi g0 0x1");
  source.insert("loadhi g0 0xFFFF");
  source.insert("loadi g1 0x4");
  source.insert("loadhi g1 0xFFFF");
  source.insert("STD_MIN_SHORT g0 g1");
  source.insert("push g0");

  // Test with a positive and a negative value
  source.insert("loadi g0 0x3");
  source.insert("loadi g1 0x0");
  source.insert("loadhi g1 0xFFFF");
  source.insert("STD_MIN_SHORT g0 g1");
  source.insert("move g2 g0");

  // Put the results in registers
  source.insert("pop g1");
  source.insert("pop g0");

  source.insert("stop");

  // Space for 16 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x40");

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
 * Check std_min.
 */
BOOST_AUTO_TEST_CASE(std_min)
{
  cpu::File source;

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("b main");

  source.insert(".include \"stdlib/math/min.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 data");
  source.insert("loada g1 data_end");
  source.insert("sub g1 g1 g0");
  source.insert("call std_min");

  source.insert("stop");

  // Data
  source.insert(".label data");
  source.insert("0x01800500");
  source.insert("0x0029D06A");
  source.insert("0x00E300B7");
  source.insert("0x8F0040C0");
  source.insert(".label data_end");

  // Space for 16 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x8F0040C0);
}

/**
 * Check std_minh.
 */
BOOST_AUTO_TEST_CASE(std_minh)
{
  cpu::File source;

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("b main");

  source.insert(".include \"stdlib/math/minh.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 data");
  source.insert("loada g1 data_end");
  source.insert("sub g1 g1 g0");
  source.insert("call std_minh");

  source.insert("stop");

  // Data
  source.insert(".label data");
  source.insert("0x01800500");
  source.insert("0x0029D06A");
  source.insert("0x00E300B7");
  source.insert("0x8F0040C0");
  source.insert(".label data_end");

  // Space for 16 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0xFFFF8F00);
}

/**
 * Check std_minq.
 */
BOOST_AUTO_TEST_CASE(std_minq)
{
  cpu::File source;

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("b main");

  source.insert(".include \"stdlib/math/minq.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 data");
  source.insert("loada g1 data_end");
  source.insert("sub g1 g1 g0");
  source.insert("call std_minq");

  source.insert("stop");

  // Data
  source.insert(".label data");
  source.insert("0x01800500");
  source.insert("0x0029D06A");
  source.insert("0x00E300B7");
  source.insert("0x8F0040C0");
  source.insert(".label data_end");

  // Space for 16 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x40");

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

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("b main");

  source.insert(".include \"stdlib/math/def.swl\"");

  // Test with 2 positive values
  source.insert(".label main");
  source.insert("loadi g0 0x0");
  source.insert("loadi g1 0x3");
  source.insert("STD_MINU g0 g0 g1");
  source.insert("push g0");

  // Test with 2 negative values
  source.insert("loadi g0 0x1");
  source.insert("loadhi g0 0xFFFF");
  source.insert("loadi g1 0x4");
  source.insert("loadhi g1 0xFFFF");
  source.insert("STD_MINU_SHORT g0 g1");
  source.insert("push g0");

  // Test with a positive and a negative value
  source.insert("loadi g0 0x3");
  source.insert("loadi g1 0x0");
  source.insert("loadhi g1 0xFFFF");
  source.insert("STD_MINU_SHORT g0 g1");
  source.insert("move g2 g0");

  // Put the results in registers
  source.insert("pop g1");
  source.insert("pop g0");

  source.insert("stop");

  // Space for 16 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x40");

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
 * Check std_minu.
 */
BOOST_AUTO_TEST_CASE(std_minu)
{
  cpu::File source;

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("b main");

  source.insert(".include \"stdlib/math/minu.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 data");
  source.insert("loada g1 data_end");
  source.insert("sub g1 g1 g0");
  source.insert("call std_minu");

  source.insert("stop");

  // Data
  source.insert(".label data");
  source.insert("0x01800500");
  source.insert("0x0029D06A");
  source.insert("0x00E300B7");
  source.insert("0x8F0040C0");
  source.insert(".label data_end");

  // Space for 16 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x0029D06A);
}

/**
 * Check std_minuh.
 */
BOOST_AUTO_TEST_CASE(std_minuh)
{
  cpu::File source;

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("b main");

  source.insert(".include \"stdlib/math/minuh.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 data");
  source.insert("loada g1 data_end");
  source.insert("sub g1 g1 g0");
  source.insert("call std_minuh");

  source.insert("stop");

  // Data
  source.insert(".label data");
  source.insert("0x01800500");
  source.insert("0x0029D06A");
  source.insert("0x00E300B7");
  source.insert("0x8F0040C0");
  source.insert(".label data_end");

  // Space for 16 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x0029);
}

/**
 * Check std_minuq.
 */
BOOST_AUTO_TEST_CASE(std_minuq)
{
  cpu::File source;

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("b main");

  source.insert(".include \"stdlib/math/minuq.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 data");
  source.insert("loada g1 data_end");
  source.insert("sub g1 g1 g0");
  source.insert("call std_minuq");

  source.insert("stop");

  // Data
  source.insert(".label data");
  source.insert("0x01800500");
  source.insert("0x0029D06A");
  source.insert("0x00E300B7");
  source.insert("0x8F0040C0");
  source.insert(".label data_end");

  // Space for 16 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x40");

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

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("b main");

  source.insert(".include \"stdlib/math/def.swl\"");

  // Test with 2 positive values
  source.insert(".label main");
  source.insert("loadi g0 0x0");
  source.insert("loadi g1 0x3");
  source.insert("STD_MAX g0 g0 g1");
  source.insert("push g0");

  // Test with 2 negative values
  source.insert("loadi g0 0x1");
  source.insert("loadhi g0 0xFFFF");
  source.insert("loadi g1 0x4");
  source.insert("loadhi g1 0xFFFF");
  source.insert("STD_MAX_SHORT g0 g1");
  source.insert("push g0");

  // Test with a positive and a negative value
  source.insert("loadi g0 0x3");
  source.insert("loadi g1 0x0");
  source.insert("loadhi g1 0xFFFF");
  source.insert("STD_MAX_SHORT g0 g1");
  source.insert("move g2 g0");

  // Put the results in registers
  source.insert("pop g1");
  source.insert("pop g0");

  source.insert("stop");

  // Space for 16 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x40");

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
 * Check std_max.
 */
BOOST_AUTO_TEST_CASE(std_max)
{
  cpu::File source;

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("b main");

  source.insert(".include \"stdlib/math/max.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 data");
  source.insert("loada g1 data_end");
  source.insert("sub g1 g1 g0");
  source.insert("call std_max");

  source.insert("stop");

  // Data
  source.insert(".label data");
  source.insert("0x01800500");
  source.insert("0x0029D06A");
  source.insert("0x00E300B7");
  source.insert("0x8F0040C0");
  source.insert(".label data_end");

  // Space for 16 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x01800500);
}

/**
 * Check std_maxh.
 */
BOOST_AUTO_TEST_CASE(std_maxh)
{
  cpu::File source;

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("b main");

  source.insert(".include \"stdlib/math/maxh.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 data");
  source.insert("loada g1 data_end");
  source.insert("sub g1 g1 g0");
  source.insert("call std_maxh");

  source.insert("stop");

  // Data
  source.insert(".label data");
  source.insert("0x01800500");
  source.insert("0x0029D06A");
  source.insert("0x00E300B7");
  source.insert("0x8F0040C0");
  source.insert(".label data_end");

  // Space for 16 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x40C0);
}

/**
 * Check std_maxq.
 */
BOOST_AUTO_TEST_CASE(std_maxq)
{
  cpu::File source;

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("b main");

  source.insert(".include \"stdlib/math/maxq.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 data");
  source.insert("loada g1 data_end");
  source.insert("sub g1 g1 g0");
  source.insert("call std_maxq");

  source.insert("stop");

  // Data
  source.insert(".label data");
  source.insert("0x01800500");
  source.insert("0x0029D06A");
  source.insert("0x00E300B7");
  source.insert("0x8F0040C0");
  source.insert(".label data_end");

  // Space for 16 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x40");

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

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("b main");

  source.insert(".include \"stdlib/math/def.swl\"");

  // Test with 2 positive values
  source.insert(".label main");
  source.insert("loadi g0 0x0");
  source.insert("loadi g1 0x3");
  source.insert("STD_MAXU g0 g0 g1");
  source.insert("push g0");

  // Test with 2 negative values
  source.insert("loadi g0 0x1");
  source.insert("loadhi g0 0xFFFF");
  source.insert("loadi g1 0x4");
  source.insert("loadhi g1 0xFFFF");
  source.insert("STD_MAXU_SHORT g0 g1");
  source.insert("push g0");

  // Test with a positive and a negative value
  source.insert("loadi g0 0x3");
  source.insert("loadi g1 0x0");
  source.insert("loadhi g1 0xFFFF");
  source.insert("STD_MAXU_SHORT g0 g1");
  source.insert("move g2 g0");

  // Put the results in registers
  source.insert("pop g1");
  source.insert("pop g0");

  source.insert("stop");

  // Space for 16 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x40");

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
 * Check std_maxu.
 */
BOOST_AUTO_TEST_CASE(std_maxu)
{
  cpu::File source;

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("b main");

  source.insert(".include \"stdlib/math/maxu.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 data");
  source.insert("loada g1 data_end");
  source.insert("sub g1 g1 g0");
  source.insert("call std_maxu");

  source.insert("stop");

  // Data
  source.insert(".label data");
  source.insert("0x01800500");
  source.insert("0x0029D06A");
  source.insert("0x00E300B7");
  source.insert("0x8F0040C0");
  source.insert(".label data_end");

  // Space for 16 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x8F0040C0);
}

/**
 * Check std_maxuh.
 */
BOOST_AUTO_TEST_CASE(std_maxuh)
{
  cpu::File source;

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("b main");

  source.insert(".include \"stdlib/math/maxuh.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 data");
  source.insert("loada g1 data_end");
  source.insert("sub g1 g1 g0");
  source.insert("call std_maxuh");

  source.insert("stop");

  // Data
  source.insert(".label data");
  source.insert("0x01800500");
  source.insert("0x0029D06A");
  source.insert("0x00E300B7");
  source.insert("0x8F0040C0");
  source.insert(".label data_end");

  // Space for 16 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0xD06A);
}

/**
 * Check std_maxuq.
 */
BOOST_AUTO_TEST_CASE(std_maxuq)
{
  cpu::File source;

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("b main");

  source.insert(".include \"stdlib/math/maxuq.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 data");
  source.insert("loada g1 data_end");
  source.insert("sub g1 g1 g0");
  source.insert("call std_maxuq");

  source.insert("stop");

  // Data
  source.insert(".label data");
  source.insert("0x01800500");
  source.insert("0x0029D06A");
  source.insert("0x00E300B7");
  source.insert("0x8F0040C0");
  source.insert(".label data_end");

  // Space for 16 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0xE3);
}

/**
 * Check std_sum.
 */
BOOST_AUTO_TEST_CASE(std_sum)
{
  cpu::File source;

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("b main");

  source.insert(".include \"stdlib/math/sum.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 data");
  source.insert("loada g1 data_end");
  source.insert("sub g1 g1 g0");
  source.insert("call std_sum");

  source.insert("stop");

  // Data
  source.insert(".label data");
  source.insert("0x01800500");
  source.insert("0x0029D06A");
  source.insert("0x00E300B7");
  source.insert("0x8F0040C0");
  source.insert(".label data_end");

  // Space for 16 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x918D16E1);
}

/**
 * Check std_sumh.
 */
BOOST_AUTO_TEST_CASE(std_sumh)
{
  cpu::File source;

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("b main");

  source.insert(".include \"stdlib/math/sumh.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 data");
  source.insert("loada g1 data_end");
  source.insert("sub g1 g1 g0");
  source.insert("call std_sumh");

  source.insert("stop");

  // Data
  source.insert(".label data");
  source.insert("0x01800500");
  source.insert("0x0029D06A");
  source.insert("0x00E300B7");
  source.insert("0x8F0040C0");
  source.insert(".label data_end");

  // Space for 16 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x1A86D);
}

/**
 * Check std_sumq.
 */
BOOST_AUTO_TEST_CASE(std_sumq)
{
  cpu::File source;

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("b main");

  source.insert(".include \"stdlib/math/sumq.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 data");
  source.insert("loada g1 data_end");
  source.insert("sub g1 g1 g0");
  source.insert("call std_sumq");

  source.insert("stop");

  // Data
  source.insert(".label data");
  source.insert("0x01800500");
  source.insert("0x0029D06A");
  source.insert("0x00E300B7");
  source.insert("0x8F0040C0");
  source.insert(".label data_end");

  // Space for 16 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x512);
}

/**
 * Check std_avg.
 */
BOOST_AUTO_TEST_CASE(std_avg)
{
  cpu::File source;

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("b main");

  source.insert(".include \"stdlib/math/avg.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 data");
  source.insert("loada g1 data_end");
  source.insert("sub g1 g1 g0");
  source.insert("call std_avg");

  source.insert("stop");

  // Data
  source.insert(".label data");
  source.insert("0x01800500");
  source.insert("0x0029D06A");
  source.insert("0x00E300B7");
  source.insert("0x8F0040C0");
  source.insert(".label data_end");

  // Space for 16 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x246345B8);
}

/**
 * Check std_avgh.
 */
BOOST_AUTO_TEST_CASE(std_avgh)
{
  cpu::File source;

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("b main");

  source.insert(".include \"stdlib/math/avgh.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 data");
  source.insert("loada g1 data_end");
  source.insert("sub g1 g1 g0");
  source.insert("call std_avgh");

  source.insert("stop");

  // Data
  source.insert(".label data");
  source.insert("0x01800500");
  source.insert("0x0029D06A");
  source.insert("0x00E300B7");
  source.insert("0x8F0040C0");
  source.insert(".label data_end");

  // Space for 16 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x350D);
}

/**
 * Check std_avgq.
 */
BOOST_AUTO_TEST_CASE(std_avgq)
{
  cpu::File source;

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("b main");

  source.insert(".include \"stdlib/math/avgq.swl\"");

  // Test
  source.insert(".label main");
  source.insert("loada g0 data");
  source.insert("loada g1 data_end");
  source.insert("sub g1 g1 g0");
  source.insert("call std_avgq");

  source.insert("stop");

  // Data
  source.insert(".label data");
  source.insert("0x01800500");
  source.insert("0x0029D06A");
  source.insert("0x00E300B7");
  source.insert("0x8F0040C0");
  source.insert(".label data_end");

  // Space for 16 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x40");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x51);
}
