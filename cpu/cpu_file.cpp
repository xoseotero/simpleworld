/**
 * @file $FILE$
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

#include "../simple/types.h"
#include "word.h"
#include "memory.h"
#include "cpu_file.h"

namespace SimpleWorld
{
namespace CPU
{

CPUFile::CPUFile(const std::string& filename) throw (FileAccessError)
  : memory_(1 << 16), CPU(&this->memory_)
{
  this->load_file(filename);
}


void CPUFile::load_file(const std::string& filename) throw (FileAccessError)
{
  std::ifstream is(filename.c_str(), std::ios::binary);
  if (is.rdstate() & std::ifstream::failbit)
    throw FileAccessError(__FILE__, __LINE__);

  // All instructions are 32bits, if the file is not X*32bits long is not valid
  is.seekg(0, std::ios_base::end);
  if ((is.tellg() % sizeof(Uint32)) != 0)
    throw FileAccessError(__FILE__, __LINE__);

#ifdef DEBUG
  std::cout << boost::str(boost::format("Loading instructions from %s:")
                          % filename)
    << std::endl;
#endif
  Uint32 instruction;
  Uint32 i = 0;
  is.seekg(0);
  while (is.read(reinterpret_cast<char*>(&instruction), sizeof(Uint32))) {
#ifdef DEBUG
    std::cout << boost::str(boost::format("> Instruction %d: 0x%8X")
                            % i
                            % instruction)
      << std::endl;
#endif

#ifdef IS_BIG_ENDIAN
    this->memory_.set_word(i * sizeof(Uint32), instruction);
#else
    this->memory_.set_word(i * sizeof(Uint32), instruction, false);
#endif

    i++;
  }
}

} // namespace CPU
} // namespace SimpleWorld
