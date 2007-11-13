/**
 * @file simpleworld/cpu/object.hpp
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

#ifndef __SIMPLEWORLD_CPU_OBJECT_HPP__
#define __SIMPLEWORLD_CPU_OBJECT_HPP__

#include <string>

#include <simpleworld/cpu/types.hpp>
#include <simpleworld/cpu/instruction.hpp>
#include <simpleworld/cpu/file.hpp>

namespace SimpleWorld
{
namespace CPU
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
   * @param set Instruction set of the CPU
   * @param filename File to open.
   */
  Object(const InstructionSet& set, const std::string& filename);


  /**
   * Decompile the object code to source code.
   * If a unknown instruction or register is found suppose that the value
   * is data.
   * @param filename File where to save.
   * @exception FileAccessError problem with file.
   */
  void decompile(const std::string filename) const;

protected:
  /**
   * Decompile a instruction.
   * @param instruction Instruction encoded..
   * @return The a line with the source code.
   * @exception InstructionNotFound instruction not found.
   * @exception RegisterNotFound register not found.
   */
  std::string decompile(Word instruction) const;

private:
  const InstructionSet& set_;
  std::string filename_;
};

}
}

#endif // __SIMPLEWORLD_CPU_OBJECT_HPP__
