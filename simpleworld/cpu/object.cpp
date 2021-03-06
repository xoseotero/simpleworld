/**
 * @file simpleworld/cpu/object.cpp
 * Simple World Language object file.
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

#include <fstream>

#include <boost/format.hpp>

#include <simpleworld/config.hpp>
#include <simpleworld/ioerror.hpp>

#include "codeerror.hpp"
#include "word.hpp"
#include "instruction.hpp"
#include "memory.hpp"
#include "memory_file.hpp"
#include "file.hpp"
#include "object.hpp"

namespace simpleworld
{
namespace cpu
{

/**
 * Constructor.
 * @param isa Instruction set architecture of the CPU
 * @param code Object code.
 */
Object::Object(const ISA& isa, const Memory& code)
  : isa_(isa), code_(code)
{
  // All instructions are 32bits, if the file is not X*32bits long is not valid
  if ((this->code_.size() % sizeof(Word)) != 0)
    throw EXCEPTION(IOError, boost::str(boost::format("\
The size of the object code (%1%) is not a multiple of 32bits")
                                        % this->code_.size()));
}

/**
 * Constructor.
 * @param isa Instruction set architecture of the CPU
 * @param filename File to open.
 */
Object::Object(const ISA& isa, const std::string& filename)
  : isa_(isa), code_(MemoryFile(filename))
{
  // All instructions are 32bits, if the file is not X*32bits long is not valid
  if ((this->code_.size() % sizeof(Word)) != 0)
    throw EXCEPTION(IOError, boost::str(boost::format("\
The size of %1% (%2%) is not a multiple of 32bits")
                                        % filename
                                        % this->code_.size()));
}


/**
 * Decompile the object code to source code.
 * If a unknown instruction or register is found suppose that the value
 * is data.
 */
File Object::decompile() const
{
  File file;
  Word instruction;

  // Decompile all the code
  for (Address i = 0; i < this->code_.size(); i += sizeof(Word)) {
    instruction = this->code_.get_word(i, false);
    try {
      file.insert(this->decompile(instruction));
    } catch (const CodeError& e) {
      // If a unknown instruction or register is found suppose that the value
      // is data.
#if defined(IS_BIG_ENDIAN)
      std::string data(boost::str(boost::format("0x%08X") % instruction));
#elif defined(IS_LITTLE_ENDIAN)
      std::string data(boost::str(boost::format("0x%08X") %
                                  change_byte_order(instruction)));
#else
#error endianness not specified
#endif
      file.insert(data);
    }
  }

  return file;
}

/**
 * Decompile the object code to source code.
 * If a unknown instruction or register is found suppose that the value
 * is data.
 * @param filename File where to save.
 * @exception IOError problem with file.
 */
void Object::decompile(const std::string filename) const
{
  this->decompile().save(filename);
}


/**
 * Decompile a instruction.
 * @param instruction Instruction encoded..
 * @return The a line with the source code.
 * @exception CPUException if the instruction is not found.
 * @exception CPUException if the register is not found.
 */
std::string Object::decompile(Word instruction) const
{
  Instruction inst(instruction);
  InstructionInfo info = this->isa_.instruction_info(inst.code);

  std::string result;
  result += info.name;
  if (info.nregs > 0) {
    result += " ";
    result += this->isa_.register_name(inst.first);
  }
  if (info.nregs > 1) {
    result += " ";
    result += this->isa_.register_name(inst.second);
  }
  // Instruction::address can store the third register or data
  if (info.nregs > 2) {
    result += " ";
    result += this->isa_.register_name(static_cast<Uint8>(inst.data));
  } else if (info.has_inmediate) {
    std::string inmediate(boost::str(boost::format("0x%04X") % inst.data));;

    result += " ";
    result += inmediate;
  }

  return result;
}

}
}
