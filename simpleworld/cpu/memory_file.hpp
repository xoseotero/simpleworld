/**
 * @file simpleworld/cpu/memory_file.hpp
 * Memory that loads its content from a file.
 *
 * begin:     Sat, 13 Oct 2007 12:10:20 +0200
 * last:      $Date$
 *
 *  Copyright (C) 2007  Xosé Otero <xoseotero@gmail.com>
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

#ifndef SIMPLEWORLD_CPU_MEMORY_FILE_HPP
#define SIMPLEWORLD_CPU_MEMORY_FILE_HPP

#include <string>

#include <simpleworld/cpu/memory.hpp>

namespace simpleworld
{
namespace cpu
{

/**
 * Memory that loads its content from a file.
 */
class MemoryFile: public Memory
{
public:
  /**
   * Copy constructor.
   * @param memory memory to copy.
   */
  MemoryFile(const Memory& memory);

  /**
   * Constructor.
   * @param filename filename from where to load the code.
   * @exception IOError problem with the file.
   */
  MemoryFile(const std::string& filename);


  /**
   * Load the code from a file.
   * @param filename filename from where to load the code into memory.
   * @exception IOError problem with the file.
   */
  void load_file(const std::string& filename);

  /**
   * Save the code to a file.
   * @param filename filename where to save the code from the memory.
   * @exception IOError problem with the file.
   */
  void save_file(const std::string& filename);
};

}
}

#endif // SIMPLEWORLD_CPU_MEMORY_FILE_HPP
