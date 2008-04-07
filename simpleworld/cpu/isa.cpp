/**
 * @file simpleworld/cpu/instruction.cpp
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

#include <boost/format.hpp>

#include <simpleworld/config.hpp>

#include "codeerror.hpp"
#include "isa.hpp"

namespace simpleworld
{
namespace cpu
{

/**
 * Constructor for a empty instruction set.
 */
ISA::ISA()
{
}


/**
 * Instruction codes.
 * @return the instruction codes.
 */
std::vector<Uint8> ISA::instruction_codes() const
{
  std::vector<Uint8> instructions;

  std::map<Uint8, InstructionInfo>::const_iterator iter =
    this->instructions_.begin();
  while (iter != this->instructions_.end()) {
    instructions.push_back((*iter).first);
    ++iter;
  }

  return instructions;
}

/**
 * Register codes.
 * @return the register codes.
 */
std::vector<Uint8> ISA::register_codes() const
{
  std::vector<Uint8> registers;

  std::map<Uint8, std::string>::const_iterator iter = this->registers_.begin();
  while (iter != this->registers_.end()) {
    registers.push_back((*iter).first);
    ++iter;
  }

  return registers;
}

/**
 * Interrupt codes.
 * @return the interrupt codes.
 */
std::vector<Uint8> ISA::interrupt_codes() const
{
  std::vector<Uint8> interrupts;

  std::map<Uint8, InterruptInfo>::const_iterator iter =
    this->interrupts_.begin();
  while (iter != this->interrupts_.end()) {
    interrupts.push_back((*iter).first);
    ++iter;
  }

  return interrupts;
}


/**
 * Info about the instruction.
 * @param code code of the instruction.
 * @return the info about the instruction.
 * @exception CodeError if the instruction is not found.
 */
InstructionInfo ISA::instruction_info(Uint8 code) const
{
  std::map<Uint8, InstructionInfo>::const_iterator iter =
    this->instructions_.find(code);
  if (iter == this->instructions_.end())
    throw EXCEPTION(CodeError, boost::str(boost::format("\
Instruction 0x%02X not found")
                                          % code));

  return (*iter).second;
}

/**
 * Code of the instruction.
 * @param name name of the instruction.
 * @return the code of the instruction.
 * @exception CodeError if the instruction is not found.
 */
Uint8 ISA::instruction_code(std::string name) const
{
  std::map<std::string, Uint8>::const_iterator iter =
    this->instruction_codes_.find(name);
  if (iter == this->instruction_codes_.end())
    throw EXCEPTION(CodeError, boost::str(boost::format("\
Instruction %1% not found")
                                          % name));

  return (*iter).second;
}

/**
 * Name of the register.
 * @param code code of the register.
 * @return the name of the register.
 * @exception CodeError if the register is not found.
 */
std::string ISA::register_name(Uint8 code) const
{
  std::map<Uint8, std::string>::const_iterator iter =
    this->registers_.find(code);
  if (iter == this->registers_.end())
    throw EXCEPTION(CodeError, boost::str(boost::format("\
Register 0x%02X not found")
                                          % code));

  return (*iter).second;
}

/**
 * Code of the register.
 * @param name name of the register.
 * @return the code of the register.
 * @exception CodeError if the register is not found.
 */
Uint8 ISA::register_code(std::string name) const
{
  std::map<std::string, Uint8>::const_iterator iter =
    this->register_codes_.find(name);
  if (iter == this->register_codes_.end())
    throw EXCEPTION(CodeError, boost::str(boost::format("\
Register %1% not found")
                                          % name));

  return (*iter).second;
}

/**
 * Info about the interrupt.
 * @param code code of the interrupt.
 * @return the info about the interrupt.
 * @exception CodeError if the interrupt is not found.
 */
InterruptInfo ISA::interrupt_info(Uint8 code) const
{
  std::map<Uint8, InterruptInfo>::const_iterator iter =
    this->interrupts_.find(code);
  if (iter == this->interrupts_.end())
    throw EXCEPTION(CodeError, boost::str(boost::format("\
Interrupt 0x%02X not found")
                                          % code));

  return (*iter).second;
}

/**
 * Code of the interrupt.
 * @param name name of the interrupt.
 * @return the code of the interrupt.
 * @exception CodeError if the interrupt is not found.
 */
Uint8 ISA::interrupt_code(std::string name) const
{
  std::map<std::string, Uint8>::const_iterator iter =
    this->interrupt_codes_.find(name);
  if (iter == this->interrupt_codes_.end())
    throw EXCEPTION(CodeError, boost::str(boost::format("\
Interrupt %1% not found")
                                          % name));

  return (*iter).second;
}


/**
 * Add a new instruction.
 * @param instruction instruction to add.
 * @exception CodeError if the instruction already exist.
 */
void ISA::add_instruction(InstructionInfo instruction)
{
  if (this->instructions_.find(instruction.code) != this->instructions_.end())
    throw EXCEPTION(CodeError, boost::str(boost::format("\
Instruction 0x%02X already exists")
                                          % instruction.code));

  this->instructions_.insert(std::pair<Uint8,
                             InstructionInfo>(instruction.code,
                                              instruction));
  this->instruction_codes_.insert(std::pair<std::string,
                                  Uint8>(instruction.name,
                                         instruction.code));
}

/**
 * Add a new instruction.
 * @param code code of the instruction.
 * @param name name of the instruction.
 * @param nregs registers used.
 * @param has_inmediate if the instruction uses the inmediate space.
 * @param func function that executes the operation.
 * @exception CodeError if the instruction already exist.
 */
void ISA::add_instruction(Uint8 code, std::string name, Uint8 nregs,
                                     bool has_inmediate, Operation func)
{
  InstructionInfo info = {code, name, nregs, has_inmediate, func};
  this->add_instruction(info);
}

/**
 * Remove a instruction.
 * @param code code of the instruction.
 * @exception CodeError if instruction is not found.
 */
void ISA::remove_instruction(Uint8 code)
{
  std::map<Uint8, InstructionInfo>::iterator iter =
    this->instructions_.find(code);
  if (iter == this->instructions_.end())
    throw EXCEPTION(CodeError, boost::str(boost::format("\
Instruction 0x%02X not found")
                                          % code));

  this->instruction_codes_.erase((*iter).second.name);
  this->instructions_.erase(iter);
}

/**
 * Add a new register.
 * @param code code of the register.
 * @param name name of the register.
 * @exception CodeError if the register already exist.
 */
void ISA::add_register(Uint8 code, std::string name)
{
  if (this->registers_.find(code) != this->registers_.end())
    throw EXCEPTION(CodeError, boost::str(boost::format("\
Register 0x%02X already exists")
                                          % code));

  this->registers_.insert(std::pair<Uint8, std::string>(code, name));
  this->register_codes_.insert(std::pair<std::string, Uint8>(name, code));
}

/**
 * Remove a register.
 * @param code code of the register.
 * @exception CPUexception if the register is not found.
 */
void ISA::remove_register(Uint8 code)
{
  std::map<Uint8, std::string>::iterator iter = this->registers_.find(code);
  if (iter == this->registers_.end())
    throw EXCEPTION(CodeError, boost::str(boost::format("\
Register 0x%02X not found")
                                          % code));

  this->register_codes_.erase((*iter).second);
  this->registers_.erase(iter);
}

/**
 * Add a new interrupt.
 * @param interrupt interrupt to add.
 * @exception CodeError if the interrupt already exist.
 */
void ISA::add_interrupt(InterruptInfo interrupt)
{
  if (this->interrupts_.find(interrupt.code) != this->interrupts_.end())
    throw EXCEPTION(CodeError, boost::str(boost::format("\
Interrupt 0x%02X already exists")
                                          % interrupt.code));

  this->interrupts_.insert(std::pair<Uint8, InterruptInfo>(interrupt.code,
                                                           interrupt));
  this->interrupt_codes_.insert(std::pair<std::string, Uint8>(interrupt.name,
                                                              interrupt.code));
}

/**
 * Add a new interrupt.
 * @param code code of the interrupt.
 * @param name name of the interrupt.
 * @param thrown_by_inst if it's thrown by a instruction.
 * @exception CodeError if the interrupt already exist.
 */
void ISA::add_interrupt(Uint8 code, std::string name, bool thrown_by_inst)
{
  InterruptInfo info = {code, name, thrown_by_inst};
  this->add_interrupt(info);
}

/**
 * Remove a interrupt.
 * @param code code of the interrupt.
 * @exception CPUexception if the interrupt is not found.
 */
void ISA::remove_interrupt(Uint8 code)
{
  std::map<Uint8, InterruptInfo>::iterator iter = this->interrupts_.find(code);
  if (iter == this->interrupts_.end())
    throw EXCEPTION(CodeError, boost::str(boost::format("\
Interrupt 0x%02X not found")
                                          % code));

  this->interrupt_codes_.erase((*iter).second.name);
  this->interrupts_.erase(iter);
}

}
}
