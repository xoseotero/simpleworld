/**
 * @file simpleworld/cpu/cpu.hpp
 * Central Processing Unit big endian with 16 registers of 32bits and 16bits of
 * address space.
 *
 *  Copyright (C) 2006-2012  Xosé Otero <xoseotero@gmail.com>
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

#ifndef SIMPLEWORLD_CPU_CPU_HPP
#define SIMPLEWORLD_CPU_CPU_HPP

#include <string>

#include <simpleworld/ints.hpp>
#include <simpleworld/cpu/types.hpp>
#include <simpleworld/cpu/memory.hpp>
#include <simpleworld/cpu/instruction.hpp>
#include <simpleworld/cpu/isa.hpp>

#define GLOBAL_REGISTERS        8      // Shared registers
#define REGISTERS_PER_WINDOW    8      // Registers per window
#define REGISTER_WINDOWS        16     // Number of register windows
#define TOTAL_REGISTERS (GLOBAL_REGISTERS + \
                         REGISTERS_PER_WINDOW * REGISTER_WINDOWS)

#define REGISTER_WC 0x0
#define REGISTER_PC 0x1
#define REGISTER_SP 0x2
#define REGISTER_IP 0x3
#define REGISTER_G0 0x4
#define REGISTER_G1 0x5
#define REGISTER_G2 0x6
#define REGISTER_G3 0x7
#define REGISTER_LR 0x8
#define REGISTER_FP 0x9
#define REGISTER_R0 0xA
#define REGISTER_R1 0xB
#define REGISTER_R2 0xC
#define REGISTER_R3 0xD
#define REGISTER_R4 0xE
#define REGISTER_R5 0xF

#define INTERRUPT_TIMER (0x0)
#define INTERRUPT_SOFTWARE (0x1)
#define INTERRUPT_INSTRUCTION (0x2)
#define INTERRUPT_MEMORY (0x3)
#define INTERRUPT_DIVISION (0x4)

// Address of nth element in the memory
#define ADDRESS(n) ((n) * sizeof(::simpleworld::cpu::Word))

namespace simpleworld
{
/**
 * Namespace for Simple World CPU.
 */
namespace cpu
{

/**
 * CPU with 16 registers of 32bits and 16bits of address space.
 */
class CPU
{
public:
  /**
   * Constructor.
   * @param registers registers of the CPU.
   * @param memory memory of the CPU.
   */
  CPU(Memory* registers, Memory* memory);

  /**
   * Destructor.
   */
  virtual ~CPU() {}


  /**
   * Instruction set architecture of the CPU.
   * @return the instruction set architecture.
   */
  const ISA& isa() const { return this->isa_; }

  /**
   * Registers of the CPU.
   * @return the registers.
   */
  const Memory registers() const { return *this->registers_; }

  /**
   * Memory of the CPU.
   * @return the memory.
   */
  const Memory memory() const { return *this->memory_; }


  /**
   * Return if the CPU is running
   * @return true if the CPU is running, false if not
   */
  bool running() const { return this->running_; }


  /**
   * Execute all the code until a stop instruction is found.
   */
  void execute();

  /**
   * Execute some cycles (or until a stop instruction is found).
   * @param cycles number of cycles to execute.
   * @return cycles remaining (> 0 if the CPU was stopped.
   */
  Uint8 execute(Uint16 cycles);

  /**
   * Execute the next instruction.
   * @exception CPUException A stop instruction was found
   */
  virtual void next();


  // Basic operations of the CPU
  /**
   * Get the value of a register.
   * @param reg the register.
   * @param system_endian if the address must be in the system endianness
   * @return the value of the register.
   */
  Word get_reg(Uint8 reg, bool system_endian = true) const;

  /**
   * Set the value of a register.
   * @param reg the register.
   * @param word the new value.
   * @param system_endian if the address must be in the system endianness
   */
  void set_reg(Uint8 reg, Word word, bool system_endian = true);

  /**
   * Get the value of a address of memory.
   * @param addr the address of the memory.
   * @param system_endian if the address must be in the system endianness
   * @return the value of the address of memory.
   */
  Word get_mem(Address addr, bool system_endian = true) const;

  /**
   * Get the value of a address of memory.
   * @param addr the address of the memory.
   * @param system_endian if the address must be in the system endianness
   * @return the value of the address of memory.
   */
  HalfWord get_halfmem(Address addr, bool system_endian = true) const;

  /**
   * Get the value of a address of memory.
   * @param addr the address of the memory.
   * @return the value of the address of memory.
   */
  QuarterWord get_quartermem(Address addr) const;

  /**
   * Set the value of a address of memory.
   * @param addr the address of the memory.
   * @param word the new value.
   * @param system_endian if the address must be in the system endianness
   */
  void set_mem(Address addr, Word word, bool system_endian = true);

  /**
   * Set the value of a address of memory.
   * @param addr the address of the memory.
   * @param word the new value.
   * @param system_endian if the address must be in the system endianness
   */
  void set_halfmem(Address addr, HalfWord word, bool system_endian = true);

  /**
   * Set the value of a address of memory.
   * @param addr the address of the memory.
   * @param word the new value.
   */
  void set_quartermem(Address addr, QuarterWord word);


  /**
   * Throw a interrupt.
   * @param code the code of the interrupt.
   * @param g1 the word stored in g1.
   * @param g2 the word stored in g2.
   * @exception MemoryError if the ip is not valid.
   */
  virtual void interrupt(Uint8 code, Word g1 = 0, Word g2 = 0);

  /**
   * Throw the Timer Interrupt.
   * @exception MemoryError if the ip is not valid.
   */
  void timer_interrupt();

protected:
  ISA isa_;                     /**< Instruction set */

  Memory* registers_;
  Memory* memory_;

  bool running_;


  /**
   * Decode the current instruction.
   * @return the current instruction decoded.
   * @exception MemoryError if pc is out of range.
   */
  Instruction fetch_instruction_() const;


  /**
   * Check if the interrupt is enabled.
   * @param code Interrupt to check.
   * @return true if the interrupt is enabled, not if not.
   * @exception CPUexception if the interrupt is not found.
   * @exception MemoryError if the ip is not valid.
   */
  bool interrupt_enabled(Uint8 code) const;
};

}
}

#endif // SIMPLEWORLD_CPU_CPU_HPP
