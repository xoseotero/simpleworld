/**
 * @file simpleworld/cpu/instruction.cpp
 * Instruction set.
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
#include "word.hpp"
#include "instruction.hpp"

namespace SimpleWorld
{
namespace CPU
{

InstructionSet::InstructionSet()
{
}

InstructionSet::InstructionSet(const InstructionSet& set)
  : instructions_(set.instructions_),
    instruction_codes_(set.instruction_codes_), registers_(set.registers_),
    register_codes_(set.register_codes_)
{
}


Word InstructionSet::encode(const Instruction& instruction)
{
  Word word;

  set_byte(&word, 0, instruction.code);
  set_byte(&word, 1, static_cast<Uint8>(instruction.first << 4 |
                                        instruction.second));

#ifdef IS_BIG_ENDIAN
  set_byte(&word, 2, get_byte(static_cast<Word>(instruction.address), 2));
  set_byte(&word, 3, get_byte(static_cast<Word>(instruction.address), 3));
#else
  set_byte(&word, 2, get_byte(static_cast<Word>(instruction.address), 1));
  set_byte(&word, 3, get_byte(static_cast<Word>(instruction.address), 0));
#endif

  return word;
}

Instruction InstructionSet::decode(Word word)
{
  Instruction instruction;

  instruction.code = get_byte(word, 0);
  instruction.first = get_byte(word, 1) >> 4;
  instruction.second = get_byte(word, 1) & 0x0F;

#ifdef IS_BIG_ENDIAN
  set_byte(&instruction.address, 0, get_byte(word, 2));
  set_byte(&instruction.address, 1, get_byte(word, 3));
#else
  set_byte(reinterpret_cast<Word*>(&instruction.address), 0,
           get_byte(word, 3));
  set_byte(reinterpret_cast<Word*>(&instruction.address), 1,
           get_byte(word, 2));
#endif

  return instruction;
}


std::vector<Uint8> InstructionSet::instruction_codes() const
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

std::vector<Uint8> InstructionSet::register_codes() const
{
  std::vector<Uint8> registers;

  std::map<Uint8, std::string>::const_iterator iter = this->registers_.begin();
  while (iter != this->registers_.end()) {
    registers.push_back((*iter).first);
    ++iter;
  }

  return registers;
}


InstructionInfo InstructionSet::instruction_info(Uint8 code) const
{
  std::map<Uint8, InstructionInfo>::const_iterator iter =
    this->instructions_.find(code);
  if (iter == this->instructions_.end())
    throw EXCEPTION(CodeError, boost::str(boost::format("\
Instruction %02x not found")
                                          % code));

  return (*iter).second;
}

Uint8 InstructionSet::instruction_code(std::string name) const
{
  std::map<std::string, Uint8>::const_iterator iter =
    this->instruction_codes_.find(name);
  if (iter == this->instruction_codes_.end())
    throw EXCEPTION(CodeError, boost::str(boost::format("\
Instruction %1% not found")
                                          % name));

  return (*iter).second;
}

std::string InstructionSet::register_name(Uint8 code) const
{
  std::map<Uint8, std::string>::const_iterator iter =
    this->registers_.find(code);
  if (iter == this->registers_.end())
    throw EXCEPTION(CodeError, boost::str(boost::format("\
Register %02x not found")
                                          % code));

  return (*iter).second;
}

Uint8 InstructionSet::register_code(std::string name) const
{
  std::map<std::string, Uint8>::const_iterator iter =
    this->register_codes_.find(name);
  if (iter == this->register_codes_.end())
    throw EXCEPTION(CodeError, boost::str(boost::format("\
Register %1% not found")
                                          % name));

  return (*iter).second;
}


void InstructionSet::add_instruction(InstructionInfo instruction)
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

void InstructionSet::add_instruction(Uint8 code, std::string name, Uint8 nregs,
                                     bool has_inmediate, Operation func)
{
  InstructionInfo info = {code, name, nregs, has_inmediate, func};
  this->add_instruction(info);
}

void InstructionSet::remove_instruction(Uint8 code)
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

void InstructionSet::add_register(Uint8 code, std::string name)
{
  if (this->registers_.find(code) != this->registers_.end())
    throw EXCEPTION(CodeError, boost::str(boost::format("\
Instruction %02x already exists")
                                          % code));

  this->registers_.insert(std::pair<Uint8, std::string>(code, name));
  this->register_codes_.insert(std::pair<std::string, Uint8>(name, code));
}

void InstructionSet::remove_register(Uint8 code)
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
