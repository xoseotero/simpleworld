/**
 * @file cpu/cpu_file.cpp
 * Central Processing Unit big endian with 16 registers of 32bits and 16bits of
 * address space that load the code from a file.
 *
 * begin:     Fri, 24 Nov 2006 03:17:54 +0100
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

#ifdef DEBUG
#include <iostream>
#include <boost/format.hpp>
#endif

#include <fstream>

#include <boost/filesystem/operations.hpp>
namespace fs = boost::filesystem;

#include "types.hpp"
#include "word.hpp"
#include "memory.hpp"
#include "cpu_file.hpp"

namespace SimpleWorld
{
namespace CPU
{

CPUFile::CPUFile(const std::string& filename)
  : memory_(0), CPU(&this->memory_)
{
  this->load_file(filename);
}


void CPUFile::load_file(const std::string& filename)
{
  std::ifstream is(filename.c_str(), std::ios::binary);
  if (is.rdstate() & std::ifstream::failbit)
    throw FileAccessError(__FILE__, __LINE__);

  // All instructions are 32bits, if the file is not X*32bits long is not valid
  boost::uintmax_t size = fs::file_size(filename);
  if ((size % sizeof(Word)) != 0)
    throw FileAccessError(__FILE__, __LINE__);

#ifdef DEBUG
  std::cout << boost::str(boost::format("Loading instructions from %s:")
                          % filename)
    << std::endl;
#endif

  // @TODO: File can be greater than the Address type.
  this->memory_.resize(size);

  Word instruction;
  Address i = 0;
  while (is.read(reinterpret_cast<char*>(&instruction), sizeof(Word))) {
#ifdef DEBUG
    std::cout << boost::str(boost::format("> Instruction %d: 0x%8X")
                            % i
                            % instruction)
      << std::endl;
#endif

#ifdef IS_BIG_ENDIAN
    this->memory_.set_word(i * sizeof(Word), instruction);
#else
    this->memory_.set_word(i * sizeof(Word), instruction, false);
#endif

    i++;
  }
}

}
}
