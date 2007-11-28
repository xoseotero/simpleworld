/**
 * @file simpleworld/cpu/isa.hpp
 * Instruction set architecture.
 *
 * begin:     Wed, 08 Nov 2006 16:29:51 +0100
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

#ifndef SIMPLEWORLD_CPU_ISA_HPP
#define SIMPLEWORLD_CPU_ISA_HPP

#include <vector>
#include <map>
#include <string>

#include <simpleworld/ints.hpp>
#include <simpleworld/cpu/types.hpp>
#include <simpleworld/cpu/instruction.hpp>
#include <simpleworld/cpu/memory.hpp>
#include <simpleworld/cpu/interrupt.hpp>

namespace SimpleWorld
{
namespace CPU
{

/**
 * How to update the CPU after a instruction.
 */
enum Update {
  None,                         /**< Do nothing */
  UpdatePC,                     /**< Update the program counter */
  UpdateInterrupt,              /**< Interrupt request */
  Stop                          /**< Stop the CPU */
};


class ISA;                      /**< Forward declaration of ISA, needed
                                     for the definition of Operation. */

/**
 * Pointer to a function that executes a instruction.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
typedef Update (*Operation) (ISA& isa, Memory& regs, Memory& mem,
                             Interrupt& interrupt, Instruction inst);

/**
 * Information about instruction's parameters.
 */
struct InstructionInfo {
  Uint8 code;                   /**< Code of the instruction */
  std::string name;             /**< Instruction name */
  Uint8 nregs;                  /**< Registers used */
  bool has_inmediate;           /**< The instruction uses the inmediate space */
  Operation func;               /**< Function that execute the operation */
};


/**
 * Instruction set architecture.
 *
 * Set of instructions: Each instruction has a code and a name.
 * Set of registers: Each register has a code and a name.
 * Set of interrupts: Each interrupt has a code and a name.
 */
class ISA
{
public:
  /**
   * Constructor for a empty instruction set.
   */
  ISA();


  /**
   * Instruction codes.
   * @return the instruction codes.
   */
  std::vector<Uint8> instruction_codes() const;

  /**
   * Register codes.
   * @return the register codes.
   */
  std::vector<Uint8> register_codes() const;

  /**
   * Interrupt codes.
   * @return the interrupt codes.
   */
  std::vector<Uint8> interrupt_codes() const;


  /**
   * Info about the instruction.
   * @param code code of the instruction.
   * @return the info about the instruction.
   * @exception CodeError if the instruction is not found.
   */
  InstructionInfo instruction_info(Uint8 code) const;

  /**
   * Code of the instruction.
   * @param name name of the instruction.
   * @return the code of the instruction.
   * @exception CodeError if the instruction is not found.
   */
  Uint8 instruction_code(std::string name) const;

  /**
   * Name of the register.
   * @param code code of the register.
   * @return the name of the register.
   * @exception CodeError if the register is not found.
   */
  std::string register_name(Uint8 code) const;

  /**
   * Code of the register.
   * @param name name of the register.
   * @return the code of the register.
   * @exception CodeError if the register is not found.
   */
  Uint8 register_code(std::string name) const;

  /**
   * Name of the interrupt.
   * @param code code of the interrupt.
   * @return the name of the interrupt.
   * @exception CodeError if the interrupt is not found.
   */
  std::string interrupt_name(Uint8 code) const;

  /**
   * Code of the interrupt.
   * @param name name of the interrupt.
   * @return the code of the interrupt.
   * @exception CodeError if the interrupt is not found.
   */
  Uint8 interrupt_code(std::string name) const;


  /**
   * Add a new instruction.
   * @param instruction instruction to add.
   * @exception CodeError if the instruction already exist.
   */
  void add_instruction(InstructionInfo instruction);

  /**
   * Add a new instruction.
   * @param code code of the instruction.
   * @param name name of the instruction.
   * @param nregs registers used.
   * @param has_inmediate if the instruction uses the inmediate space.
   * @param func function that executes the operation.
   * @exception CodeError if the instruction already exist.
   */
  void add_instruction(Uint8 code, std::string name, Uint8 nregs, bool
                       has_inmediate, Operation func);

  /**
   * Remove a instruction.
   * @param code code of the instruction.
   * @exception CodeError if instruction is not found.
   */
  void remove_instruction(Uint8 code);

  /**
   * Add a new register.
   * @param code code of the register.
   * @param name name of the register.
   * @exception CodeError if the register already exist.
   */
  void add_register(Uint8 code, std::string name);

  /**
   * Remove a register.
   * @param code code of the register.
   * @exception CPUexception if the register is not found.
   */
  void remove_register(Uint8 code);

  /**
   * Add a new interrupt.
   * @param code code of the interrupt.
   * @param name name of the interrupt.
   * @exception CodeError if the interrupt already exist.
   */
  void add_interrupt(Uint8 code, std::string name);

  /**
   * Remove a interrupt.
   * @param code code of the interrupt.
   * @exception CPUexception if the interrupt is not found.
   */
  void remove_interrupt(Uint8 code);

private:
  std::map<Uint8, InstructionInfo> instructions_;
  std::map<std::string, Uint8> instruction_codes_;

  std::map<Uint8, std::string> registers_;
  std::map<std::string, Uint8> register_codes_;

  std::map<Uint8, std::string> interrupts_;
  std::map<std::string, Uint8> interrupt_codes_;
};

}
}

#endif // SIMPLEWORLD_CPU_ISA_HPP