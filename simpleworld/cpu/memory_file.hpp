/**
 * @file simpleworld/cpu/memory_file.hpp
 * Memory that loads its content from a file.
 *
 * begin:     Sat, 13 Oct 2007 12:10:20 +0200
 * last:      $Date$
 *
 *  Copyright (C) 2007  Xosé Antón Otero Ferreira <xoseotero@gmail.com>
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

#ifndef __SIMPLEWORLD_CPU_MEMORY_FILE_HPP__
#define __SIMPLEWORLD_CPU_MEMORY_FILE_HPP__

#include <string>

#include <simpleworld/cpu/memory.hpp>
#include <simpleworld/cpu/file.hpp>

namespace SimpleWorld
{
namespace CPU
{

/**
 * Memory that loads its content from a file.
 */
class MemoryFile: public Memory
{
public:
  /**
   * Constructor.
   * @param filename filename from where to load the code.
   * @exception FileAccessError problem with the file.
   */
  MemoryFile(const std::string& filename);


  /**
   * Load the code from a file.
   * @param filename filename from where to load the code into memory.
   * @exception FileAccessError problem with the file.
   */
  void load_file(const std::string& filename);
};

}
}

#endif // __SIMPLEWORLD_CPU_MEMORY_FILE_HPP__
