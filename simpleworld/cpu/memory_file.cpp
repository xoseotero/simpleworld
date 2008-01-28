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

#include <fstream>

#include <boost/filesystem/operations.hpp>
#include <boost/format.hpp>
namespace fs = boost::filesystem;

#include <simpleworld/ioerror.hpp>

#include "memory_file.hpp"

namespace SimpleWorld
{
namespace CPU
{

/**
 * Copy constructor.
 * @param memory memory to copy.
 */
MemoryFile::MemoryFile(const Memory& memory)
  : Memory(memory)
{
}

/**
 * Constructor.
 * @param filename filename from where to load the code.
 * @exception IOError problem with the file.
 */
MemoryFile::MemoryFile(const std::string& filename)
  : Memory()
{
  this->load_file(filename);
}


/**
 * Load the code from a file.
 * @param filename filename from where to load the code into memory.
 * @exception IOError problem with the file.
 */
void MemoryFile::load_file(const std::string& filename)
{
  std::ifstream is(filename.c_str(), std::ios::binary);
  if (not is.is_open())
    throw EXCEPTION(IOError, boost::str(boost::format("\
File %1% is not readable")
                                        % filename));

  // All instructions are 32bits, if the file is not X*32bits long is not valid
  boost::uintmax_t size = fs::file_size(filename);
  if ((size % sizeof(Word)) != 0)
    throw EXCEPTION(IOError, boost::str(boost::format("\
The size of %1% (%2%) is not a multiple of 32bits")
                                        % filename
                                        % size));

  // @TODO: File can be greater than the Address type.
  this->resize(size);

  Word instruction;
  Address i = 0;
  while (is.read(reinterpret_cast<char*>(&instruction), sizeof(Word))) {
#ifdef IS_BIG_ENDIAN
    this->set_word(i * sizeof(Word), instruction);
#else
    this->set_word(i * sizeof(Word), instruction, false);
#endif

    i++;
  }
}

/**
 * Save the code to a file.
 * @param filename filename where to save the code from the memory.
 * @exception IOError problem with the file.
 */
void MemoryFile::save_file(const std::string& filename)
{
  std::ofstream os(filename.c_str(), std::ios::binary | std::ios::trunc);
  if (not os.is_open())
    throw EXCEPTION(IOError, boost::str(boost::format("\
File %1% is not writable")
                                        % filename));

  Word instruction;
  Address i;
  for (i = 0; i < this->size(); i += sizeof(Word)) {
#ifdef IS_BIG_ENDIAN
    instruction = this->get_word(i);
#else
    instruction = this->get_word(i, false);
#endif

    os.write(reinterpret_cast<char*>(&instruction), sizeof(Word));

    if (not os.good())
      throw EXCEPTION(IOError, boost::str(boost::format("\
Error writing to %1%")
                                          % filename));
  }
}

}
}
