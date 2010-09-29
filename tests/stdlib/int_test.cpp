/**
 * @file tests/stdlib/int_test.cpp
 * Unit test for stdlib/int.swl
 *
 *  Copyright (C) 2009-2010  Xosé Otero <xoseotero@gmail.com>
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

#define BOOST_TEST_MODULE Unit test for stdlib/int.swl
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/bug.hpp>
#include <simpleworld/cpu/memory.hpp>
#include <simpleworld/cpu/memory_file.hpp>
#include <simpleworld/cpu/file.hpp>
#include <simpleworld/cpu/source.hpp>
#include <simpleworld/cpu/cpu.hpp>
namespace sw = simpleworld;
namespace cpu = simpleworld::cpu;

#include "src/common/fakecpu.hpp"


#define REGISTER(cpu, name) ADDRESS((cpu).isa().register_code(name))
#define CPU_SAVE (TESTOUTPUT "int.swo")
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

  source.insert(line++, ".include \"stdlib/int.swl\"");
  BOOST_CHECK_NO_THROW(compile(source));
}

/**
 * Check the definitions.
 */
BOOST_AUTO_TEST_CASE(swl_definitions)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  source.insert(line++, ".include \"stdlib/int.swl\"");
  source.insert(line++, "STD_ITIMER");
  source.insert(line++, "STD_ISW");
  source.insert(line++, "STD_IINST");
  source.insert(line++, "STD_IMEM");
  source.insert(line++, "STD_IDIV");
  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);

  BOOST_CHECK_EQUAL(memory[memory.size() - ADDRESS(5)], 0x0000);
  BOOST_CHECK_EQUAL(memory[memory.size() - ADDRESS(4)], 0x0001);
  BOOST_CHECK_EQUAL(memory[memory.size() - ADDRESS(3)], 0x0002);
  BOOST_CHECK_EQUAL(memory[memory.size() - ADDRESS(2)], 0x0003);
  BOOST_CHECK_EQUAL(memory[memory.size() - ADDRESS(1)], 0x0004);
}

/**
 * Check std_handler with timer interrupt.
 */
BOOST_AUTO_TEST_CASE(std_handler_timer)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "move fp sp");
  source.insert(line++, "loada r0 heap");
  source.insert(line++, "loadi r1 0x400");
  source.insert(line++, "loadi r2 0x1");
  source.insert(line++, "call std_init");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/def.swl\"");
  source.insert(line++, ".include \"stdlib/init.swl\"");
  source.insert(line++, ".include \"stdlib/int.swl\"");

  // Handler for interrupt
  source.insert(line++, ".label handler");
  source.insert(line++, "loadi r0 0xF505");
  source.insert(line++, "store r0 data");
  source.insert(line++, "ret");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loadi r0 STD_ITIMER");
  source.insert(line++, "loada r1 handler");
  source.insert(line++, "call std_handler");

  // Flag
  source.insert(line++, "loadi r11 0xF1F1");

  source.insert(line++, "load r0 data");
  source.insert(line++, "stop");

  // Space for 1 words
  source.insert(line++, ".label data");
  source.insert(line++, ".block STD_WORDSIZE");

  // Space for 256 words in the heap
  source.insert(line++, ".label heap");
  source.insert(line++, ".block 0x400");

  // Space for 32 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x80");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  int i = 0;
  while (registers[REGISTER(cpu, "r11")] != 0xF1F1) {
    cpu.execute(1);

    if (++i == MAX_CYCLES)
      break;
  }
  cpu.interrupt(INTERRUPT_TIMER);
  cpu.execute(MAX_CYCLES - i);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0xF505);
}

/**
 * Check std_handler with software interrupt.
 */
BOOST_AUTO_TEST_CASE(std_handler_software)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "move fp sp");
  source.insert(line++, "loada r0 heap");
  source.insert(line++, "loadi r1 0x400");
  source.insert(line++, "loadi r2 0x1");
  source.insert(line++, "call std_init");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/def.swl\"");
  source.insert(line++, ".include \"stdlib/init.swl\"");
  source.insert(line++, ".include \"stdlib/int.swl\"");

  // Handler for interrupt
  source.insert(line++, ".label handler");
  source.insert(line++, "loadi r0 0xF505");
  source.insert(line++, "store r0 data");
  source.insert(line++, "ret");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loadi r0 STD_ISW");
  source.insert(line++, "loada r1 handler");
  source.insert(line++, "call std_handler");

  source.insert(line++, "int 0x0");

  source.insert(line++, "load r0 data");
  source.insert(line++, "stop");

  // Space for 1 words
  source.insert(line++, ".label data");
  source.insert(line++, ".block STD_WORDSIZE");

  // Space for 256 words in the heap
  source.insert(line++, ".label heap");
  source.insert(line++, ".block 0x400");

  // Space for 32 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x80");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0xF505);
}

/**
 * Check std_handler with instruction interrupt.
 */
BOOST_AUTO_TEST_CASE(std_handler_instruction)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "move fp sp");
  source.insert(line++, "loada r0 heap");
  source.insert(line++, "loadi r1 0x400");
  source.insert(line++, "loadi r2 0x1");
  source.insert(line++, "call std_init");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/def.swl\"");
  source.insert(line++, ".include \"stdlib/init.swl\"");
  source.insert(line++, ".include \"stdlib/int.swl\"");

  // Handler for interrupt
  source.insert(line++, ".label handler");
  source.insert(line++, "loadi r0 0xF505");
  source.insert(line++, "store r0 data");
  source.insert(line++, "ret");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loadi r0 STD_IINST");
  source.insert(line++, "loada r1 handler");
  source.insert(line++, "call std_handler");

  source.insert(line++, "0xFFFFFFFF");

  source.insert(line++, "load r0 data");
  source.insert(line++, "stop");

  // Space for 1 words
  source.insert(line++, ".label data");
  source.insert(line++, ".block STD_WORDSIZE");

  // Space for 256 words in the heap
  source.insert(line++, ".label heap");
  source.insert(line++, ".block 0x400");

  // Space for 32 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x80");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0xF505);
}

/**
 * Check std_handler with memory interrupt.
 */
BOOST_AUTO_TEST_CASE(std_handler_memory)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "move fp sp");
  source.insert(line++, "loada r0 heap");
  source.insert(line++, "loadi r1 0x400");
  source.insert(line++, "loadi r2 0x1");
  source.insert(line++, "call std_init");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/def.swl\"");
  source.insert(line++, ".include \"stdlib/init.swl\"");
  source.insert(line++, ".include \"stdlib/int.swl\"");

  // Handler for interrupt
  source.insert(line++, ".label handler");
  source.insert(line++, "loadi r0 0xF505");
  source.insert(line++, "store r0 data");
  source.insert(line++, "ret");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loadi r0 STD_IMEM");
  source.insert(line++, "loada r1 handler");
  source.insert(line++, "call std_handler");

  source.insert(line++, "load r0 0x7FFF");

  source.insert(line++, "load r0 data");
  source.insert(line++, "stop");

  // Space for 1 words
  source.insert(line++, ".label data");
  source.insert(line++, ".block STD_WORDSIZE");

  // Space for 256 words in the heap
  source.insert(line++, ".label heap");
  source.insert(line++, ".block 0x400");

  // Space for 32 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x80");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0xF505);
}

/**
 * Check std_handler with division interrupt.
 */
BOOST_AUTO_TEST_CASE(std_handler_division)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "move fp sp");
  source.insert(line++, "loada r0 heap");
  source.insert(line++, "loadi r1 0x400");
  source.insert(line++, "loadi r2 0x1");
  source.insert(line++, "call std_init");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/def.swl\"");
  source.insert(line++, ".include \"stdlib/init.swl\"");
  source.insert(line++, ".include \"stdlib/int.swl\"");

  // Handler for interrupt
  source.insert(line++, ".label handler");
  source.insert(line++, "loadi r0 0xF505");
  source.insert(line++, "store r0 data");
  source.insert(line++, "ret");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loadi r0 STD_IDIV");
  source.insert(line++, "loada r1 handler");
  source.insert(line++, "call std_handler");

  source.insert(line++, "divi r0 r0 0x0");

  source.insert(line++, "load r0 data");
  source.insert(line++, "stop");

  // Space for 1 words
  source.insert(line++, ".label data");
  source.insert(line++, ".block STD_WORDSIZE");

  // Space for 256 words in the heap
  source.insert(line++, ".label heap");
  source.insert(line++, ".block 0x400");

  // Space for 32 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x80");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0xF505);
}

/**
 * Check std_handler with world action interrupt.
 */
BOOST_AUTO_TEST_CASE(std_handler_worldaction)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "move fp sp");
  source.insert(line++, "loada r0 heap");
  source.insert(line++, "loadi r1 0x400");
  source.insert(line++, "loadi r2 0x1");
  source.insert(line++, "call std_init");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/def.swl\"");
  source.insert(line++, ".include \"stdlib/init.swl\"");
  source.insert(line++, ".include \"stdlib/int.swl\"");

  // Handler for interrupt
  source.insert(line++, ".label handler");
  source.insert(line++, "loadi r0 0xF505");
  source.insert(line++, "store r0 data");
  source.insert(line++, "ret");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loadi r0 STD_IACTION");
  source.insert(line++, "loada r1 handler");
  source.insert(line++, "call std_handler");

  // Flag
  source.insert(line++, "loadi r11 0xF1F1");

  // NOP needed because World Action interrupts are thrown by instructions
  // and when caught a instruction is skipped
  source.insert(line++, "or r0 r0 r0");

  source.insert(line++, "load r0 data");
  source.insert(line++, "stop");

  // Space for 1 words
  source.insert(line++, ".label data");
  source.insert(line++, ".block STD_WORDSIZE");

  // Space for 256 words in the heap
  source.insert(line++, ".label heap");
  source.insert(line++, ".block 0x400");

  // Space for 32 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x80");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  int i = 0;
  while (registers[REGISTER(cpu, "r11")] != 0xF1F1) {
    cpu.execute(1);

    if (++i == MAX_CYCLES)
      break;
  }
  cpu.interrupt(INTERRUPT_WORLDACTION);
  cpu.execute(MAX_CYCLES - i);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0xF505);
}

/**
 * Check std_handler with worldevent interrupt.
 */
BOOST_AUTO_TEST_CASE(std_handler_worldevent)
{
  cpu::File source;
  cpu::Source::size_type line = 0;

  // Initialize the stack pointer
  source.insert(line++, ".label init");
  source.insert(line++, "loada sp stack");
  source.insert(line++, "move fp sp");
  source.insert(line++, "loada r0 heap");
  source.insert(line++, "loadi r1 0x400");
  source.insert(line++, "loadi r2 0x1");
  source.insert(line++, "call std_init");
  source.insert(line++, "b main");

  source.insert(line++, ".include \"stdlib/def.swl\"");
  source.insert(line++, ".include \"stdlib/init.swl\"");
  source.insert(line++, ".include \"stdlib/int.swl\"");

  // Handler for interrupt
  source.insert(line++, ".label handler");
  source.insert(line++, "loadi r0 0xF505");
  source.insert(line++, "store r0 data");
  source.insert(line++, "ret");

  // Test
  source.insert(line++, ".label main");
  source.insert(line++, "loadi r0 STD_IEVENT");
  source.insert(line++, "loada r1 handler");
  source.insert(line++, "call std_handler");

  // Flag
  source.insert(line++, "loadi r11 0xF1F1");

  source.insert(line++, "load r0 data");
  source.insert(line++, "stop");

  // Space for 1 words
  source.insert(line++, ".label data");
  source.insert(line++, ".block STD_WORDSIZE");

  // Space for 256 words in the heap
  source.insert(line++, ".label heap");
  source.insert(line++, ".block 0x400");

  // Space for 32 words in the stack
  source.insert(line++, ".label stack");
  source.insert(line++, ".block 0x80");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  int i = 0;
  while (registers[REGISTER(cpu, "r11")] != 0xF1F1) {
    cpu.execute(1);

    if (++i == MAX_CYCLES)
      break;
  }
  cpu.interrupt(INTERRUPT_WORLDEVENT);
  cpu.execute(MAX_CYCLES - i);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "r0")], 0xF505);
}
