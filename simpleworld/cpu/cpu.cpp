/**
 * @file simpleworld/cpu/cpu.cpp
 * Central Processing Unit big endian with 16 registers of 32bits and 16bits of
 * address space.
 *
 * begin:     Mon, 06 Nov 2006 13:39:29 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2006-2008  Xosé Otero <xoseotero@users.sourceforge.net>
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
#include "cs.hpp"
#include "operations.hpp"

namespace simpleworld
{
namespace cpu
{

/**
 * Constructor.
 * The registers size can change to allow at least 16 registers.
 * @param registers registers of the CPU.
 * @param memory memory of the CPU.
 */
CPU::CPU(Memory* registers, Memory* memory)
  : registers_(registers), memory_(memory), running_(true)
{
  // 16 registers
  static const Address min_size = sizeof(Word) * 16;
  if (this->registers_->size() < min_size)
    this->registers_->resize(min_size);

  this->isa_.add_register(0x0, "r0");
  this->isa_.add_register(0x1, "r1");
  this->isa_.add_register(0x2, "r2");
  this->isa_.add_register(0x3, "r3");
  this->isa_.add_register(0x4, "r4");
  this->isa_.add_register(0x5, "r5");
  this->isa_.add_register(0x6, "r6");
  this->isa_.add_register(0x7, "r7");
  this->isa_.add_register(0x8, "r8");
  this->isa_.add_register(0x9, "r9");
  this->isa_.add_register(0xa, "r10");
  this->isa_.add_register(0xb, "r11");
  this->isa_.add_register(0xc, "pc");
  this->isa_.add_register(0xd, "sp");
  this->isa_.add_register(0xe, "fp");
  this->isa_.add_register(0xf, "cs");

  // Interrupts
  this->isa_.add_interrupt(INTERRUPT_TIMER, "TimerInterrupt", false);
  this->isa_.add_interrupt(INTERRUPT_SOFTWARE, "SoftwareInterrupt", true);
  this->isa_.add_interrupt(INTERRUPT_INSTRUCTION, "InvalidInstruction", true);
  this->isa_.add_interrupt(INTERRUPT_MEMORY, "InvalidMemoryLocation", true);
  this->isa_.add_interrupt(INTERRUPT_DIVISION, "DivisionByZero", true);

  // Instructions
  // Management operations
  this->isa_.add_instruction(0x00, "stop", 0, false, stop);
  this->isa_.add_instruction(0x01, "restart", 0, false, restart);

  // Move operations
  this->isa_.add_instruction(0x08, "move", 2, false, move);
  this->isa_.add_instruction(0x0a, "swap", 2, false, swap);

  // Stack operations
  this->isa_.add_instruction(0x0c, "push", 1, false, push);
  this->isa_.add_instruction(0x0e, "pop", 1, false, pop);

  // Load operations
  this->isa_.add_instruction(0x10, "load", 1, true, load);
  this->isa_.add_instruction(0x11, "loadi", 1, true, loadi);
  this->isa_.add_instruction(0x12, "loadhi", 1, true, loadhi);
  this->isa_.add_instruction(0x13, "loada", 1, true, loada);
  this->isa_.add_instruction(0x14, "loadrr", 3, false, loadrr);
  this->isa_.add_instruction(0x15, "loadri", 2, true, loadri);

  // Store operations
  this->isa_.add_instruction(0x18, "store", 1, true, store);
  this->isa_.add_instruction(0x1c, "storerr", 3, false, storerr);
  this->isa_.add_instruction(0x1d, "storeri", 2, true, storeri);

  // Branch operations
  this->isa_.add_instruction(0x20, "b", 0, true, b);
  this->isa_.add_instruction(0x22, "bz", 1, true, bz);
  this->isa_.add_instruction(0x23, "bnz", 1, true, bnz);
  this->isa_.add_instruction(0x24, "beq", 2, true, beq);
  this->isa_.add_instruction(0x25, "bne", 2, true, bne);
  this->isa_.add_instruction(0x26, "blt", 2, true, blt);
  this->isa_.add_instruction(0x27, "bltu", 2, true, bltu);
  this->isa_.add_instruction(0x28, "bgt", 2, true, bgt);
  this->isa_.add_instruction(0x29, "bgtu", 2, true, bgtu);
  this->isa_.add_instruction(0x2a, "ble", 2, true, ble);
  this->isa_.add_instruction(0x2b, "bleu", 2, true, bleu);
  this->isa_.add_instruction(0x2c, "bge", 2, true, bge);
  this->isa_.add_instruction(0x2d, "bgeu", 2, true, bgeu);

  // Function operations
  this->isa_.add_instruction(0x30, "call", 0, true, call);
  this->isa_.add_instruction(0x31, "int", 0, true,
                             ::simpleworld::cpu::interrupt);
  this->isa_.add_instruction(0x34, "ret", 0, false, ret);
  this->isa_.add_instruction(0x35, "reti", 0, false, reti);

  // Arithmetic operations
  this->isa_.add_instruction(0x40, "add", 3, false, add);
  this->isa_.add_instruction(0x41, "addi", 2, true, addi);
  this->isa_.add_instruction(0x42, "sub", 3, false, sub);
  this->isa_.add_instruction(0x43, "subi", 2, true, subi);
  this->isa_.add_instruction(0x44, "multl", 3, false, multl);
  this->isa_.add_instruction(0x45, "multli", 2, true, multli);
  this->isa_.add_instruction(0x46, "multlu", 3, false, multlu);
  this->isa_.add_instruction(0x47, "multlui", 2, true, multlui);
  this->isa_.add_instruction(0x48, "multh", 3, false, multh);
  this->isa_.add_instruction(0x49, "multhi", 2, true, multhi);
  this->isa_.add_instruction(0x4a, "multhu", 3, false, multhu);
  this->isa_.add_instruction(0x4b, "multhui", 2, true, multhui);
  this->isa_.add_instruction(0x4c, "div", 3, false, div);
  this->isa_.add_instruction(0x4d, "divi", 2, true, divi);
  this->isa_.add_instruction(0x4e, "divu", 3, false, divu);
  this->isa_.add_instruction(0x4f, "divui", 2, true, divui);
  this->isa_.add_instruction(0x50, "mod", 3, false, mod);
  this->isa_.add_instruction(0x51, "modi", 2, true, modi);
  this->isa_.add_instruction(0x52, "modu", 3, false, modu);
  this->isa_.add_instruction(0x53, "modui", 2, true, modui);

  // Logic operations
  this->isa_.add_instruction(0x60, "not", 2, false, lnot);
  this->isa_.add_instruction(0x68, "or", 3, false, lor);
  this->isa_.add_instruction(0x69, "ori", 2, true, lori);
  this->isa_.add_instruction(0x6a, "and", 3, false, land);
  this->isa_.add_instruction(0x6b, "andi", 2, true, landi);
  this->isa_.add_instruction(0x6c, "xor", 3, false, lxor);
  this->isa_.add_instruction(0x6d, "xori", 2, true, lxori);

  // Shift operations
  this->isa_.add_instruction(0x70, "sll", 3, false, sll);
  this->isa_.add_instruction(0x71, "slli", 2, true, slli);
  this->isa_.add_instruction(0x72, "srl", 3, false, srl);
  this->isa_.add_instruction(0x73, "srli", 2, true, srli);
  this->isa_.add_instruction(0x74, "sla", 3, false, sla);
  this->isa_.add_instruction(0x75, "slai", 2, true, slai);
  this->isa_.add_instruction(0x76, "sra", 3, false, sra);
  this->isa_.add_instruction(0x77, "srai", 2, true, srai);
  this->isa_.add_instruction(0x78, "rl", 3, false, rl);
  this->isa_.add_instruction(0x79, "rli", 2, true, rli);
  this->isa_.add_instruction(0x7a, "rr", 3, false, rr);
  this->isa_.add_instruction(0x7b, "rri", 2, true, rri);
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
Uint8 CPU::execute(Uint8 cycles)
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
Instruction info:\tcode: 0x%02x, name: %s, nregs: %d, has_i: %d")
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
                                 this->registers_->get_word(ADDRESS(REGISTER_PC)) + 4);
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
    this->interrupt(INTERRUPT_INSTRUCTION,
                    this->memory_->get_word(ADDRESS(REGISTER_PC)),
                    instruction.code);
  } catch (const MemoryError& exc) {
    // The pc could be out of range
    try {
      this->interrupt(INTERRUPT_MEMORY,
                      this->memory_->get_word(ADDRESS(REGISTER_PC)),
                      instruction.data);
    } catch (const MemoryError& exc) {
      // Set the instruction as 0.
      // This is a value that can't raise a exception, because is a "stop",
      // so the code can know that the pc was out of range.
      this->interrupt(INTERRUPT_MEMORY,
                      0,
                      instruction.data);
    }
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
 * Throw a interrupt.
 * @param code the ode of the interrupt.
 * @param r1 the word stored in r1.
 * @param r2 the word stored in r2.
 */
void CPU::interrupt(Uint8 code, Word r1, Word r2)
{
  // Update pc if the interrupt is thrown by a instruction.
  if (this->isa_.interrupt_info(code).thrown_by_inst)
    this->registers_->set_word(ADDRESS(REGISTER_PC),
                               this->registers_->get_word(ADDRESS(REGISTER_PC)) + 4);


  if (not this->interrupt_enabled(code)) {
#ifdef DEBUG
    std::cout << boost::str(boost::format("\
Interrupt couldn't be thrown:\tcode: 0x%02x, name: %s")
                            % static_cast<int>(code)
                            % this->isa_.interrupt_info(code).name)
                            << std::endl;
#endif

    return;
  }

#ifdef DEBUG
  std::cout << boost::str(boost::format("\
Interrupt thrown:\tcode: 0x%02x, name: %s")
                          % static_cast<int>(code)
                          % this->isa_.interrupt_info(code).name)
            << std::endl;
#endif

  CS cs(this->registers_->get_word(ADDRESS(REGISTER_CS), false));
  Word handler = this->memory_->get_word(cs.itp + ADDRESS(code));

  // Save all the registers in the stack
  Sint8 i;
  for (i = 0; i < 16; i++) {
    // Store a register:
    // Save the register in the top of the stack
    this->memory_->set_word(this->registers_->get_word(ADDRESS(REGISTER_SP)),
                            this->registers_->get_word(ADDRESS(i)));
    // Update stack pointer
    this->registers_->set_word(ADDRESS(REGISTER_SP),
                               this->registers_->get_word(ADDRESS(REGISTER_SP)) + 4);
  }

  // Update the frame pointer
  this->registers_->set_word(ADDRESS(REGISTER_FP),
                             this->registers_->get_word(ADDRESS(REGISTER_SP)));

  // Store the information of the interrupt
  this->registers_->set_word(ADDRESS(0), code);
  this->registers_->set_word(ADDRESS(1), r1);
  this->registers_->set_word(ADDRESS(2), r2);

  // Update the PC with the interrupt handler location
  this->registers_->set_word(ADDRESS(REGISTER_PC), handler);

  // Update the cs register
  cs.interrupt = true;
  cs.max_interrupts--;
  this->registers_->set_word(ADDRESS(REGISTER_CS), cs.encode(), false);
}

/**
 * Throw the Timer Interrupt.
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
  std::cout << boost::str(boost::format("Instruction[0x%08x]: 0x%08x")
    % this->registers_->get_word(ADDRESS(REGISTER_PC))
    % this->memory_->get_word(this->registers_->get_word(ADDRESS(REGISTER_PC)),
                              false))
    << std::endl;
#endif
  return Instruction(this->memory_->get_word(this->registers_->get_word(ADDRESS(REGISTER_PC)), false));
}


/**
 * Check if the interrupt is enabled.
 * @param code Interrupt to check.
 * @return true if the interrupt is enabled, not if not.
 * @exception CPUexception if the interrupt is not found.
 */
bool CPU::interrupt_enabled(Uint8 code) const
{
  CS cs(this->registers_->get_word(ADDRESS(REGISTER_CS), false));
  Word handler = this->memory_->get_word(cs.itp + ADDRESS(code));
  return (cs.enable and cs.max_interrupts > 0 and handler != 0);
}

}
}
