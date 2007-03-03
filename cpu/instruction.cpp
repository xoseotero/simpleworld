/**
 * @file cpu/instruction.cpp
 * $DESCRIPTION$
 *
 * begin:     Wed, 08 Nov 2006 16:29:51 +0100
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2006, Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  License: See COPYING file that comes with this distribution
 */

#include "../simple/config.h"
#include "word.h"
#include "instruction.h"

namespace SimpleWorld
{
namespace CPU
{

InstructionSet::InstructionSet() throw ()
{
}

InstructionSet::InstructionSet(const InstructionSet& set) throw ()
  : instructions_(set.instructions_),
    instruction_codes_(set.instruction_codes_), registers_(set.registers_),
    register_codes_(set.register_codes_)
{
}


Word InstructionSet::encode(const Instruction& instruction) throw ()
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

Instruction InstructionSet::decode(Word word) throw ()
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


std::vector<Uint8> InstructionSet::instruction_codes() const throw ()
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

std::vector<Uint8> InstructionSet::register_codes() const throw ()
{
  std::vector<Uint8> registers;

  std::map<Uint8, std::string>::const_iterator iter = this->registers_.begin();
  while (iter != this->registers_.end()) {
    registers.push_back((*iter).first);
    ++iter;
  }

  return registers;
}


InstructionInfo InstructionSet::instruction_info(Uint8 code)
  const throw (InstructionNotFound)
{
  std::map<Uint8, InstructionInfo>::const_iterator iter =
    this->instructions_.find(code);
  if (iter == this->instructions_.end())
    throw InstructionNotFound(__FILE__, __LINE__);

  return (*iter).second;
}

Uint8 InstructionSet::instruction_code(std::string name) const
  throw (InstructionNotFound)
{
  std::map<std::string, Uint8>::const_iterator iter =
    this->instruction_codes_.find(name);
  if (iter == this->instruction_codes_.end())
    throw InstructionNotFound(__FILE__, __LINE__);

  return (*iter).second;
}

std::string InstructionSet::register_name(Uint8 code) const
  throw (RegisterNotFound)
{
  std::map<Uint8, std::string>::const_iterator iter =
    this->registers_.find(code);
  if (iter == this->registers_.end())
    throw RegisterNotFound(__FILE__, __LINE__);

  return (*iter).second;
}

Uint8 InstructionSet::register_code(std::string name) const
  throw (RegisterNotFound)
{
  std::map<std::string, Uint8>::const_iterator iter =
    this->register_codes_.find(name);
  if (iter == this->register_codes_.end())
    throw RegisterNotFound(__FILE__, __LINE__);

  return (*iter).second;
}


void InstructionSet::add_instruction(InstructionInfo instruction)
  throw (InstructionExist)
{
  if (this->instructions_.find(instruction.code) != this->instructions_.end())
    throw InstructionExist(__FILE__, __LINE__);

  this->instructions_.insert(std::pair<Uint8,
                             InstructionInfo>(instruction.code,
                                              instruction));
  this->instruction_codes_.insert(std::pair<std::string,
                                  Uint8>(instruction.name,
                                         instruction.code));
}

void InstructionSet::add_instruction(Uint8 code, std::string name, Uint8 nregs,
                                     bool has_inmediate, Operation func)
  throw (InstructionExist)
{
  InstructionInfo info = {code, name, nregs, has_inmediate, func};
  this->add_instruction(info);
}

void InstructionSet::remove_instruction(Uint8 code) throw (InstructionNotFound)
{
  std::map<Uint8, InstructionInfo>::iterator iter =
    this->instructions_.find(code);
  if (iter == this->instructions_.end())
    throw InstructionNotFound(__FILE__, __LINE__);

  this->instruction_codes_.erase((*iter).second.name);
  this->instructions_.erase(iter);
}

void InstructionSet::add_register(Uint8 code, std::string name)
  throw (RegisterExist)
{
  if (this->registers_.find(code) != this->registers_.end())
    throw RegisterExist(__FILE__, __LINE__);

  this->registers_.insert(std::pair<Uint8, std::string>(code, name));
  this->register_codes_.insert(std::pair<std::string, Uint8>(name, code));
}

void InstructionSet::remove_register(Uint8 code) throw (RegisterNotFound)
{
  std::map<Uint8, std::string>::iterator iter = this->registers_.find(code);
  if (iter == this->registers_.end())
    throw RegisterNotFound(__FILE__, __LINE__);

  this->register_codes_.erase((*iter).second);
  this->registers_.erase(iter);
}

} // namespace CPU
} // namespace SimpleWorld
