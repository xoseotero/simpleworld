/**
 * @file cpu/object.cpp
 * Simple World Language object file.
 *
 * begin:     Sun, 19 Nov 2006 17:12:38 +0100
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2006, Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  License: See COPYING file that comes with this distribution
 */

#include <fstream>

#include "word.h"
#include "object.h"

namespace SimpleWorld
{
namespace CPU
{

Object::Object(const InstructionSet& set, const std::string& filename)
  throw ()
  : set_(set), filename_(filename)
{
}


void Object::decompile(const std::string filename) const
  throw (FileAccessError)
{
  File file;
  File::size_type i = 0;
  Uint32 instruction;

  std::ifstream is(this->filename_.c_str(), std::ios::binary);
  if (is.rdstate() & std::ifstream::failbit)
    throw FileAccessError(__FILE__, __LINE__);

  // All instructions are 32bits, if the file is not X*32bits long is not valid
  is.seekg(0, std::ios_base::end);
  if ((is.tellg() % sizeof(Uint32)) != 0)
    throw FileAccessError(__FILE__, __LINE__);

  is.seekg(0);
  while (is.read(reinterpret_cast<char*>(&instruction), sizeof(Uint32))) {
    // If a unknown instruction or register is found suppose that the value
    // is data.
    try {
      file.insert(i, this->decompile(instruction));
    }
    catch (InstructionNotFound&) {
      char address[11];
      std::snprintf(address, 11, "%x", instruction);
      file.insert(i, address);
    }
    catch (RegisterNotFound&) {
      char address[11];
      std::snprintf(address, 11, "%x", instruction);
      file.insert(i, address);
    }

    i++;
  }

  file.save(filename);
}

std::string Object::decompile(Uint32 instruction) const
  throw (InstructionNotFound, RegisterNotFound)
{
  Instruction inst = InstructionSet::decode(instruction);
  InstructionInfo info = this->set_.instruction_info(inst.code);

  std::string result;
  result += info.name;
  if (info.nregs > 0) {
    result += " ";
    result += this->set_.register_name(inst.first);
  }
  if (info.nregs > 1) {
    result += " ";
    result += this->set_.register_name(inst.second);
  }
  // Instruction::address can store the third register or a inmediate value
  if (info.nregs > 2) {
    result += " ";
    result += this->set_.register_name(static_cast<Uint8>(inst.address));
  } else if (info.has_inmediate) {
    char address[7];
    std::snprintf(address, 7, "0x%x", inst.address);
    result += " ";
    result += address;
  }

  return result;
}

} // namespace CPU
} // namespace SimpleWorld
