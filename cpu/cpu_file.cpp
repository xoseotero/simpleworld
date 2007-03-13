/**
 * @file cpu/cpu_file.cpp
 * Central Processing Unit big endian with 16 registers of 32bits and 16bits of
 * address space that load the code from a file.
 *
 * begin:     Fri, 24 Nov 2006 03:17:54 +0100
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2006, Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  License: See COPYING file that comes with this distribution
 */

#ifdef DEBUG
#include <iostream>
#include <boost/format.hpp>
#endif
#include <fstream>

#include "types.hpp"
#include "word.hpp"
#include "memory.hpp"
#include "cpu_file.hpp"

namespace SimpleWorld
{
namespace CPU
{

CPUFile::CPUFile(const std::string& filename)
  : memory_(1 << 16), CPU(&this->memory_)
{
  this->load_file(filename);
}


void CPUFile::load_file(const std::string& filename)
{
  std::ifstream is(filename.c_str(), std::ios::binary);
  if (is.rdstate() & std::ifstream::failbit)
    throw FileAccessError(__FILE__, __LINE__);

  // All instructions are 32bits, if the file is not X*32bits long is not valid
  is.seekg(0, std::ios_base::end);
  if ((is.tellg() % sizeof(Word)) != 0)
    throw FileAccessError(__FILE__, __LINE__);

#ifdef DEBUG
  std::cout << boost::str(boost::format("Loading instructions from %s:")
                          % filename)
    << std::endl;
#endif
  Word instruction;
  Word i = 0;
  is.seekg(0);
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
