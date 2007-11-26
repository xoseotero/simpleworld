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

namespace SimpleWorld
{
namespace CPU
{

ISA::ISA()
{
}


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


InstructionInfo ISA::instruction_info(Uint8 code) const
{
  std::map<Uint8, InstructionInfo>::const_iterator iter =
    this->instructions_.find(code);
  if (iter == this->instructions_.end())
    throw EXCEPTION(CodeError, boost::str(boost::format("\
Instruction %02x not found")
                                          % code));

  return (*iter).second;
}

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

std::string ISA::register_name(Uint8 code) const
{
  std::map<Uint8, std::string>::const_iterator iter =
    this->registers_.find(code);
  if (iter == this->registers_.end())
    throw EXCEPTION(CodeError, boost::str(boost::format("\
Register %02x not found")
                                          % code));

  return (*iter).second;
}

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


void ISA::add_instruction(InstructionInfo instruction)
{
  if (this->instructions_.find(instruction.code) != this->instructions_.end())
    throw EXCEPTION(CodeError, boost::str(boost::format("\
Instruction %02x already exists")
                                          % instruction.code));

  this->instructions_.insert(std::pair<Uint8,
                             InstructionInfo>(instruction.code,
                                              instruction));
  this->instruction_codes_.insert(std::pair<std::string,
                                  Uint8>(instruction.name,
                                         instruction.code));
}

void ISA::add_instruction(Uint8 code, std::string name, Uint8 nregs,
                                     bool has_inmediate, Operation func)
{
  InstructionInfo info = {code, name, nregs, has_inmediate, func};
  this->add_instruction(info);
}

void ISA::remove_instruction(Uint8 code)
{
  std::map<Uint8, InstructionInfo>::iterator iter =
    this->instructions_.find(code);
  if (iter == this->instructions_.end())
    throw EXCEPTION(CodeError, boost::str(boost::format("\
Instruction %02x not found")
                                          % code));

  this->instruction_codes_.erase((*iter).second.name);
  this->instructions_.erase(iter);
}

void ISA::add_register(Uint8 code, std::string name)
{
  if (this->registers_.find(code) != this->registers_.end())
    throw EXCEPTION(CodeError, boost::str(boost::format("\
Register %02x already exists")
                                          % code));

  this->registers_.insert(std::pair<Uint8, std::string>(code, name));
  this->register_codes_.insert(std::pair<std::string, Uint8>(name, code));
}

void ISA::remove_register(Uint8 code)
{
  std::map<Uint8, std::string>::iterator iter = this->registers_.find(code);
  if (iter == this->registers_.end())
    throw EXCEPTION(CodeError, boost::str(boost::format("\
Register %02x not found")
                                          % code));

  this->register_codes_.erase((*iter).second);
  this->registers_.erase(iter);
}

}
}
