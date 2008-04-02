/**
 * @file simpleworld/cpu/object.cpp
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

#include <boost/format.hpp>

#include <simpleworld/config.hpp>
#include <simpleworld/ioerror.hpp>

#include "codeerror.hpp"
#include "word.hpp"
#include "instruction.hpp"
#include "file.hpp"
#include "object.hpp"

namespace SimpleWorld
{
namespace CPU
{

/**
 * Constructor.
 * @param isa Instruction set architecture of the CPU
 * @param filename File to open.
 */
Object::Object(const ISA& isa, const std::string& filename)
  : isa_(isa), filename_(filename)
{
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
  File file;
  File::size_type i = 0;
  Word instruction;

  std::ifstream is(this->filename_.c_str(), std::ios::binary);
  if (is.rdstate() & std::ifstream::failbit)
    throw EXCEPTION(IOError, boost::str(boost::format("\
File %1% is not readable")
                                        % filename));

  // All instructions are 32bits, if the file is not X*32bits long is not valid
  is.seekg(0, std::ios_base::end);
  if ((is.tellg() % sizeof(Word)) != 0)
    throw EXCEPTION(IOError, boost::str(boost::format("\
The size of %1% (%2%) is not a multiple of 32bits")
                                        % filename
                                        % is.tellg()));

  // Decompile all the code
  is.seekg(0);
  while (is.read(reinterpret_cast<char*>(&instruction), sizeof(Word))) {
    try {
      file.insert(i, this->decompile(instruction));
    } catch (const CodeError& e) {
      // If a unknown instruction or register is found suppose that the value
      // is data.
#ifdef IS_BIG_ENDIAN
      std::string data(boost::str(boost::format("0x%x") % instruction));
#else
      std::string data(boost::str(boost::format("0x%x") %
                                  change_byte_order(instruction)));
#endif // IS_BIG_ENDIAN
      file.insert(i, data);
    }

    i++;
  }

  file.save(filename);
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
    std::string inmediate(boost::str(boost::format("0x%x") % inst.data));;

    result += " ";
    result += inmediate;
  }

  return result;
}

}
}
