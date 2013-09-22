/**
 * @file simpleworld/cpu/instruction.cpp
 * Instruction set architecture.
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

#include <boost/format.hpp>

#include <simpleworld/config.hpp>

#include "codeerror.hpp"
#include "operations.hpp"
#include "isa.hpp"

namespace simpleworld
{
namespace cpu
{

/**
 * Constructor.
 */
ISA::ISA()
{
  for (Uint16 i = 0; i < 256; i++)
    this->instructions_[i] = NULL;

  this->add_register(REGISTER_PC, "pc");
  this->add_register(REGISTER_WC, "wc");
  this->add_register(REGISTER_SP, "sp");
  this->add_register(REGISTER_IP, "ip");
  this->add_register(REGISTER_G0, "g0");
  this->add_register(REGISTER_G1, "g1");
  this->add_register(REGISTER_G2, "g2");
  this->add_register(REGISTER_G3, "g3");
  this->add_register(REGISTER_LR, "lr");
  this->add_register(REGISTER_FP, "fp");
  this->add_register(REGISTER_R0, "r0");
  this->add_register(REGISTER_R1, "r1");
  this->add_register(REGISTER_R2, "r2");
  this->add_register(REGISTER_R3, "r3");
  this->add_register(REGISTER_R4, "r4");
  this->add_register(REGISTER_R5, "r5");

  // Interrupts
  this->add_interrupt(INTERRUPT_TIMER, "TimerInterrupt", false);
  this->add_interrupt(INTERRUPT_SOFTWARE, "SoftwareInterrupt", true);
  this->add_interrupt(INTERRUPT_INSTRUCTION, "InvalidInstruction", true);
  this->add_interrupt(INTERRUPT_MEMORY, "InvalidMemoryLocation", true);
  this->add_interrupt(INTERRUPT_DIVISION, "DivisionByZero", true);

  // Instructions
  // Management operations
  this->add_instruction(0x00, "stop", 0, false, stop);
  this->add_instruction(0x01, "restart", 0, false, restart);

  // Move operations
  this->add_instruction(0x10, "move", 2, false, move);
  this->add_instruction(0x12, "swap", 2, false, swap);

  // Stack operations
  this->add_instruction(0x18, "push", 1, false, push);
  this->add_instruction(0x1a, "pop", 1, false, pop);

  // Load operations
  this->add_instruction(0x20, "load", 1, true, load);
  this->add_instruction(0x22, "loadrr", 3, false, loadrr);
  this->add_instruction(0x23, "loadri", 2, true, loadri);
  this->add_instruction(0x24, "loadh", 1, true, loadh);
  this->add_instruction(0x26, "loadhrr", 3, false, loadhrr);
  this->add_instruction(0x27, "loadhri", 2, true, loadhri);
  this->add_instruction(0x28, "loadq", 1, true, loadq);
  this->add_instruction(0x2a, "loadqrr", 3, false, loadqrr);
  this->add_instruction(0x2b, "loadqri", 2, true, loadqri);
  this->add_instruction(0x2c, "loadi", 1, true, loadi);
  this->add_instruction(0x2d, "loadhi", 1, true, loadhi);
  this->add_instruction(0x2e, "loada", 1, true, loada);

  // Store operations
  this->add_instruction(0x30, "store", 1, true, store);
  this->add_instruction(0x32, "storerr", 3, false, storerr);
  this->add_instruction(0x33, "storeri", 2, true, storeri);
  this->add_instruction(0x34, "storeh", 1, true, storeh);
  this->add_instruction(0x36, "storehrr", 3, false, storehrr);
  this->add_instruction(0x37, "storehri", 2, true, storehri);
  this->add_instruction(0x38, "storeq", 1, true, storeq);
  this->add_instruction(0x3a, "storeqrr", 3, false, storeqrr);
  this->add_instruction(0x3b, "storeqri", 2, true, storeqri);

  // Branch operations
  this->add_instruction(0x40, "b", 0, true, b);
  this->add_instruction(0x42, "bz", 1, true, bz);
  this->add_instruction(0x43, "bnz", 1, true, bnz);
  this->add_instruction(0x44, "beq", 2, true, beq);
  this->add_instruction(0x45, "bne", 2, true, bne);
  this->add_instruction(0x46, "blt", 2, true, blt);
  this->add_instruction(0x47, "bltu", 2, true, bltu);
  this->add_instruction(0x48, "bgt", 2, true, bgt);
  this->add_instruction(0x49, "bgtu", 2, true, bgtu);
  this->add_instruction(0x4a, "ble", 2, true, ble);
  this->add_instruction(0x4b, "bleu", 2, true, bleu);
  this->add_instruction(0x4c, "bge", 2, true, bge);
  this->add_instruction(0x4d, "bgeu", 2, true, bgeu);

  // Function operations
  this->add_instruction(0x50, "call", 0, true, call);
  this->add_instruction(0x51, "callr", 1, false, callr);
  this->add_instruction(0x52, "int", 0, true,
                             ::simpleworld::cpu::interrupt);
  this->add_instruction(0x54, "ret", 0, false, ret);
  this->add_instruction(0x55, "reti", 0, false, reti);

  // Arithmetic operations
  this->add_instruction(0x60, "add", 3, false, add);
  this->add_instruction(0x61, "addi", 2, true, addi);
  this->add_instruction(0x62, "sub", 3, false, sub);
  this->add_instruction(0x63, "subi", 2, true, subi);
  this->add_instruction(0x64, "mult", 3, false, mult);
  this->add_instruction(0x65, "multi", 2, true, multi);
  this->add_instruction(0x66, "multh", 3, false, multh);
  this->add_instruction(0x67, "multhi", 2, true, multhi);
  this->add_instruction(0x68, "multhu", 3, false, multhu);
  this->add_instruction(0x69, "multhui", 2, true, multhui);
  this->add_instruction(0x6a, "div", 3, false, div);
  this->add_instruction(0x6b, "divi", 2, true, divi);
  this->add_instruction(0x6c, "mod", 3, false, mod);
  this->add_instruction(0x6d, "modi", 2, true, modi);

  // Sign extension operations
  this->add_instruction(0x70, "signh", 2, false, signh);
  this->add_instruction(0x71, "signq", 2, false, signq);

  // Logic operations
  this->add_instruction(0x80, "not", 2, false, lnot);
  this->add_instruction(0x88, "or", 3, false, lor);
  this->add_instruction(0x89, "ori", 2, true, lori);
  this->add_instruction(0x8a, "and", 3, false, land);
  this->add_instruction(0x8b, "andi", 2, true, landi);
  this->add_instruction(0x8c, "xor", 3, false, lxor);
  this->add_instruction(0x8d, "xori", 2, true, lxori);

  // Shift operations
  this->add_instruction(0x90, "sll", 3, false, sll);
  this->add_instruction(0x91, "slli", 2, true, slli);
  this->add_instruction(0x92, "srl", 3, false, srl);
  this->add_instruction(0x93, "srli", 2, true, srli);
  this->add_instruction(0x94, "sra", 3, false, sra);
  this->add_instruction(0x95, "srai", 2, true, srai);
  this->add_instruction(0x96, "rl", 3, false, rl);
  this->add_instruction(0x97, "rli", 2, true, rli);
  this->add_instruction(0x98, "rr", 3, false, rr);
  this->add_instruction(0x99, "rri", 2, true, rri);
}

/**
 * Destructor.
 */
ISA::~ISA()
{
  for (Uint16 i = 0; i < 256; i++)
    if (this->instructions_[i] != NULL)
      delete this->instructions_[i];
}


/**
 * Instruction codes.
 * @return the instruction codes.
 */
std::vector<Uint8> ISA::instruction_codes() const
{
  std::vector<Uint8> instructions;

  for (Uint16 i = 0; i < 256; i++)
    if (this->instructions_[i] != NULL)
      instructions.push_back(i);

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
  if (this->instructions_[code] == NULL)
    throw EXCEPTION(CodeError, boost::str(boost::format("\
Instruction 0x%02X not found")
                                          % code));

  return *this->instructions_[code];
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
  if (this->instructions_[instruction.code] != NULL)
    throw EXCEPTION(CodeError, boost::str(boost::format("\
Instruction 0x%02X already exists")
                                          % instruction.code));

  this->instructions_[instruction.code] = new InstructionInfo(instruction);
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
  if (this->instructions_[code] == NULL)
    throw EXCEPTION(CodeError, boost::str(boost::format("\
Instruction 0x%02X not found")
                                          % code));

  this->instruction_codes_.erase(this->instructions_[code]->name);
  delete this->instructions_[code];
  this->instructions_[code] = NULL;
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


/**
 * Global variable with the ISA.
 */
const ISA isa;

}
}
