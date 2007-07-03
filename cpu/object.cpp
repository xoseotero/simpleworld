/**
 * @file cpu/object.cpp
 * Simple World Language object file.
 *
 * begin:     Sun, 19 Nov 2006 17:12:38 +0100
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

#include <fstream>

#include "word.hpp"
#include "object.hpp"

namespace SimpleWorld
{
namespace CPU
{

Object::Object(const InstructionSet& set, const std::string& filename)
  : set_(set), filename_(filename)
{
}


void Object::decompile(const std::string filename) const
{
  File file;
  File::size_type i = 0;
  Word instruction;

  std::ifstream is(this->filename_.c_str(), std::ios::binary);
  if (is.rdstate() & std::ifstream::failbit)
    throw FileAccessError(__FILE__, __LINE__);

  // All instructions are 32bits, if the file is not X*32bits long is not valid
  is.seekg(0, std::ios_base::end);
  if ((is.tellg() % sizeof(Word)) != 0)
    throw FileAccessError(__FILE__, __LINE__);

  is.seekg(0);
  while (is.read(reinterpret_cast<char*>(&instruction), sizeof(Word))) {
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

std::string Object::decompile(Word instruction) const
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

}
}
