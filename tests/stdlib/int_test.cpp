/**
 * @file tests/stdlib/int_test.cpp
 * Unit test for stdlib/int.swl
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

  cpu::Source source(cpu.isa());
  source.add_include_path(INCLUDE_DIR);

  source.insert(0, file);
  source.compile(CPU_SAVE);
}


/**
 * Compile the file.
 */
BOOST_AUTO_TEST_CASE(swl_compile)
{
  cpu::File source;

  source.insert(".include \"stdlib/int.swl\"");

  source.insert("STD_ITIMER");
  source.insert("STD_ISW");
  source.insert("STD_IINST");
  source.insert("STD_IMEM");
  source.insert("STD_IDIV");
  source.insert("std_enable");
  source.insert("std_disable");
  source.insert("std_handler");
  source.insert("std_rmhandler");

  BOOST_CHECK_NO_THROW(compile(source));
}

/**
 * Check the definitions.
 */
BOOST_AUTO_TEST_CASE(swl_definitions)
{
  cpu::File source;

  source.insert(".include \"stdlib/int/def.swl\"");
  source.insert("STD_ITIMER");
  source.insert("STD_ISW");
  source.insert("STD_IINST");
  source.insert("STD_IMEM");
  source.insert("STD_IDIV");
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

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("move fp sp");
  source.insert("loada g0 heap");
  source.insert("loadi g1 0x400");
  source.insert("call std_init");
  source.insert("b main");

  source.insert(".include \"stdlib/def.swl\"");
  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"stdlib/int/def.swl\"");
  source.insert(".include \"stdlib/int/handler.swl\"");

  // Handler for interrupt
  source.insert(".label handler");
  source.insert("loadi g0 0xF505");
  source.insert("store g0 data");
  source.insert("ret");

  // Test
  source.insert(".label main");
  source.insert("loadi g0 STD_ITIMER");
  source.insert("loada g1 handler");
  source.insert("call std_handler");

  // Flag
  source.insert("loadi r5 0xF1F1");

  source.insert("load g0 data");
  source.insert("stop");

  // Space for 1 words
  source.insert(".label data");
  source.insert(".block STD_WORDSIZE");

  // Space for 256 words in the heap
  source.insert(".label heap");
  source.insert(".block 0x400");

  // Space for 32 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x80");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  int i = 0;
  while (registers[REGISTER(cpu, "r5")] != 0xF1F1) {
    cpu.execute(1);

    if (++i == MAX_CYCLES)
      break;
  }
  cpu.interrupt(INTERRUPT_TIMER);
  cpu.execute(MAX_CYCLES - i);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0xF505);
}

/**
 * Check std_handler with software interrupt.
 */
BOOST_AUTO_TEST_CASE(std_handler_software)
{
  cpu::File source;

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("move fp sp");
  source.insert("loada g0 heap");
  source.insert("loadi g1 0x400");
  source.insert("call std_init");
  source.insert("b main");

  source.insert(".include \"stdlib/def.swl\"");
  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"stdlib/int/def.swl\"");
  source.insert(".include \"stdlib/int/handler.swl\"");

  // Handler for interrupt
  source.insert(".label handler");
  source.insert("loadi g0 0xF505");
  source.insert("store g0 data");
  source.insert("ret");

  // Test
  source.insert(".label main");
  source.insert("loadi g0 STD_ISW");
  source.insert("loada g1 handler");
  source.insert("call std_handler");

  source.insert("int 0x0");

  source.insert("load g0 data");
  source.insert("stop");

  // Space for 1 words
  source.insert(".label data");
  source.insert(".block STD_WORDSIZE");

  // Space for 256 words in the heap
  source.insert(".label heap");
  source.insert(".block 0x400");

  // Space for 32 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x80");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0xF505);
}

/**
 * Check std_handler with instruction interrupt.
 */
BOOST_AUTO_TEST_CASE(std_handler_instruction)
{
  cpu::File source;

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("move fp sp");
  source.insert("loada g0 heap");
  source.insert("loadi g1 0x400");
  source.insert("call std_init");
  source.insert("b main");

  source.insert(".include \"stdlib/def.swl\"");
  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"stdlib/int/def.swl\"");
  source.insert(".include \"stdlib/int/handler.swl\"");

  // Handler for interrupt
  source.insert(".label handler");
  source.insert("loadi g0 0xF505");
  source.insert("store g0 data");
  source.insert("ret");

  // Test
  source.insert(".label main");
  source.insert("loadi g0 STD_IINST");
  source.insert("loada g1 handler");
  source.insert("call std_handler");

  source.insert("0xFFFFFFFF");

  source.insert("load g0 data");
  source.insert("stop");

  // Space for 1 words
  source.insert(".label data");
  source.insert(".block STD_WORDSIZE");

  // Space for 256 words in the heap
  source.insert(".label heap");
  source.insert(".block 0x400");

  // Space for 32 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x80");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0xF505);
}

/**
 * Check std_handler with memory interrupt.
 */
BOOST_AUTO_TEST_CASE(std_handler_memory)
{
  cpu::File source;

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("move fp sp");
  source.insert("loada g0 heap");
  source.insert("loadi g1 0x400");
  source.insert("call std_init");
  source.insert("b main");

  source.insert(".include \"stdlib/def.swl\"");
  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"stdlib/int/def.swl\"");
  source.insert(".include \"stdlib/int/handler.swl\"");

  // Handler for interrupt
  source.insert(".label handler");
  source.insert("loadi g0 0xF505");
  source.insert("store g0 data");
  source.insert("ret");

  // Test
  source.insert(".label main");
  source.insert("loadi g0 STD_IMEM");
  source.insert("loada g1 handler");
  source.insert("call std_handler");

  source.insert("load g0 0x7FFF");

  source.insert("load g0 data");
  source.insert("stop");

  // Space for 1 words
  source.insert(".label data");
  source.insert(".block STD_WORDSIZE");

  // Space for 256 words in the heap
  source.insert(".label heap");
  source.insert(".block 0x400");

  // Space for 32 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x80");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0xF505);
}

/**
 * Check std_handler with division interrupt.
 */
BOOST_AUTO_TEST_CASE(std_handler_division)
{
  cpu::File source;

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("move fp sp");
  source.insert("loada g0 heap");
  source.insert("loadi g1 0x400");
  source.insert("call std_init");
  source.insert("b main");

  source.insert(".include \"stdlib/def.swl\"");
  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"stdlib/int/def.swl\"");
  source.insert(".include \"stdlib/int/handler.swl\"");

  // Handler for interrupt
  source.insert(".label handler");
  source.insert("loadi g0 0xF505");
  source.insert("store g0 data");
  source.insert("ret");

  // Test
  source.insert(".label main");
  source.insert("loadi g0 STD_IDIV");
  source.insert("loada g1 handler");
  source.insert("call std_handler");

  source.insert("divi g0 g0 0x0");

  source.insert("load g0 data");
  source.insert("stop");

  // Space for 1 words
  source.insert(".label data");
  source.insert(".block STD_WORDSIZE");

  // Space for 256 words in the heap
  source.insert(".label heap");
  source.insert(".block 0x400");

  // Space for 32 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x80");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  cpu.execute(MAX_CYCLES);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0xF505);
}

/**
 * Check std_handler with world action interrupt.
 */
BOOST_AUTO_TEST_CASE(std_handler_worldaction)
{
  cpu::File source;

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("move fp sp");
  source.insert("loada g0 heap");
  source.insert("loadi g1 0x400");
  source.insert("call std_init");
  source.insert("b main");

  source.insert(".include \"stdlib/def.swl\"");
  source.insert(".include \"stdlib/world.swl\"");
  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"stdlib/int/handler.swl\"");

  // Handler for interrupt
  source.insert(".label handler");
  source.insert("loadi g0 0xF505");
  source.insert("store g0 data");
  source.insert("ret");

  // Test
  source.insert(".label main");
  source.insert("loadi g0 STD_IACTION");
  source.insert("loada g1 handler");
  source.insert("call std_handler");

  // Flag
  source.insert("loadi r5 0xF1F1");

  // NOP needed because World Action interrupts are thrown by instructions
  // and when caught a instruction is skipped
  source.insert("or g0 g0 g0");

  source.insert("load g0 data");
  source.insert("stop");

  // Space for 1 words
  source.insert(".label data");
  source.insert(".block STD_WORDSIZE");

  // Space for 256 words in the heap
  source.insert(".label heap");
  source.insert(".block 0x400");

  // Space for 32 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x80");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  int i = 0;
  while (registers[REGISTER(cpu, "r5")] != 0xF1F1) {
    cpu.execute(1);

    if (++i == MAX_CYCLES)
      break;
  }
  cpu.interrupt(INTERRUPT_WORLDACTION);
  cpu.execute(MAX_CYCLES - i);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0xF505);
}

/**
 * Check std_handler with worldevent interrupt.
 */
BOOST_AUTO_TEST_CASE(std_handler_worldevent)
{
  cpu::File source;

  // Initialize the stack pointer
  source.insert(".label init");
  source.insert("loada sp stack");
  source.insert("move fp sp");
  source.insert("loada g0 heap");
  source.insert("loadi g1 0x400");
  source.insert("call std_init");
  source.insert("b main");

  source.insert(".include \"stdlib/def.swl\"");
  source.insert(".include \"stdlib/world.swl\"");
  source.insert(".include \"stdlib/init.swl\"");
  source.insert(".include \"stdlib/int/handler.swl\"");

  // Handler for interrupt
  source.insert(".label handler");
  source.insert("loadi g0 0xF505");
  source.insert("store g0 data");
  source.insert("ret");

  // Test
  source.insert(".label main");
  source.insert("loadi g0 STD_IEVENT");
  source.insert("loada g1 handler");
  source.insert("call std_handler");

  // Flag
  source.insert("loadi r5 0xF1F1");

  source.insert("load g0 data");
  source.insert("stop");

  // Space for 1 words
  source.insert(".label data");
  source.insert(".block STD_WORDSIZE");

  // Space for 256 words in the heap
  source.insert(".label heap");
  source.insert(".block 0x400");

  // Space for 32 words in the stack
  source.insert(".label stack");
  source.insert(".block 0x80");

  compile(source);

  cpu::Memory registers;
  cpu::MemoryFile memory(CPU_SAVE);
  FakeCPU cpu(&registers, &memory);
  int i = 0;
  while (registers[REGISTER(cpu, "r5")] != 0xF1F1) {
    cpu.execute(1);

    if (++i == MAX_CYCLES)
      break;
  }
  cpu.interrupt(INTERRUPT_WORLDEVENT);
  cpu.execute(MAX_CYCLES - i);

  BOOST_CHECK(not cpu.running());
  BOOST_CHECK_EQUAL(registers[REGISTER(cpu, "g0")], 0xF505);
}
