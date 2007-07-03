/**
 * @file cpu/cpu.h
 * Central Processing Unit big endian with 16 registers of 32bits and 16bits of
 * address space.
 *
 * begin:     Mon, 06 Nov 2006 13:39:29 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2006-2007  Xosé Otero <xoseotero@users.sourceforge.net>
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

#ifndef __CPU_CPU_H__
#define __CPU_CPU_H__

#include <string>

#include <simple/types.hpp>
#include <cpu/types.hpp>
#include <cpu/exception.hpp>
#include <cpu/memory.hpp>
#include <cpu/instruction.hpp>
#include <cpu/interrupt.hpp>

namespace SimpleWorld
{
/**
 * Namespace for Simple World CPU.
 */
namespace CPU
{

/**
 * CPU exception.
 * It's raised if the CPU is stopped after a "stop" instruction.
 */
class CPUStopped: public std::runtime_error, public CPUException
{
  /**
   * Constructor.
   * @param file File where the exception is raised.
   * @param line Line where the exception is raised.
   * @param what Reason of the exception.
   */
public:
  CPUStopped(std::string file = "", Uint32 line = 0,
             const std::string& what = "CPU Stopped") throw()
    : runtime_error(what), CPUException(file, line)
  {}

  /**
   * Destructor.
   */
  ~CPUStopped() throw () {}
};


/**
 * CPU with 16 registers of 32bits and 16bits of address space.
 */
class CPU
{
public:
  /**
   * Constructor.
   */
  CPU(Memory* memory);


  /**
   * Instruction set of the CPU.
   * @return the instruction set.
   */
  const InstructionSet& instruction_set() const { return this->set_; }

  /**
   * Registers of the CPU.
   * @return the registers.
   */
  const Memory registers() const { return this->registers_; }

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
   * Execute the next instruction.
   * @exception CPUStopped A stop instruction was found
   */
  virtual void next();

protected:
  Memory registers_;  /**< 16 registers */
  Memory* memory_;

  bool interrupt_request_;	/**< If a interrupt was thrown. */
  Interrupt interrupt_;

  InstructionSet set_; /**< Instruction set */

  bool running_;

  /**
   * Decode the current instruction.
   */
  Instruction fetch_instruction() const;

private:
  /**
   * Handle the interrupt.
   */
  void interrupt_handler_();
};

}
}

#endif // __CPU_CPU_H__
