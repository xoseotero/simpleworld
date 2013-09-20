/**
 * @file tests/cpu/cpu_test.cpp
 * Unit test for CPU::CPU.
 *
 *  Copyright (C) 2007-2013  Xosé Otero <xoseotero@gmail.com>
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

#include <typeinfo>
#include <cxxabi.h>

#define BOOST_TEST_MODULE Unit test for cpu::CPU
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/config.hpp>
#include <simpleworld/ints.hpp>
#include <simpleworld/cpu/types.hpp>
#include <simpleworld/cpu/memory.hpp>
#include <simpleworld/cpu/memory_file.hpp>
#include <simpleworld/cpu/cpu.hpp>
#include <simpleworld/cpu/file.hpp>
#include <simpleworld/cpu/source.hpp>
namespace sw = simpleworld;
namespace cpu = simpleworld::cpu;


#define REGISTER(cpu, name) ADDRESS((cpu).isa().register_code(name))

#define CPU_SAVE (TESTOUTPUT "cpu_save.swo")


/**
 * Compile a file to CPU_SAVE.
 * @param file file to compile
 */
void compile(const cpu::File& file)
{
  cpu::Memory registers;
  cpu::CPU cpu(&registers, NULL);

  cpu::Source source(cpu.isa(), file);
  source.compile(CPU_SAVE);
}


/**
 * Check if the registers are initialized to 0.
 */
BOOST_AUTO_TEST_CASE(cpu_initialization)
{
  cpu::Memory registers;
  cpu::CPU cpu(&registers, NULL);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "wc")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "pc")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "sp")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "ip")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g2")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g3")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "lr")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "fp")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r1")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r2")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r3")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r4")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r5")], 0x0);
}

/**
 * Check the getters/setters of the registers.
 */
BOOST_AUTO_TEST_CASE(cpu_reg)
{
  cpu::Memory registers;
  cpu::CPU cpu(&registers, NULL);

  cpu.set_reg(0, 0x0123ABCD);
  cpu.set_reg(15, 0x01100110);

  BOOST_CHECK_EQUAL(cpu.get_reg(0), 0x0123ABCD);
  BOOST_CHECK_EQUAL(cpu.get_reg(0, true), 0x0123ABCD);
#if defined(IS_BIG_ENDIAN)
  BOOST_CHECK_EQUAL(cpu.get_reg(0, false),
                    cpu.get_reg(0, true));
#elif defined(IS_LITTLE_ENDIAN)
  BOOST_CHECK_EQUAL(cpu.get_reg(0, false), 0xCDAB2301);
#else
#error endianness not defined
#endif
  BOOST_CHECK_EQUAL(cpu.get_reg(15), 0x01100110);
}

/**
 * Check the getters/setters of the memory (words).
 */
BOOST_AUTO_TEST_CASE(cpu_mem_word)
{
  cpu::Memory registers;
  cpu::Memory memory(16 * sizeof(cpu::Word));
  cpu::CPU cpu(&registers, &memory);

  cpu.set_mem(0, 0x0123ABCD);
  cpu.set_mem(15, 0x01100110);

  BOOST_CHECK_EQUAL(cpu.get_mem(0), 0x0123ABCD);
  BOOST_CHECK_EQUAL(cpu.get_mem(0, true), 0x0123ABCD);
#if defined(IS_BIG_ENDIAN)
  BOOST_CHECK_EQUAL(cpu.get_mem(0, false),
                    cpu.get_mem(0, true));
#elif defined(IS_LITTLE_ENDIAN)
  BOOST_CHECK_EQUAL(cpu.get_mem(0, false), 0xCDAB2301);
#else
#error endianness not defined
#endif
  BOOST_CHECK_EQUAL(cpu.get_mem(15), 0x01100110);
}

/**
 * Check the getters/setters of the memory (half words).
 */
BOOST_AUTO_TEST_CASE(cpu_mem_halfword)
{
  cpu::Memory registers;
  cpu::Memory memory(16 * sizeof(cpu::Word));
  cpu::CPU cpu(&registers, &memory);

  cpu.set_halfmem(4, 0x0123);
  cpu.set_halfmem(6, 0xABCD);

  BOOST_CHECK_EQUAL(cpu.get_halfmem(4), 0x0123);
  BOOST_CHECK_EQUAL(cpu.get_halfmem(4, true), 0x0123);
#if defined(IS_BIG_ENDIAN)
  BOOST_CHECK_EQUAL(cpu.get_halfmem(4, false),
                    cpu.get_halfmem(4, true));
#elif defined(IS_LITTLE_ENDIAN)
  BOOST_CHECK_EQUAL(cpu.get_halfmem(4, false), 0x2301);
#else
#error endianness not defined
#endif
  BOOST_CHECK_EQUAL(cpu.get_halfmem(6), 0xABCD);

  BOOST_CHECK_EQUAL(cpu.get_mem(4), 0x0123ABCD);
}

/**
 * Check the getters/setters of the memory (quarter words).
 */
BOOST_AUTO_TEST_CASE(cpu_mem_quarterword)
{
  cpu::Memory registers;
  cpu::Memory memory(16 * sizeof(cpu::Word));
  cpu::CPU cpu(&registers, &memory);

  cpu.set_quartermem(8, 0x01);
  cpu.set_quartermem(9, 0x23);
  cpu.set_quartermem(10, 0xAB);
  cpu.set_quartermem(11, 0xCD);

  BOOST_CHECK_EQUAL(cpu.get_quartermem(8), 0x01);
  BOOST_CHECK_EQUAL(cpu.get_quartermem(9), 0x23);
  BOOST_CHECK_EQUAL(cpu.get_quartermem(10), 0xAB);
  BOOST_CHECK_EQUAL(cpu.get_quartermem(11), 0xCD);

  BOOST_CHECK_EQUAL(cpu.get_mem(8), 0x0123ABCD);
}

/**
 * Execute the stop instruction.
 */
BOOST_AUTO_TEST_CASE(cpu_management_stop)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  source.insert(line++, "stop");
  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);
  cpu.execute(line);

  BOOST_CHECK_EQUAL(cpu.running(), false);
}

/**
 * Execute the restart instruction.
 */
BOOST_AUTO_TEST_CASE(cpu_management_restart)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  source.insert(line++, "restart");
  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);
  cpu.execute(line);

  // All the registers must be set to 0
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "wc")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "pc")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "sp")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "ip")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g2")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g3")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "lr")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "fp")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r1")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r2")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r3")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r4")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r5")], 0x0);
}

/**
 * Execute the load operations (special).
 * Operations: loadi, loadhi, loada
 */
BOOST_AUTO_TEST_CASE(cpu_load_special)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Load the address
  source.insert(line++, "loada g0 data");

  // Load inmediate values
  source.insert(line++, "loadi g1 0x0");
  source.insert(line++, "loadi g2 0x4");
  source.insert(line++, "loadi r0 0x8");
  source.insert(line++, "loadi r1 0xc");

  // Load inmediate values in the higher 16 bits
  source.insert(line++, "loadhi g1 0xc");
  source.insert(line++, "loadhi g2 0x8");
  source.insert(line++, "loadhi r0 0x4");
  source.insert(line++, "loadhi r1 0x0");

  // Data
  sw::Uint8 data = line;
  source.insert(line++, ".label data");
  source.insert(line++, "0x1234abcd");

  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);
  cpu.execute(data);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")],
                    memory.size() - sizeof(cpu::Word));
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0xc0000);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g2")], 0x80004);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0x40008);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r1")], 0xc);
}

/**
 * Execute the load operations (32 bits).
 * Operations: load, loadrr, loadri
 */
BOOST_AUTO_TEST_CASE(cpu_load_word)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Load a inmediate value
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loadi g1 0x0");
  source.insert(line++, "loadi g2 0x4");
  source.insert(line++, "loadi g3 0x8");
  source.insert(line++, "loadi r0 0xc");

  // Load data with a pointer
  source.insert(line++, "load r1 data");

  // Load data with a offset (register)
  source.insert(line++, "loadrr r2 g0 g1");
  source.insert(line++, "loadrr r3 g0 g2");
  source.insert(line++, "loadrr r4 g0 g3");
  source.insert(line++, "loadrr r5 g0 r0");

  // Load data with a offset (inmediate)
  source.insert(line++, "loadri sp g0 0x0");
  source.insert(line++, "loadri fp g0 0x4");
  source.insert(line++, "loadri ip g0 0x8");

  source.insert(line++, "loadi g0 0x0");
  source.insert(line++, "loadi g1 0x10");
  source.insert(line++, "loadhi g2 0xffff");
  source.insert(line++, "loadhi g3 0x7f07");

  // Data to be loaded into the registers
  sw::Uint8 data = line;
  source.insert(line++, ".label data");
  source.insert(line++, "0x1234abcd");
  source.insert(line++, "0xef567890");
  source.insert(line++, "0x77777757");
  source.insert(line++, "0x01010101");

  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);
  cpu.execute(data);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x0000);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0x0010);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g2")], 0xffff0004);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g3")], 0x7f070008);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0x000c);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r1")], 0x1234abcd);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r2")], 0x1234abcd);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r3")], 0xef567890);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r4")], 0x77777757);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r5")], 0x01010101);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "sp")], 0x1234abcd);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "fp")], 0xef567890);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "ip")], 0x77777757);
}

/**
 * Execute the load operations (16 bits).
 * Operations: loadh, loadhrr, loadhri
 */
BOOST_AUTO_TEST_CASE(cpu_load_haldword)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Load a inmediate value
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loadi g1 0x0");
  source.insert(line++, "loadi g2 0x2");
  source.insert(line++, "loadi g3 0x4");
  source.insert(line++, "loadi r0 0x6");

  // Load data with a pointer
  source.insert(line++, "loadh r1 data");

  // Load data with a offset (register)
  source.insert(line++, "loadhrr r2 g0 g1");
  source.insert(line++, "loadhrr r3 g0 g2");
  source.insert(line++, "loadhrr r4 g0 g3");
  source.insert(line++, "loadhrr r5 g0 r0");

  // Load data with a offset (inmediate)
  source.insert(line++, "loadhri sp g0 0x0");
  source.insert(line++, "loadhri fp g0 0x4");
  source.insert(line++, "loadhri ip g0 0x8");

  // Data to be loaded into the registers
  sw::Uint8 data = line;
  source.insert(line++, ".label data");
  source.insert(line++, "0x1234abcd");
  source.insert(line++, "0xef567890");
  source.insert(line++, "0x77777757");
  source.insert(line++, "0x01010101");

  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);
  cpu.execute(data);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r1")], 0x1234);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r2")], 0x1234);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r3")], 0xabcd);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r4")], 0xef56);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r5")], 0x7890);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "sp")], 0x1234);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "fp")], 0xef56);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "ip")], 0x7777);
}

/**
 * Execute the load operations (8 bits).
 * Operations: loadq, loadqrr, loadqri
 */
BOOST_AUTO_TEST_CASE(cpu_load_quarterword)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Load a inmediate value
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loadi g1 0x0");
  source.insert(line++, "loadi g2 0x1");
  source.insert(line++, "loadi g3 0x2");
  source.insert(line++, "loadi r0 0x3");

  // Load data with a pointer
  source.insert(line++, "loadq r1 data");

  // Load data with a offset (register)
  source.insert(line++, "loadqrr r2 g0 g1");
  source.insert(line++, "loadqrr r3 g0 g2");
  source.insert(line++, "loadqrr r4 g0 g3");
  source.insert(line++, "loadqrr r5 g0 r0");

  // Load data with a offset (inmediate)
  source.insert(line++, "loadqri sp g0 0x0");
  source.insert(line++, "loadqri fp g0 0x4");
  source.insert(line++, "loadqri ip g0 0x8");

  // Data to be loaded into the registers
  sw::Uint8 data = line;
  source.insert(line++, ".label data");
  source.insert(line++, "0x1234abcd");
  source.insert(line++, "0xef567890");
  source.insert(line++, "0x77777757");
  source.insert(line++, "0x01010101");

  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);
  cpu.execute(data);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r1")], 0x12);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r2")], 0x12);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r3")], 0x34);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r4")], 0xab);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r5")], 0xcd);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "sp")], 0x12);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "fp")], 0xef);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "ip")], 0x77);
}

/**
 * Execute the store operations (32 bits).
 * Operations: store, storerr, storeri
 */
BOOST_AUTO_TEST_CASE(cpu_store_word)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Load into the registers the data to be stored in the memory
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loadi g1 0x0");
  source.insert(line++, "loadi g2 0x4");
  source.insert(line++, "loadi g3 0x8");
  source.insert(line++, "loadi r0 0xc");

  source.insert(line++, "loadi r1 0x1010");
  source.insert(line++, "loadhi r1 0x1010");
  source.insert(line++, "loadi r2 0x0f0f");
  source.insert(line++, "loadhi r2 0x0f0f");
  source.insert(line++, "loadi r3 0x7777");
  source.insert(line++, "loadhi r3 0x7777");
  source.insert(line++, "loadi r4 0xffff");
  source.insert(line++, "loadhi r4 0xffff");

  // Store data with a pointer
  source.insert(line++, "store r1 data");

  // Store data with a offset (register)
  source.insert(line++, "storerr g0 r2 g2");

  // Store data with a offset (inmediate)
  source.insert(line++, "storeri g0 r3 0x8");
  source.insert(line++, "storeri g0 r4 0xc");
  // The code ends here

  // Space to store 4 words of data
  sw::Uint8 data = line;
  source.insert(line++, ".label data");
  source.insert(line++, "0x08080808");
  source.insert(line++, "0x08080808");
  source.insert(line++, "0x08080808");
  source.insert(line++, "0x08080808");

  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);
  cpu.execute(data);

  BOOST_CHECK_EQUAL(memory[ADDRESS(data)], 0x10101010);
  BOOST_CHECK_EQUAL(memory[ADDRESS(data) + 0x4], 0x0f0f0f0f);
  BOOST_CHECK_EQUAL(memory[ADDRESS(data) + 0x8], 0x77777777);
  BOOST_CHECK_EQUAL(memory[ADDRESS(data) + 0xc], 0xffffffff);
}

/**
 * Execute the store operations (16 bits).
 * Operations: storeh, storehrr, storehri
 */
BOOST_AUTO_TEST_CASE(cpu_store_halfword)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Load into the registers the data to be stored in the memory
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loadi g1 0x0");
  source.insert(line++, "loadi g2 0x2");
  source.insert(line++, "loadi g3 0x4");
  source.insert(line++, "loadi r0 0x6");

  source.insert(line++, "loadi r1 0x1010");
  source.insert(line++, "loadi r2 0x0f0f");
  source.insert(line++, "loadi r3 0x7777");
  source.insert(line++, "loadi r4 0xffff");

  // Store data with a pointer
  source.insert(line++, "storeh r1 data");

  // Store data with a offset (register)
  source.insert(line++, "storehrr g0 r2 g2");

  // Store data with a offset (inmediate)
  source.insert(line++, "storehri g0 r3 0x4");
  source.insert(line++, "storehri g0 r4 0x6");
  // The code ends here

  // Space to store 4 words of data
  sw::Uint8 data = line;
  source.insert(line++, ".label data");
  source.insert(line++, "0x08080808");
  source.insert(line++, "0x08080808");

  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);
  cpu.execute(data);

  BOOST_CHECK_EQUAL(memory[ADDRESS(data)], 0x10100f0f);
  BOOST_CHECK_EQUAL(memory[ADDRESS(data) + 0x4], 0x7777ffff);
}

/**
 * Execute the store operations (8 bits).
 * Operations: storeq, storeqrr, storeqri
 */
BOOST_AUTO_TEST_CASE(cpu_store_quarterword)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Load into the registers the data to be stored in the memory
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loadi g1 0x0");
  source.insert(line++, "loadi g2 0x1");
  source.insert(line++, "loadi g3 0x2");
  source.insert(line++, "loadi r0 0x3");

  source.insert(line++, "loadi r1 0x10");
  source.insert(line++, "loadi r2 0x0f");
  source.insert(line++, "loadi r3 0x77");
  source.insert(line++, "loadi r4 0xff");

  // Store data with a pointer
  source.insert(line++, "storeq r1 data");

  // Store data with a offset (register)
  source.insert(line++, "storeqrr g0 r2 g2");

  // Store data with a offset (inmediate)
  source.insert(line++, "storeqri g0 r3 0x2");
  source.insert(line++, "storeqri g0 r4 0x3");
  // The code ends here

  // Space to store 4 words of data
  sw::Uint8 data = line;
  source.insert(line++, ".label data");
  source.insert(line++, "0x08080808");

  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);
  cpu.execute(data);

  BOOST_CHECK_EQUAL(memory[ADDRESS(data)], 0x100f77ff);
}

/**
 * Execute the move operations.
 * Operations: move, swap
 */
BOOST_AUTO_TEST_CASE(cpu_move)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Load into the registers the data to be moved
  source.insert(line++, "loadi g0 0x0000");
  source.insert(line++, "loadi g1 0x0001");
  source.insert(line++, "loadi g2 0xffff");
  source.insert(line++, "loadi g3 0x7705");

  source.insert(line++, "move r0 g0");
  source.insert(line++, "move r1 g1");
  source.insert(line++, "move r2 g2");
  source.insert(line++, "move r3 g3");

  source.insert(line++, "swap r4 g0");
  source.insert(line++, "swap r5 g1");
  source.insert(line++, "swap sp g2");
  source.insert(line++, "swap fp g3");

  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);
  cpu.execute(line);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0x0000);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r1")], 0x0001);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r2")], 0xffff);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r3")], 0x7705);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r4")], 0x00000000);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r5")], 0x00010000);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "sp")], 0xffff0000);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "fp")], 0x77050000);
}

/**
 * Execute the stack operations.
 * Operations: push, pop
 */
BOOST_AUTO_TEST_CASE(cpu_stack)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, "loada sp stack");
  source.insert(line++, "loadi g0 0xabcd");
  source.insert(line++, "loadi g1 0x0000");

  // Push the register g0, change it, and then pop it.
  source.insert(line++, "push g0");
  source.insert(line++, "move g0 g1");
  source.insert(line++, "pop g0");

  // Space for 1 word in the stack
  sw::Uint8 stack = line;
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x4");

  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);

  cpu.execute(stack);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0xabcd);
}

/**
 * Execute the branch operations.
 * Operations: b, bz, bnz, beq, bne, blt, bltu, bgt, bgtu, ble, bleu, bge, begu
 */
BOOST_AUTO_TEST_CASE(cpu_branch)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Load into the registers the data to be stored in the memory
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loadi g1 0x1");

  // Load into the registers the data to be compared
  source.insert(line++, "loadi g2 0x0");
  source.insert(line++, "loadi g3 0x10");
  source.insert(line++, "loadi r0 0x10");
  source.insert(line++, "loadi r1 0xffff");
  source.insert(line++, "slli r1 r1 0x10"); // r1 is negative

  // Always
  source.insert(line++, "b branch_always");
  source.insert(line++, "stop");

  // If g3 == r0
  source.insert(line++, ".label branch_always");
  source.insert(line++, "storeri g0 g1 0x0");
  source.insert(line++, "beq g3 r0 branch_zero");
  source.insert(line++, "stop");

  // If g2 == 0
  source.insert(line++, ".label branch_zero");
  source.insert(line++, "storeri g0 g1 0x4");
  source.insert(line++, "bz g2 branch_nonzero");
  source.insert(line++, "stop");

  // If g3 != 0
  source.insert(line++, ".label branch_nonzero");
  source.insert(line++, "storeri g0 g1 0x8");
  source.insert(line++, "bnz g3 branch_equal");
  source.insert(line++, "stop");

  // If g2 != g3
  source.insert(line++, ".label branch_equal");
  source.insert(line++, "storeri g0 g1 0xc");
  source.insert(line++, "bne g2 g3 branch_notequal");
  source.insert(line++, "stop");

  // If r1 < r0
  source.insert(line++, ".label branch_notequal");
  source.insert(line++, "storeri g0 g1 0x10");
  source.insert(line++, "blt r1 r0 branch_less");
  source.insert(line++, "stop");

  // If r0 < r1
  source.insert(line++, ".label branch_less");
  source.insert(line++, "storeri g0 g1 0x14");
  source.insert(line++, "bltu r0 r1 branch_lessu");
  source.insert(line++, "stop");

  // If g3 > r1
  source.insert(line++, ".label branch_lessu");
  source.insert(line++, "storeri g0 g1 0x18");
  source.insert(line++, "bgt g3 r1 branch_greater");
  source.insert(line++, "stop");

  // If r1 > g3
  source.insert(line++, ".label branch_greater");
  source.insert(line++, "storeri g0 g1 0x1c");
  source.insert(line++, "bgtu r1 g3 branch_greateru");
  source.insert(line++, "stop");

  // If g2 <= g3
  source.insert(line++, ".label branch_greateru");
  source.insert(line++, "storeri g0 g1 0x20");
  source.insert(line++, "ble g2 g3 branch_lessequal");
  source.insert(line++, "stop");

  // If g2 <= g3
  source.insert(line++, ".label branch_lessequal");
  source.insert(line++, "storeri g0 g1 0x24");
  source.insert(line++, "bleu g2 g3 branch_lessequalu");
  source.insert(line++, "stop");

  // If g3 >= r0
  source.insert(line++, ".label branch_lessequalu");
  source.insert(line++, "storeri g0 g1 0x28");
  source.insert(line++, "bge g3 r0 branch_greaterequal");
  source.insert(line++, "stop");

  // If r1 >= r0
  source.insert(line++, ".label branch_greaterequal");
  source.insert(line++, "storeri g0 g1 0x2c");
  source.insert(line++, "bgeu r1 r0 branch_greaterequalu");
  source.insert(line++, "stop");

  source.insert(line++, ".label branch_greaterequalu");
  source.insert(line++, "storeri g0 g1 0x30");

  // End of test
  source.insert(line++, "stop");

  // Space to store 11 words of data
  sw::Uint8 data = line - 13;
  source.insert(line++, ".label data");
  source.insert(line++, ".block 0x34");

  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);

  cpu.execute();

  BOOST_CHECK_EQUAL(memory[ADDRESS(data)], 0x1);
  BOOST_CHECK_EQUAL(memory[ADDRESS(data) + 0x4], 0x1);
  BOOST_CHECK_EQUAL(memory[ADDRESS(data) + 0x8], 0x1);
  BOOST_CHECK_EQUAL(memory[ADDRESS(data) + 0xc], 0x1);
  BOOST_CHECK_EQUAL(memory[ADDRESS(data) + 0x10], 0x1);
  BOOST_CHECK_EQUAL(memory[ADDRESS(data) + 0x14], 0x1);
  BOOST_CHECK_EQUAL(memory[ADDRESS(data) + 0x18], 0x1);
  BOOST_CHECK_EQUAL(memory[ADDRESS(data) + 0x1c], 0x1);
  BOOST_CHECK_EQUAL(memory[ADDRESS(data) + 0x20], 0x1);
  BOOST_CHECK_EQUAL(memory[ADDRESS(data) + 0x24], 0x1);
  BOOST_CHECK_EQUAL(memory[ADDRESS(data) + 0x28], 0x1);
  BOOST_CHECK_EQUAL(memory[ADDRESS(data) + 0x2c], 0x1);
  BOOST_CHECK_EQUAL(memory[ADDRESS(data) + 0x30], 0x1);
}

/**
 * Execute the function operations call, callr and ret.
 */
BOOST_AUTO_TEST_CASE(cpu_function)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, "loada sp stack");

  // Execute some functions
  source.insert(line++, "call func0");
  source.insert(line++, "call func1");

  source.insert(line++, "loada r0 func2");
  source.insert(line++, "callr r0");
  source.insert(line++, "loada r0 func3");
  source.insert(line++, "callr r0");

  // End of test
  source.insert(line++, "stop");

  // Functions
  source.insert(line++, ".label func0");
  source.insert(line++, "loadi g0 0xf");
  source.insert(line++, "ret");

  source.insert(line++, ".label func1");
  source.insert(line++, "loadi g1 0xf0");
  source.insert(line++, "ret");

  source.insert(line++, ".label func2");
  source.insert(line++, "loadi g2 0xf00");
  source.insert(line++, "ret");

  source.insert(line++, ".label func3");
  source.insert(line++, "loadi g3 0xf000");
  source.insert(line++, "ret");

  // Space for a word in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x8");

  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);

  cpu.execute();

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0xf);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g1")], 0xf0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g2")], 0xf00);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g3")], 0xf000);
}

/**
 * Execute the function operations int and reti.
 */
BOOST_AUTO_TEST_CASE(cpu_interrupt)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, "loada sp stack");

  // Initialize the control & status register
  source.insert(line++, "loada ip interrupts_table");

  // Raise some interrupts
  source.insert(line++, "int 0x1111");     // Software interrupt
  source.insert(line++, "0xffffffff");     // Invalid instruction
  source.insert(line++, "load g0 0xfff");  // Invalid memory location
  source.insert(line++, "divi g0 g0 0x0"); // Division by zero

  // Disable the sotware interrupt
  source.insert(line++, "loadi g0 0x0");
  source.insert(line++, "loada g1 interrupts_table");
  source.insert(line++, "storeri g1 g0 0x4");

  source.insert(line++, "int 0x7777");

  // End of test
  source.insert(line++, "stop");

  // Interrupt handler
  // Store the code of the interrupt in a the position data + 4 * code
  source.insert(line++, ".label handler_int");
  source.insert(line++, "multi g1 g0 0x4");
  source.insert(line++, "loada g2 data");
  source.insert(line++, "storerr g1 g0 g2");
  source.insert(line++, "reti");

  // Software interrupt handler
  // Store the data of the software interrupt in data + 4
  source.insert(line++, ".label handler_soft");
  source.insert(line++, "loada g0 data");
  source.insert(line++, "storeri g0 g1 0x4");
  source.insert(line++, "reti");

  // Space to store 5 words of data
  sw::Uint8 data = line - 2;
  source.insert(line++, ".label data");
  source.insert(line++, ".block 0x14");

  // Space for the 16 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  // Interrupts table
  source.insert(line++, ".label interrupts_table");
  source.insert(line++, "handler_int");   // Timer interrupt
  source.insert(line++, "handler_soft");  // Software interrupt
  source.insert(line++, "handler_int");   // Invalid instruction
  source.insert(line++, "handler_int");   // Invalid memory location
  source.insert(line++, "handler_int");   // Division by zero

  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);

  cpu.execute();

  BOOST_CHECK_EQUAL(memory[ADDRESS(data)], 0x0);
  BOOST_CHECK_EQUAL(memory[ADDRESS(data) + 0x4], 0x1111);
  BOOST_CHECK_EQUAL(memory[ADDRESS(data) + 0x8], 0x2);
  BOOST_CHECK_EQUAL(memory[ADDRESS(data) + 0xc], 0x3);
  BOOST_CHECK_EQUAL(memory[ADDRESS(data) + 0x10], 0x4);
}

/**
 * Check if the frame pointer works.
 */
BOOST_AUTO_TEST_CASE(cpu_frame_pointer)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, "loada sp stack");

  // Initialize the interrupt table pointer register
  source.insert(line++, "loada ip interrupts_table");

  // Test the frame pointer
  source.insert(line++, "call function");
  source.insert(line++, "int 0x0");

  // End of test
  source.insert(line++, "stop");

  // Function that allocates space for 64 words in the stack
  source.insert(line++, ".label function");
  source.insert(line++, "addi sp sp 0x100");
  // Store 0x1 in data
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loadi g1 0x1");
  source.insert(line++, "storeri g0 g1 0x0");
  source.insert(line++, "ret");

  // Interrupt that allocates space for 64 words in the stack
  source.insert(line++, ".label handler_int");
  source.insert(line++, "addi sp sp 0x100");
  // Store 0x1 in data + 4
  source.insert(line++, "loada g0 data");
  source.insert(line++, "loadi g1 0x2");
  source.insert(line++, "storeri g0 g1 0x4");
  source.insert(line++, "reti");

  // Space to store 2 words of data
  sw::Uint8 data = line - 2;
  source.insert(line++, ".label data");
  source.insert(line++, ".block 0x8");

  // Space for 16 word in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x40");

  // Interrupts table
  source.insert(line++, ".label interrupts_table");
  source.insert(line++, "0x0");         // Timer interrupt
  source.insert(line++, "handler_int"); // Software interrupt
  source.insert(line++, "0x0");         // Invalid instruction
  source.insert(line++, "0x0");         // Invalid memory location
  source.insert(line++, "0x0");         // Division by zero

  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);

  cpu.execute();

  BOOST_CHECK_EQUAL(memory[ADDRESS(data)], 0x1);
  BOOST_CHECK_EQUAL(memory[ADDRESS(data) + 0x4], 0x2);
}

/**
 * Check if the stack is increased when push, call is executed or a interrupt
 * is handled.
 */
BOOST_AUTO_TEST_CASE(cpu_stack_increases)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, "loada sp stack");

  // Initialize the interrupt table pointer
  source.insert(line++, "loada ip interrupts_table");

  // Increase the stack
  source.insert(line++, "push g0");
  source.insert(line++, "call function");

  source.insert(line++, ".label function");
  source.insert(line++, "int 0x0");

  source.insert(line++, ".label handler_int");
  source.insert(line++, "stop");

  // Space for 18 elements in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x48");

  // Interrupts table
  source.insert(line++, ".label interrupts_table");
  source.insert(line++, "0x0");         // Timer interrupt
  source.insert(line++, "handler_int"); // Software interrupt
  source.insert(line++, "0x0");         // Invalid instruction
  source.insert(line++, "0x0");         // Invalid memory location
  source.insert(line++, "0x0");         // Division by zero

  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);

  sw::Uint32 sp = 0;
  while (cpu.running()) {
    cpu.execute(1);

    sw::Uint32 new_sp = registers[REGISTER(cpu, "sp")];
    BOOST_CHECK(new_sp >= sp);

    sp = new_sp;
  }
}

/**
 * Check if the stack is decreased when pop, ret or reti are executed.
 */
BOOST_AUTO_TEST_CASE(cpu_stack_decreases)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, "loada sp stack");

  // Initialize the interrupt table pointer
  source.insert(line++, "loada ip interrupts_table");

  // Use the stack
  source.insert(line++, "push g0");
  source.insert(line++, "call function");
  source.insert(line++, "pop g0");

  // End of test
  source.insert(line++, "stop");

  source.insert(line++, ".label function");
  source.insert(line++, "int 0x0");
  source.insert(line++, "ret");

  source.insert(line++, ".label handler_int");
  source.insert(line++, "reti");

  // Space for 18 elements in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x48");

  // Interrupts table
  source.insert(line++, ".label interrupts_table");
  source.insert(line++, "0x0");         // Timer interrupt
  source.insert(line++, "handler_int"); // Software interrupt
  source.insert(line++, "0x0");         // Invalid instruction
  source.insert(line++, "0x0");         // Invalid memory location
  source.insert(line++, "0x0");         // Division by zero

  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);

  cpu.execute(1);
  sw::Uint32 sp = registers[REGISTER(cpu, "sp")];
  cpu.execute();
  BOOST_CHECK_EQUAL(sp, registers[REGISTER(cpu, "sp")]);
}

/**
 * Execute the arithmetic operations add, addi, sub and subi.
 */
BOOST_AUTO_TEST_CASE(cpu_arithmetic_add)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  source.insert(line++, "loadi g0 0x0");
  source.insert(line++, "loadi g1 0x1");
  source.insert(line++, "loadi g2 0xffff");
  source.insert(line++, "loadi g3 0xffff");
  source.insert(line++, "slli g3 g3 0x10");
  source.insert(line++, "or g3 g3 g2");

  source.insert(line++, "add r0 g3 g1");
  source.insert(line++, "add r1 g2 g1");
  source.insert(line++, "addi r2 g2 0xffff");
  source.insert(line++, "addi r3 g0 0x0");
  source.insert(line++, "sub r4 g2 g2");
  source.insert(line++, "sub r5 g0 g1");
  source.insert(line++, "subi lr g2 0xf0f0");
  source.insert(line++, "subi fp g0 0x1");
  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);

  cpu.execute(line);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r1")], 0x10000);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r2")], 0x1fffe);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r3")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r4")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r5")], 0xffffffff);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "lr")], 0xf0f);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "fp")], 0xffffffff);
}

/**
 * Execute the arithmetic operations mult, multi, multh, multhi,
 * multhu and multhui.
 */
BOOST_AUTO_TEST_CASE(cpu_arithmetic_mult)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  source.insert(line++, "loadi g0 0x0");
  source.insert(line++, "loadi g1 0x10");
  source.insert(line++, "loadi g2 0xffff");
  source.insert(line++, "loadi g3 0xffff");
  source.insert(line++, "slli g3 g3 0x10");
  source.insert(line++, "or g3 g3 g2");

  source.insert(line++, "mult r0 g2 g3");
  source.insert(line++, "multi r1 g3 0x10");
  source.insert(line++, "mult r2 g3 g0");
  source.insert(line++, "multi r3 g2 0xfff");
  source.insert(line++, "multh r4 g3 g3");
  source.insert(line++, "multhi r5 g3 0x1");
  source.insert(line++, "multhu lr g3 g3");
  source.insert(line++, "multhui fp g3 0xf");
  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);

  cpu.execute(line);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0xffff0001);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r1")], 0xfffffff0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r2")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r3")], 0xffef001);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r4")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r5")], 0xffffffff);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "lr")], 0xfffffffe);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "fp")], 0xe);
}

/**
 * Execute the arithmetic operations div, divi, mod, and modi.
 */
BOOST_AUTO_TEST_CASE(cpu_arithmetic_div)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  source.insert(line++, "loadi g0 0x1");
  source.insert(line++, "loadi g1 0x10");
  source.insert(line++, "loadi g2 0xffff");
  source.insert(line++, "loadi g3 0xffff");
  source.insert(line++, "slli g3 g3 0x10");
  source.insert(line++, "or g3 g3 g2");

  source.insert(line++, "div r0 g3 g2");
  source.insert(line++, "div r1 g1 g0");
  source.insert(line++, "divi r2 g3 0x1");
  source.insert(line++, "divi r3 g2 0xffff");
  source.insert(line++, "mod r4 g2 g3");
  source.insert(line++, "mod r5 g2 g1");
  source.insert(line++, "modi lr g1 0x9");
  source.insert(line++, "modi fp g3 0xffff");
  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);

  cpu.execute(line);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0x10001);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r1")], 0x10);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r2")], 0xffffffff);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r3")], 0x1);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r4")], 0xffff);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r5")], 0xf);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "lr")], 0x7);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "fp")], 0x0);
}

/**
 * Execute the sign extension operations.
 * Operations: signh, signq
 */
BOOST_AUTO_TEST_CASE(cpu_sign_halfword)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  source.insert(line++, "loadi g0 0x0080");
  source.insert(line++, "loadi g1 0x007f");
  source.insert(line++, "loadi g2 0x8000");
  source.insert(line++, "loadhi g2 0xffff");
  source.insert(line++, "loadi g3 0x7f00");
  source.insert(line++, "loadhi g3 0xffff");

  source.insert(line++, "signh r0 g0");
  source.insert(line++, "signh r1 g1");
  source.insert(line++, "signh r2 g2");
  source.insert(line++, "signh r3 g3");

  source.insert(line++, "signq r4 g0");
  source.insert(line++, "signq r5 g1");
  source.insert(line++, "signq lr g2");
  source.insert(line++, "signq fp g3");

  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);
  cpu.execute(line);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0x80);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r1")], 0x7f);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r2")], 0xffff8000);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r3")], 0x7f00);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r4")], 0xffffff80);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r5")], 0x7f);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "lr")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "fp")], 0x0);
}

/**
 * Execute the logic operations.
 * Operations: not, or, ori, and, andi, xor, xori
 */
BOOST_AUTO_TEST_CASE(cpu_logic)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  source.insert(line++, "loadi g0 0x0000");
  source.insert(line++, "loadi g1 0x0001");
  source.insert(line++, "loadi g2 0xffff");
  source.insert(line++, "loadi g3 0x7705");

  source.insert(line++, "not r0 g0");
  source.insert(line++, "or r1 g1 g3");
  source.insert(line++, "ori r2 g2 0x1010");
  source.insert(line++, "and r3 g1 g3");
  source.insert(line++, "andi r4 g2 0x1010");
  source.insert(line++, "xor r5 g1 g3");
  source.insert(line++, "xori fp g2 0x1010");

  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);
  cpu.execute(line);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0xffffffff);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r1")], 0x7705);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r2")], 0xffff);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r3")], 0x0001);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r4")], 0x1010);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r5")], 0x7704);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "fp")], 0xefef);
}

/**
 * Execute shift operations.
 * Operations: sll, slli, srl, srli, sra, srai, rl, rli, rr, rri
 */
BOOST_AUTO_TEST_CASE(cpu_shift)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  source.insert(line++, "loadi g0 0x0000");
  source.insert(line++, "loadi g1 0x1010");
  source.insert(line++, "loadi g2 0x0707");
  source.insert(line++, "loadi g3 0xffff");

  source.insert(line++, "loadi r0 0x4");
  source.insert(line++, "loadi r1 0x8");
  source.insert(line++, "loadi r2 0x10");

  source.insert(line++, "sll r3 g3 r1");
  source.insert(line++, "slli r3 r3 0x8"); // r3 = 0xffff0000
  source.insert(line++, "srl r4 g3 g0");
  source.insert(line++, "srli r4 r4 0x2"); // r4 = 0x00003fff
  source.insert(line++, "or r3 r3 r4");    // r3 = 0xffff3fff

  source.insert(line++, "sll r4 g3 r1");
  source.insert(line++, "slli r4 r4 0x8"); // r4 = 0xffff0000
  source.insert(line++, "sra r4 r4 r1");   // r4 = 0x80ffff00
  source.insert(line++, "srai r4 r4 0x8"); // r4 = 0x8080ffff

  source.insert(line++, "rl r5 g3 r1");
  source.insert(line++, "rli r5 r5 0x8");    // r5 =  0xffff0000
  source.insert(line++, "rr lr g3 r2");
  source.insert(line++, "rri lr lr 0x10"); // lr = 0x0000ffff
  source.insert(line++, "or r5 r5 lr");     // r5 =  0xffffffff

  source.insert(line++, "slli lr g2 0x4");
  source.insert(line++, "srli lr lr 0x4"); // lr = 0x0707

  source.insert(line++, "rli fp g2 0x4");
  source.insert(line++, "rri fp fp 0x4"); // fp = 0x0707

  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);
  cpu.execute(line);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r3")], 0xffff3fff);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r4")], 0x8080ffff);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r5")], 0xffffffff);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "lr")], 0x0707);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "fp")], 0x0707);
}

/**
 * Check the register windows.
 */
BOOST_AUTO_TEST_CASE(cpu_windows)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, "loada sp stack");
  // Change registers g0 and r1
  source.insert(line++, "loadi g0 0x1010");
  source.insert(line++, "loadi r1 0x0f0f");
  source.insert(line++, "call func");
  source.insert(line++, "stop");

  // Change registers g0 and r1
  source.insert(line++, ".label func");
  source.insert(line++, "loadi g0 0x10AB");
  source.insert(line++, "loadi r1 0xBA01");
  source.insert(line++, "ret");

  // Space for 2 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x8");

  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);
  cpu.execute();

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0x10AB);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r1")], 0x0f0f);
}
