/**
 * @file cpu/cpu_file.h
 * Central Processing Unit big endian with 16 registers of 32bits and 16bits of
 * address space that load the code from a file.
 *
 * begin:     Fri, 24 Nov 2006 03:12:49 +0100
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

#ifndef __CPU_CPUFILE_H__
#define __CPU_CPUFILE_H__

#include <cpu/cpu.hpp>
#include <cpu/file.hpp>

namespace SimpleWorld
{
namespace CPU
{

/**
 * CPU that load the code from a file.
 * 64KBs of memory is reserved.
 */
class CPUFile: public CPU
{
public:
  /**
   * Constructor.
   * @param filename filename from where to load the code into memory.
   * @exception FileAccessError problem with the file.
   */
  CPUFile(const std::string& filename);


  /**
   * Load the code from a file.
   * @param filename filename from where to load the code into memory.
   * @exception FileAccessError problem with the file.
   */
  void load_file(const std::string& filename);

private:
  Memory memory_;
};

}
}

#endif // __CPU_CPUFILE_H__
