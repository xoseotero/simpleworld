/**
 * @file cpu/object.h
 * Simple World Language object file.
 *
 * begin:     Sun, 19 Nov 2006 17:12:38 +0100
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2006, Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __CPU_OBJECT_H__
#define __CPU_OBJECT_H__

#include <string>

#include <cpu/instruction.h>
#include <cpu/file.h>

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
  Object(const InstructionSet& set, const std::string& filename) throw ();


  /**
   * Decompile the object code to source code.
   * If a unknown instruction or register is found suppose that the value
   * is data.
   * @param filename File where to save.
   * @exception FileAccessError problem with file.
   */
  void decompile(const std::string filename) const
    throw (FileAccessError);

protected:
  /**
   * Decompile a instruction.
   * @param instruction Instruction encoded..
   * @return The a line with the source code.
   * @exception InstructionNotFound instruction not found.
   * @exception RegisterNotFound register not found.
   */
  std::string decompile(Uint32 instruction) const
    throw (InstructionNotFound, RegisterNotFound);

private:
  const InstructionSet& set_;
  std::string filename_;
};

}
}

#endif // __CPU_OBJECT_H__
