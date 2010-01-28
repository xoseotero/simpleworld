/**
 * @file tests/cpu/cpu_test.cpp
 * Unit test for CPU::CPU.
 *
 *  Copyright (C) 2007-2010  Xosé Otero <xoseotero@users.sourceforge.net>
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

  std::vector<std::string> include_path;
  cpu::Source source(cpu.isa(), include_path);

  source.insert(0, file);
  source.compile(CPU_SAVE);
}


/**
 * Check if the registers are initialized to 0.
 */
BOOST_AUTO_TEST_CASE(cpu_initialization)
{
  cpu::Memory registers;
  cpu::CPU cpu(&registers, NULL);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r1")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r2")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r3")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r4")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r5")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r6")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r7")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r8")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r9")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r10")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r11")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "pc")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "sp")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "fp")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "cs")], 0x0);
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
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r1")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r2")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r3")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r4")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r5")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r6")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r7")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r8")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r9")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r10")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r11")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "pc")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "sp")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "fp")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "cs")], 0x0);
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
  source.insert(line++, "loada r0 data");

  // Load inmediate values
  source.insert(line++, "loadi r1 0x0");
  source.insert(line++, "loadi r2 0x4");
  source.insert(line++, "loadi r3 0x8");
  source.insert(line++, "loadi r4 0xc");

  // Load inmediate values in the higher 16 bits
  source.insert(line++, "loadhi r1 0xc");
  source.insert(line++, "loadhi r2 0x8");
  source.insert(line++, "loadhi r3 0x4");
  source.insert(line++, "loadhi r4 0x0");

  // Data
  sw::Uint8 data = line;
  source.insert(line++, ".label data");
  source.insert(line++, "0x1234abcd");

  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);
  cpu.execute(data);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")],
                    memory.size() - sizeof(cpu::Word));
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r1")], 0xc0000);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r2")], 0x80004);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r3")], 0x40008);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r4")], 0xc);
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
  source.insert(line++, "loada r0 data");
  source.insert(line++, "loadi r1 0x0");
  source.insert(line++, "loadi r2 0x4");
  source.insert(line++, "loadi r3 0x8");
  source.insert(line++, "loadi r4 0xc");

  // Load data with a pointer
  source.insert(line++, "load r5 data");

  // Load data with a offset (register)
  source.insert(line++, "loadrr r6 r0 r1");
  source.insert(line++, "loadrr r7 r0 r2");
  source.insert(line++, "loadrr r8 r0 r3");
  source.insert(line++, "loadrr r9 r0 r4");

  // Load data with a offset (inmediate)
  source.insert(line++, "loadri r10 r0 0x0");
  source.insert(line++, "loadri r11 r0 0x4");
  source.insert(line++, "loadri cs r0 0x8");

  source.insert(line++, "loadi r0 0x0");
  source.insert(line++, "loadi r1 0x10");
  source.insert(line++, "loadhi r2 0xffff");
  source.insert(line++, "loadhi r3 0x7f07");

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

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0x0000);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r1")], 0x0010);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r2")], 0xffff0004);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r3")], 0x7f070008);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r4")], 0x000c);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r5")], 0x1234abcd);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r6")], 0x1234abcd);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r7")], 0xef567890);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r8")], 0x77777757);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r9")], 0x01010101);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r10")], 0x1234abcd);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r11")], 0xef567890);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "cs")], 0x77777757);
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
  source.insert(line++, "loada r0 data");
  source.insert(line++, "loadi r1 0x0");
  source.insert(line++, "loadi r2 0x2");
  source.insert(line++, "loadi r3 0x4");
  source.insert(line++, "loadi r4 0x6");

  // Load data with a pointer
  source.insert(line++, "loadh r5 data");

  // Load data with a offset (register)
  source.insert(line++, "loadhrr r6 r0 r1");
  source.insert(line++, "loadhrr r7 r0 r2");
  source.insert(line++, "loadhrr r8 r0 r3");
  source.insert(line++, "loadhrr r9 r0 r4");

  // Load data with a offset (inmediate)
  source.insert(line++, "loadhri r10 r0 0x0");
  source.insert(line++, "loadhri r11 r0 0x4");
  source.insert(line++, "loadhri cs r0 0x8");

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

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r5")], 0x1234);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r6")], 0x1234);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r7")], 0xabcd);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r8")], 0xef56);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r9")], 0x7890);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r10")], 0x1234);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r11")], 0xef56);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "cs")], 0x7777);
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
  source.insert(line++, "loada r0 data");
  source.insert(line++, "loadi r1 0x0");
  source.insert(line++, "loadi r2 0x1");
  source.insert(line++, "loadi r3 0x2");
  source.insert(line++, "loadi r4 0x3");

  // Load data with a pointer
  source.insert(line++, "loadq r5 data");

  // Load data with a offset (register)
  source.insert(line++, "loadqrr r6 r0 r1");
  source.insert(line++, "loadqrr r7 r0 r2");
  source.insert(line++, "loadqrr r8 r0 r3");
  source.insert(line++, "loadqrr r9 r0 r4");

  // Load data with a offset (inmediate)
  source.insert(line++, "loadqri r10 r0 0x0");
  source.insert(line++, "loadqri r11 r0 0x4");
  source.insert(line++, "loadqri cs r0 0x8");

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

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r5")], 0x12);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r6")], 0x12);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r7")], 0x34);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r8")], 0xab);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r9")], 0xcd);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r10")], 0x12);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r11")], 0xef);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "cs")], 0x77);
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
  source.insert(line++, "loada r0 data");
  source.insert(line++, "loadi r1 0x0");
  source.insert(line++, "loadi r2 0x4");
  source.insert(line++, "loadi r3 0x8");
  source.insert(line++, "loadi r4 0xc");

  source.insert(line++, "loadi r5 0x1010");
  source.insert(line++, "loadhi r5 0x1010");
  source.insert(line++, "loadi r6 0x0f0f");
  source.insert(line++, "loadhi r6 0x0f0f");
  source.insert(line++, "loadi r7 0x7777");
  source.insert(line++, "loadhi r7 0x7777");
  source.insert(line++, "loadi r8 0xffff");
  source.insert(line++, "loadhi r8 0xffff");

  // Store data with a pointer
  source.insert(line++, "store r5 data");

  // Store data with a offset (register)
  source.insert(line++, "storerr r0 r6 r2");

  // Store data with a offset (inmediate)
  source.insert(line++, "storeri r0 r7 0x8");
  source.insert(line++, "storeri r0 r8 0xc");
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
  source.insert(line++, "loada r0 data");
  source.insert(line++, "loadi r1 0x0");
  source.insert(line++, "loadi r2 0x2");
  source.insert(line++, "loadi r3 0x4");
  source.insert(line++, "loadi r4 0x6");

  source.insert(line++, "loadi r5 0x1010");
  source.insert(line++, "loadi r6 0x0f0f");
  source.insert(line++, "loadi r7 0x7777");
  source.insert(line++, "loadi r8 0xffff");

  // Store data with a pointer
  source.insert(line++, "storeh r5 data");

  // Store data with a offset (register)
  source.insert(line++, "storehrr r0 r6 r2");

  // Store data with a offset (inmediate)
  source.insert(line++, "storehri r0 r7 0x4");
  source.insert(line++, "storehri r0 r8 0x6");
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
  source.insert(line++, "loada r0 data");
  source.insert(line++, "loadi r1 0x0");
  source.insert(line++, "loadi r2 0x1");
  source.insert(line++, "loadi r3 0x2");
  source.insert(line++, "loadi r4 0x3");

  source.insert(line++, "loadi r5 0x10");
  source.insert(line++, "loadi r6 0x0f");
  source.insert(line++, "loadi r7 0x77");
  source.insert(line++, "loadi r8 0xff");

  // Store data with a pointer
  source.insert(line++, "storeq r5 data");

  // Store data with a offset (register)
  source.insert(line++, "storeqrr r0 r6 r2");

  // Store data with a offset (inmediate)
  source.insert(line++, "storeqri r0 r7 0x2");
  source.insert(line++, "storeqri r0 r8 0x3");
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
  source.insert(line++, "loadi r0 0x0000");
  source.insert(line++, "loadi r1 0x0001");
  source.insert(line++, "loadi r2 0xffff");
  source.insert(line++, "loadi r3 0x7705");

  source.insert(line++, "move r4 r0");
  source.insert(line++, "move r5 r1");
  source.insert(line++, "move r6 r2");
  source.insert(line++, "move r7 r3");

  source.insert(line++, "swap r8 r0");
  source.insert(line++, "swap r9 r1");
  source.insert(line++, "swap r10 r2");
  source.insert(line++, "swap r11 r3");

  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);
  cpu.execute(line);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r4")], 0x0000);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r5")], 0x0001);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r6")], 0xffff);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r7")], 0x7705);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r8")], 0x00000000);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r9")], 0x00010000);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r10")], 0xffff0000);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r11")], 0x77050000);
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
  source.insert(line++, "loadi r0 0xabcd");
  source.insert(line++, "loadi r1 0x0000");

  // Push the register r0, change it, and then pop it.
  source.insert(line++, "push r0");
  source.insert(line++, "move r0 r1");
  source.insert(line++, "pop r0");

  // Space for 1 word in the stack
  sw::Uint8 stack = line;
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x4");

  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);

  cpu.execute(stack);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0xabcd);
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
  source.insert(line++, "loada r0 data");
  source.insert(line++, "loadi r1 0x1");

  // Load into the registers the data to be compared
  source.insert(line++, "loadi r2 0x0");
  source.insert(line++, "loadi r3 0x10");
  source.insert(line++, "loadi r4 0x10");
  source.insert(line++, "loadi r5 0xffff");
  source.insert(line++, "slli r5 r5 0x10"); // r5 is negative

  // Always
  source.insert(line++, "b branch_always");
  source.insert(line++, "stop");

  // If r3 == r4
  source.insert(line++, ".label branch_always");
  source.insert(line++, "storeri r0 r1 0x0");
  source.insert(line++, "beq r3 r4 branch_zero");
  source.insert(line++, "stop");

  // If r2 == 0
  source.insert(line++, ".label branch_zero");
  source.insert(line++, "storeri r0 r1 0x4");
  source.insert(line++, "bz r2 branch_nonzero");
  source.insert(line++, "stop");

  // If r3 != 0
  source.insert(line++, ".label branch_nonzero");
  source.insert(line++, "storeri r0 r1 0x8");
  source.insert(line++, "bnz r3 branch_equal");
  source.insert(line++, "stop");

  // If r2 != r3
  source.insert(line++, ".label branch_equal");
  source.insert(line++, "storeri r0 r1 0xc");
  source.insert(line++, "bne r2 r3 branch_notequal");
  source.insert(line++, "stop");

  // If r5 < r4
  source.insert(line++, ".label branch_notequal");
  source.insert(line++, "storeri r0 r1 0x10");
  source.insert(line++, "blt r5 r4 branch_less");
  source.insert(line++, "stop");

  // If r4 < r5
  source.insert(line++, ".label branch_less");
  source.insert(line++, "storeri r0 r1 0x14");
  source.insert(line++, "bltu r4 r5 branch_lessu");
  source.insert(line++, "stop");

  // If r3 > r5
  source.insert(line++, ".label branch_lessu");
  source.insert(line++, "storeri r0 r1 0x18");
  source.insert(line++, "bgt r3 r5 branch_greater");
  source.insert(line++, "stop");

  // If r5 > r3
  source.insert(line++, ".label branch_greater");
  source.insert(line++, "storeri r0 r1 0x1c");
  source.insert(line++, "bgtu r5 r3 branch_greateru");
  source.insert(line++, "stop");

  // If r2 <= r3
  source.insert(line++, ".label branch_greateru");
  source.insert(line++, "storeri r0 r1 0x20");
  source.insert(line++, "ble r2 r3 branch_lessequal");
  source.insert(line++, "stop");

  // If r2 <= r3
  source.insert(line++, ".label branch_lessequal");
  source.insert(line++, "storeri r0 r1 0x24");
  source.insert(line++, "bleu r2 r3 branch_lessequalu");
  source.insert(line++, "stop");

  // If r3 >= r4
  source.insert(line++, ".label branch_lessequalu");
  source.insert(line++, "storeri r0 r1 0x28");
  source.insert(line++, "bge r3 r4 branch_greaterequal");
  source.insert(line++, "stop");

  // If r5 >= r4
  source.insert(line++, ".label branch_greaterequal");
  source.insert(line++, "storeri r0 r1 0x2c");
  source.insert(line++, "bgeu r5 r4 branch_greaterequalu");
  source.insert(line++, "stop");

  source.insert(line++, ".label branch_greaterequalu");
  source.insert(line++, "storeri r0 r1 0x30");

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

  source.insert(line++, "loada r11 func2");
  source.insert(line++, "callr r11");
  source.insert(line++, "loada r11 func3");
  source.insert(line++, "callr r11");

  // End of test
  source.insert(line++, "stop");

  // Functions
  source.insert(line++, ".label func0");
  source.insert(line++, "loadi r0 0xf");
  source.insert(line++, "ret");

  source.insert(line++, ".label func1");
  source.insert(line++, "loadi r1 0xf0");
  source.insert(line++, "ret");

  source.insert(line++, ".label func2");
  source.insert(line++, "loadi r2 0xf00");
  source.insert(line++, "ret");

  source.insert(line++, ".label func3");
  source.insert(line++, "loadi r3 0xf000");
  source.insert(line++, "ret");

  // Space for a word in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x8");

  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);

  cpu.execute();

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0xf);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r1")], 0xf0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r2")], 0xf00);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r3")], 0xf000);
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
  source.insert(line++, "loada r0 interrupts_table");
  source.insert(line++, "loadi r1 0x1");
  source.insert(line++, "loadi r2 0x1");
  source.insert(line++, "call cs_set");

  // Raise some interrupts
  source.insert(line++, "int 0x1111");     // Software interrupt
  source.insert(line++, "0xffffffff");     // Invalid instruction
  source.insert(line++, "load r0 0xfff");  // Invalid memory location
  source.insert(line++, "divi r0 r0 0x0"); // Division by zero

  // Disable the sotware interrupt
  source.insert(line++, "loadi r0 0x0");
  source.insert(line++, "loada r1 interrupts_table");
  source.insert(line++, "storeri r1 r0 0x4");

  source.insert(line++, "int 0x7777");

  // End of test
  source.insert(line++, "stop");

  // Set the values of the cs
  source.insert(line++, ".label cs_set");
  source.insert(line++, "slli cs r0 0x10");
  source.insert(line++, "loadi r3 0x0");
  source.insert(line++, "beq r1 r3 _interrupt_continue");
  source.insert(line++, "ori cs cs 0x80");
  source.insert(line++, ".label _interrupt_continue");
  source.insert(line++, "or cs cs r2");
  source.insert(line++, "ret");

  // Interrupt handler
  // Store the code of the interrupt in a the position data + 4 * code
  source.insert(line++, ".label handler_int");
  source.insert(line++, "multi r1 r0 0x4");
  source.insert(line++, "loada r2 data");
  source.insert(line++, "storerr r1 r0 r2");
  source.insert(line++, "reti");

  // Software interrupt handler
  // Store the data of the software interrupt in data + 4
  source.insert(line++, ".label handler_soft");
  source.insert(line++, "loada r0 data");
  source.insert(line++, "storeri r0 r1 0x4");
  source.insert(line++, "reti");

  // Space to store 5 words of data
  sw::Uint8 data = line - 4;
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

  // Initialize the control & status register
  source.insert(line++, "loada r0 interrupts_table");
  source.insert(line++, "loadi r1 0x1");
  source.insert(line++, "loadi r2 0x1");
  source.insert(line++, "call cs_set");

  // Test the frame pointer
  source.insert(line++, "call function");
  source.insert(line++, "int 0x0");

  // End of test
  source.insert(line++, "stop");

  // Set the values of the cs
  source.insert(line++, ".label cs_set");
  source.insert(line++, "slli cs r0 0x10");
  source.insert(line++, "loadi r3 0x0");
  source.insert(line++, "beq r1 r3 _interrupt_continue");
  source.insert(line++, "ori cs cs 0x80");
  source.insert(line++, ".label _interrupt_continue");
  source.insert(line++, "or cs cs r2");
  source.insert(line++, "ret");

  // Function that allocates space for 64 words in the stack
  source.insert(line++, ".label function");
  source.insert(line++, "addi sp sp 0x100");
  // Store 0x1 in data
  source.insert(line++, "loada r0 data");
  source.insert(line++, "loadi r1 0x1");
  source.insert(line++, "storeri r0 r1 0x0");
  source.insert(line++, "ret");

  // Interrupt that allocates space for 64 words in the stack
  source.insert(line++, ".label handler_int");
  source.insert(line++, "addi sp sp 0x100");
  // Store 0x1 in data + 4
  source.insert(line++, "loada r0 data");
  source.insert(line++, "loadi r1 0x2");
  source.insert(line++, "storeri r0 r1 0x4");
  source.insert(line++, "reti");

  // Space to store 2 words of data
  sw::Uint8 data = line - 4;
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
 * Check if the frame pointer works for passing data in the stack.
 */
BOOST_AUTO_TEST_CASE(cpu_frame_pointer2)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, "loada sp stack");

  // Test the frame pointer
  source.insert(line++, "loada r0 data");
  source.insert(line++, "push r0");
  source.insert(line++, "loadi r0 0x1");
  source.insert(line++, "push r0");
  source.insert(line++, "call store_data");
  source.insert(line++, "subi sp sp 0x8");

  source.insert(line++, "loada r0 data");
  source.insert(line++, "addi r0 r0 0x4");
  source.insert(line++, "push r0");
  source.insert(line++, "loadi r0 0x2");
  source.insert(line++, "push r0");
  source.insert(line++, "call store_data");

  // End of test
  source.insert(line++, "stop");

  // Function that receibes 2 parameters in the stack
  source.insert(line++, ".label store_data");
  // Get the parammeters from the stack
  source.insert(line++, "loadri r0 fp 0xFFF0");
  source.insert(line++, "loadri r1 fp 0xFFF4");
  // Store the second parameter in the address pointed by the first one
  source.insert(line++, "storeri r0 r1 0x0");
  source.insert(line++, "ret");

  // Space to store 2 words of data
  sw::Uint8 data = line - 1;
  source.insert(line++, ".label data");
  source.insert(line++, ".block 0x8");

  // Space for 4 word in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x10");

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

  // Initialize the control & status register
  source.insert(line++, "loada r0 interrupts_table");
  source.insert(line++, "slli cs r0 0x10");
  source.insert(line++, "loadi r0 0x0");
  source.insert(line++, "ori cs cs 0x80");
  source.insert(line++, "ori cs cs 0x1");

  // Increase the stack
  source.insert(line++, "push r0");
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

  // Initialize the control & status register
  source.insert(line++, "loada r0 interrupts_table");
  source.insert(line++, "loadi r1 0x1");
  source.insert(line++, "loadi r2 0x1");
  source.insert(line++, "call cs_set");

  // Use the stack
  source.insert(line++, "push r0");
  source.insert(line++, "call function");
  source.insert(line++, "pop r0");

  // End of test
  source.insert(line++, "stop");

  // Set the values of the cs
  source.insert(line++, ".label cs_set");
  source.insert(line++, "slli cs r0 0x10");
  source.insert(line++, "loadi r3 0x0");
  source.insert(line++, "beq r1 r3 _interrupt_continue");
  source.insert(line++, "ori cs cs 0x80");
  source.insert(line++, ".label _interrupt_continue");
  source.insert(line++, "or cs cs r2");
  source.insert(line++, "ret");

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

  source.insert(line++, "loadi r0 0x0");
  source.insert(line++, "loadi r1 0x1");
  source.insert(line++, "loadi r2 0xffff");
  source.insert(line++, "loadi r3 0xffff");
  source.insert(line++, "slli r3 r3 0x10");
  source.insert(line++, "or r3 r3 r2");

  source.insert(line++, "add r4 r3 r1");
  source.insert(line++, "add r5 r2 r1");
  source.insert(line++, "addi r6 r2 0xffff");
  source.insert(line++, "addi r7 r0 0x0");
  source.insert(line++, "sub r8 r2 r2");
  source.insert(line++, "sub r9 r0 r1");
  source.insert(line++, "subi r10 r2 0xf0f0");
  source.insert(line++, "subi r11 r0 0x1");
  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);

  cpu.execute(line);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r4")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r5")], 0x10000);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r6")], 0x1fffe);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r7")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r8")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r9")], 0xffffffff);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r10")], 0xf0f);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r11")], 0xffffffff);
}

/**
 * Execute the arithmetic operations mult, multi, multh, multhi,
 * multhu and multhui.
 */
BOOST_AUTO_TEST_CASE(cpu_arithmetic_mult)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  source.insert(line++, "loadi r0 0x0");
  source.insert(line++, "loadi r1 0x10");
  source.insert(line++, "loadi r2 0xffff");
  source.insert(line++, "loadi r3 0xffff");
  source.insert(line++, "slli r3 r3 0x10");
  source.insert(line++, "or r3 r3 r2");

  source.insert(line++, "mult r4 r2 r3");
  source.insert(line++, "multi r5 r3 0x10");
  source.insert(line++, "mult r6 r3 r0");
  source.insert(line++, "multi r7 r2 0xfff");
  source.insert(line++, "multh r8 r3 r3");
  source.insert(line++, "multhi r9 r3 0x1");
  source.insert(line++, "multhu r10 r3 r3");
  source.insert(line++, "multhui r11 r3 0xf");
  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);

  cpu.execute(line);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r4")], 0xffff0001);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r5")], 0xfffffff0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r6")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r7")], 0xffef001);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r8")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r9")], 0xffffffff);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r10")], 0xfffffffe);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r11")], 0xe);
}

/**
 * Execute the arithmetic operations div, divi, mod, and modi.
 */
BOOST_AUTO_TEST_CASE(cpu_arithmetic_div)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  source.insert(line++, "loadi r0 0x1");
  source.insert(line++, "loadi r1 0x10");
  source.insert(line++, "loadi r2 0xffff");
  source.insert(line++, "loadi r3 0xffff");
  source.insert(line++, "slli r3 r3 0x10");
  source.insert(line++, "or r3 r3 r2");

  source.insert(line++, "div r4 r3 r2");
  source.insert(line++, "div r5 r1 r0");
  source.insert(line++, "divi r6 r3 0x1");
  source.insert(line++, "divi r7 r2 0xffff");
  source.insert(line++, "mod r8 r2 r3");
  source.insert(line++, "mod r9 r2 r1");
  source.insert(line++, "modi r10 r1 0x9");
  source.insert(line++, "modi r11 r3 0xffff");
  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);

  cpu.execute(line);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r4")], 0x10001);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r5")], 0x10);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r6")], 0xffffffff);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r7")], 0x1);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r8")], 0xffff);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r9")], 0xf);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r10")], 0x7);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r11")], 0x0);
}

/**
 * Execute the sign extension operations.
 * Operations: signh, signq
 */
BOOST_AUTO_TEST_CASE(cpu_sign_halfword)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  source.insert(line++, "loadi r0 0x0080");
  source.insert(line++, "loadi r1 0x007f");
  source.insert(line++, "loadi r2 0x8000");
  source.insert(line++, "loadhi r2 0xffff");
  source.insert(line++, "loadi r3 0x7f00");
  source.insert(line++, "loadhi r3 0xffff");

  source.insert(line++, "signh r4 r0");
  source.insert(line++, "signh r5 r1");
  source.insert(line++, "signh r6 r2");
  source.insert(line++, "signh r7 r3");

  source.insert(line++, "signq r8 r0");
  source.insert(line++, "signq r9 r1");
  source.insert(line++, "signq r10 r2");
  source.insert(line++, "signq r11 r3");

  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);
  cpu.execute(line);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r4")], 0x80);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r5")], 0x7f);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r6")], 0xffff8000);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r7")], 0x7f00);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r8")], 0xffffff80);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r9")], 0x7f);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r10")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r11")], 0x0);
}

/**
 * Execute the logic operations.
 * Operations: not, or, ori, and, andi, xor, xori
 */
BOOST_AUTO_TEST_CASE(cpu_logic)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  source.insert(line++, "loadi r0 0x0000");
  source.insert(line++, "loadi r1 0x0001");
  source.insert(line++, "loadi r2 0xffff");
  source.insert(line++, "loadi r3 0x7705");

  source.insert(line++, "not r4 r0");
  source.insert(line++, "or r5 r1 r3");
  source.insert(line++, "ori r6 r2 0x1010");
  source.insert(line++, "and r7 r1 r3");
  source.insert(line++, "andi r8 r2 0x1010");
  source.insert(line++, "xor r9 r1 r3");
  source.insert(line++, "xori r10 r2 0x1010");

  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);
  cpu.execute(line);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r4")], 0xffffffff);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r5")], 0x7705);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r6")], 0xffff);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r7")], 0x0001);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r8")], 0x1010);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r9")], 0x7704);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r10")], 0xefef);
}

/**
 * Execute shift operations.
 * Operations: sll, slli, srl, srli, sra, srai, rl, rli, rr, rri
 */
BOOST_AUTO_TEST_CASE(cpu_shift)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  source.insert(line++, "loadi r0 0x0000");
  source.insert(line++, "loadi r1 0x1010");
  source.insert(line++, "loadi r2 0x0707");
  source.insert(line++, "loadi r3 0xffff");

  source.insert(line++, "loadi r4 0x4");
  source.insert(line++, "loadi r5 0x8");
  source.insert(line++, "loadi r6 0x10");

  source.insert(line++, "sll r7 r3 r5");
  source.insert(line++, "slli r7 r7 0x8"); // r7 = 0xffff0000
  source.insert(line++, "srl r8 r3 r0");
  source.insert(line++, "srli r8 r8 0x2"); // r8 = 0x00003fff
  source.insert(line++, "or r7 r7 r8");    // r7 = 0xffff3fff

  source.insert(line++, "sll r8 r3 r5");
  source.insert(line++, "slli r8 r8 0x8"); // r8 = 0xffff0000
  source.insert(line++, "sra r8 r8 r5");   // r8 = 0x80ffff00
  source.insert(line++, "srai r8 r8 0x8"); // r8 = 0x8080ffff

  source.insert(line++, "rl r9 r3 r5");
  source.insert(line++, "rli r9 r9 0x8");    // r9 =  0xffff0000
  source.insert(line++, "rr r10 r3 r6");
  source.insert(line++, "rri r10 r10 0x10"); // r10 = 0x0000ffff
  source.insert(line++, "or r9 r9 r10");     // r9 =  0xffffffff

  source.insert(line++, "slli r10 r2 0x4");
  source.insert(line++, "srli r10 r10 0x4"); // r10 = 0x0707

  source.insert(line++, "rli r11 r2 0x4");
  source.insert(line++, "rri r11 r11 0x4"); // r11 = 0x0707

  compile(source);


  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  cpu::CPU cpu(&registers, &memory);
  cpu.execute(line);

  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0x0);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r7")], 0xffff3fff);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r8")], 0x8080ffff);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r9")], 0xffffffff);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r10")], 0x0707);
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r11")], 0x0707);
}
