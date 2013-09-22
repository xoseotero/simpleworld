/**
 * @file simpleworld/cpu/cpu.cpp
 * Central Processing Unit big endian with 16 registers of 32bits and 16bits of
 * address space.
 *
 *  Copyright (C) 2006-2013  Xosé Otero <xoseotero@gmail.com>
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

#ifdef DEBUG
#include <iostream>
#include <boost/format.hpp>
#endif

#include "codeerror.hpp"
#include "memoryerror.hpp"
#include "cpu.hpp"

namespace simpleworld
{
namespace cpu
{

/**
 * Constructor.
 * @param isa instruction set architecture of the CPU.
 * @param registers registers of the CPU.
 * @param memory memory of the CPU.
 */
CPU::CPU(const ISA& isa, Memory* registers, Memory* memory)
  : isa_(isa), registers_(registers), memory_(memory), running_(true)
{
  // space for all the registers (global registers + windowed registers)
  Address min_size = (TOTAL_REGISTERS) * sizeof(Word);
  if (this->registers_->size() < min_size)
    this->registers_->resize(min_size);
}


/**
 * Execute all the code until a stop instruction is found.
 */
void CPU::execute()
{
  while (this->running())
    this->next();
}

/**
 * Execute some cycles (or until a stop instruction is found).
 * @param cycles number of cycles to execute.
 * @return cycles remaining (> 0 if the CPU was stopped.
 */
Uint8 CPU::execute(Uint16 cycles)
{
  while (this->running() and (cycles > 0)) {
    this->next();

    cycles--;
  }

  return cycles;
}

/**
 * Execute the next instruction.
 * @exception CPUException A stop instruction was found
 */
void CPU::next()
{
  if (not this->running_)
    throw EXCEPTION(CPUException, "CPU stopped");

  Instruction instruction;
  try {
    instruction = this->fetch_instruction_();
    InstructionInfo info = this->isa_.instruction_info(instruction.code);
#ifdef DEBUG
    std::cout << boost::str(boost::format("\
Instruction info:\tcode: 0x%02X, name: %s, nregs: %d, has_i: %d")
                            % static_cast<int>(info.code)
                            % info.name
                            % static_cast<int>(info.nregs)
                            % static_cast<int>(info.has_inmediate))
                            << std::endl;
#endif

    switch (info.func(*this, instruction)) {
    case UpdatePC:
      // Update PC
      this->registers_->set_word(ADDRESS(REGISTER_PC),
                                 this->registers_->get_word(ADDRESS(REGISTER_PC)) + sizeof(Word));
      break;
    case UpdateInterrupt:
      // Throw a interrupt
      break;
    case UpdateStop:
      this->running_ = false;
      break;
    case UpdateNone:
      break;
    }
  } catch (const CodeError& exc) {
    // If the pc or the ip are out of range, the error is critical and
    // the CPU must be stopped
    this->interrupt(INTERRUPT_INSTRUCTION,
                    this->registers_->get_word(ADDRESS(REGISTER_PC)),
                    instruction.code);
  } catch (const MemoryError& exc) {
    // If the pc or the ip are out of range, then the error is critical and
    // the CPU must be stopped
    this->interrupt(INTERRUPT_MEMORY,
                    this->registers_->get_word(ADDRESS(REGISTER_PC)),
                    exc.address);
  }
}


/**
 * Get the value of a register.
 * @param reg the register.
 * @param system_endian if the address must be in the system endianness
 * @return the value of the register.
 */
Word CPU::get_reg(Uint8 reg, bool system_endian) const
{
  if (reg >= GLOBAL_REGISTERS) {
    Uint32 window = this->registers_->get_word(ADDRESS(REGISTER_WC));
    if (window >= REGISTER_WINDOWS)
      window = REGISTER_WINDOWS - 1;
    reg += window * REGISTERS_PER_WINDOW;
  }

  return this->registers_->get_word(ADDRESS(reg), system_endian);
}

/**
 * Set the value of a register.
 * @param reg the register.
 * @param word the new value.
 * @param system_endian if the address must be in the system endianness
 */
void CPU::set_reg(Uint8 reg, Word word, bool system_endian)
{
  if (reg >= GLOBAL_REGISTERS) {
    Uint32 window = this->registers_->get_word(ADDRESS(REGISTER_WC));
    if (window >= REGISTER_WINDOWS)
      window = REGISTER_WINDOWS - 1;
    reg += window * REGISTERS_PER_WINDOW;
  }

  this->registers_->set_word(ADDRESS(reg), word, system_endian); 
}

/**
 * Get the value of a address of memory.
 * @param addr the address of the memory.
 * @param system_endian if the address must be in the system endianness
 * @return the value of the address of memory.
 */
Word CPU::get_mem(Address addr, bool system_endian) const
{
  return this->memory_->get_word(addr, system_endian);
}

/**
 * Get the value of a address of memory.
 * @param addr the address of the memory.
 * @param system_endian if the address must be in the system endianness
 * @return the value of the address of memory.
 */
HalfWord CPU::get_halfmem(Address addr, bool system_endian) const
{
  return this->memory_->get_halfword(addr, system_endian);
}

/**
 * Get the value of a address of memory.
 * @param addr the address of the memory.
 * @return the value of the address of memory.
 */
QuarterWord CPU::get_quartermem(Address addr) const
{
  return this->memory_->get_quarterword(addr);
}

/**
 * Set the value of a address of memory.
 * @param addr the address of the memory.
 * @param word the new value.
 * @param system_endian if the address must be in the system endianness
 */
void CPU::set_mem(Address addr, Word word, bool system_endian)
{
  this->memory_->set_word(addr, word, system_endian);
}

/**
 * Set the value of a address of memory.
 * @param addr the address of the memory.
 * @param word the new value.
 * @param system_endian if the address must be in the system endianness
 */
void CPU::set_halfmem(Address addr, HalfWord word, bool system_endian)
{
  this->memory_->set_halfword(addr, word, system_endian);
}

/**
 * Set the value of a address of memory.
 * @param addr the address of the memory.
 * @param word the new value.
 */
void CPU::set_quartermem(Address addr, QuarterWord word)
{
  this->memory_->set_quarterword(addr, word);
}


/**
 * Throw a interrupt.
 * @param code the code of the interrupt.
 * @param g1 the word stored in g1.
 * @param g2 the word stored in g2.
 * @exception MemoryError if the ip is not valid.
 */
void CPU::interrupt(Uint8 code, Word g1, Word g2)
{
  // If there isn't enough space in the stack to store all the registers,
  // then a MemoryError exception is thrown.
  // This error is critical because no more interrupts can be thrown, so
  // the CPU must be stopped.

  // Update pc if the interrupt is thrown by a instruction.
  if (this->isa_.interrupt_info(code).thrown_by_inst)
    this->registers_->set_word(ADDRESS(REGISTER_PC),
                               this->registers_->get_word(ADDRESS(REGISTER_PC)) + sizeof(Word));


  if (not this->interrupt_enabled(code)) {
#ifdef DEBUG
    std::cout << boost::str(boost::format("\
Interrupt couldn't be thrown:\tcode: 0x%02X, name: %s")
                            % static_cast<int>(code)
                            % this->isa_.interrupt_info(code).name)
                            << std::endl;
#endif

    return;
  }

#ifdef DEBUG
  std::cout << boost::str(boost::format("\
Interrupt thrown:\tcode: 0x%02X, name: %s")
                          % static_cast<int>(code)
                          % this->isa_.interrupt_info(code).name)
            << std::endl;
#endif

  Word handler =
    this->memory_->get_word(this->registers_->get_word(ADDRESS(REGISTER_IP)) +
                            ADDRESS(code));

  // Save the global registers in the stack
  Word sp = this->registers_->get_word(ADDRESS(REGISTER_SP));
  this->memory_->set_word(sp, this->registers_->get_word(ADDRESS(REGISTER_IP)));
  sp += sizeof(Word);
  this->memory_->set_word(sp, this->registers_->get_word(ADDRESS(REGISTER_G0)));
  sp += sizeof(Word);
  this->memory_->set_word(sp, this->registers_->get_word(ADDRESS(REGISTER_G1)));
  sp += sizeof(Word);
  this->memory_->set_word(sp, this->registers_->get_word(ADDRESS(REGISTER_G2)));
  sp += sizeof(Word);
  this->memory_->set_word(sp, this->registers_->get_word(ADDRESS(REGISTER_G3)));
  sp += sizeof(Word);
  // Update stack pointer
  this->registers_->set_word(ADDRESS(REGISTER_SP), sp);

  // Update the wc register
  Word wc = this->registers_->get_word(ADDRESS(REGISTER_WC));
  if (wc >= (REGISTER_WINDOWS - 1))
    // Save the local registers in the stack
    for (Sint8 i = GLOBAL_REGISTERS;
         i < GLOBAL_REGISTERS + REGISTERS_PER_WINDOW;
         i++) {
      // Save the register in the top of the stack
      this->memory_->set_word(sp, this->get_reg(i));
      // Update stack pointer
      sp += sizeof(Word);
    }
  // Update stack pointer
  this->registers_->set_word(ADDRESS(REGISTER_SP), sp);
  this->registers_->set_word(ADDRESS(REGISTER_WC), wc + 1);

  // Update the link register and the frame pointer
  this->set_reg(REGISTER_LR, this->registers_->get_word(ADDRESS(REGISTER_PC)));
  this->set_reg(REGISTER_FP, this->registers_->get_word(ADDRESS(REGISTER_SP)));

  // Store the information of the interrupt
  this->registers_->set_word(ADDRESS(REGISTER_G0), code);
  this->registers_->set_word(ADDRESS(REGISTER_G1), g1);
  this->registers_->set_word(ADDRESS(REGISTER_G2), g2);

  // Execute the interrupt handler
  this->registers_->set_word(ADDRESS(REGISTER_PC), handler);
}

/**
 * Throw the Timer Interrupt.
 * @exception MemoryError if the ip is not valid.
 */
void CPU::timer_interrupt()
{
  this->interrupt(INTERRUPT_TIMER);
}


/**
 * Decode the current instruction.
 * @return the current instruction decoded.
 * @exception MemoryError if pc is out of range.
 */
Instruction CPU::fetch_instruction_() const
{
#ifdef DEBUG
  std::cout << boost::str(boost::format("Instruction[0x%08X]: 0x%08X")
    % this->registers_->get_word(ADDRESS(REGISTER_PC))
    % this->memory_->get_word(this->registers_->get_word(ADDRESS(REGISTER_PC))))
    << std::endl;
#endif
  return Instruction(this->memory_->get_word(this->registers_->get_word(ADDRESS(REGISTER_PC)), false));
}


/**
 * Check if the interrupt is enabled.
 * @param code Interrupt to check.
 * @return true if the interrupt is enabled, not if not.
 * @exception CPUexception if the interrupt is not found.
 * @exception MemoryError if the ip is not valid.
 */
bool CPU::interrupt_enabled(Uint8 code) const
{
  // If the region pointed by the ip is not valid, then a MemoryError
  // exception is thrown.
  // This error is critical because no more interrupts can be thrown, so
  // the CPU must be stopped.

  Word ip = this->registers_->get_word(ADDRESS(REGISTER_IP));
  if (ip == 0)
    return false;

  Word handler = this->memory_->get_word(ip + ADDRESS(code));
  return handler != 0;
}

}
}
