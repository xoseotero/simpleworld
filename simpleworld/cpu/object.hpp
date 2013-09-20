/**
 * @file simpleworld/cpu/object.hpp
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

#ifndef SIMPLEWORLD_CPU_OBJECT_HPP
#define SIMPLEWORLD_CPU_OBJECT_HPP

#include <string>

#include <simpleworld/cpu/types.hpp>
#include <simpleworld/cpu/isa.hpp>


class Memory;
class File;


namespace simpleworld
{
namespace cpu
{

/**
 * Simple World Language object file.
 *
 * The object code can be decompiled to source code.
 */
class Object
{
public:
  /**
   * Constructor.
   * @param isa Instruction set architecture of the CPU
   * @param code Object code.
   */
  Object(const ISA& isa, const Memory& code);

  /**
   * Constructor.
   * @param isa Instruction set architecture of the CPU
   * @param filename File to open.
   */
  Object(const ISA& isa, const std::string& filename);


  /**
   * Decompile the object code to source code.
   * If a unknown instruction or register is found suppose that the value
   * is data.
   */
  File decompile() const;

  /**
   * Decompile the object code to source code.
   * If a unknown instruction or register is found suppose that the value
   * is data.
   * @param filename File where to save.
   * @exception IOError problem with file.
   */
  void decompile(const std::string filename) const;

protected:
  /**
   * Decompile a instruction.
   * @param instruction Instruction encoded..
   * @return The a line with the source code.
   * @exception CPUException if the instruction is not found.
   * @exception CPUException if the register is not found.
   */
  std::string decompile(Word instruction) const;

private:
  const ISA& isa_;
  const Memory code_;
};

}
}

#endif // SIMPLEWORLD_CPU_OBJECT_HPP
