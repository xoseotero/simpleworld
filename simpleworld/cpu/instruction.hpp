/**
 * @file simpleworld/cpu/instruction.hpp
 * A instruction.
 *
 *  Copyright (C) 2006-2007  Xosé Otero <xoseotero@gmail.com>
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

#ifndef SIMPLEWORLD_CPU_INSTRUCTION_HPP
#define SIMPLEWORLD_CPU_INSTRUCTION_HPP

#include <simpleworld/ints.hpp>
#include <simpleworld/cpu/types.hpp>

namespace simpleworld
{
namespace cpu
{

/**
 * A instruction.
 */
class Instruction
{
public:
  /**
   * Default constructor.
   */
  Instruction();

  /**
   * Constructor that decodes a word (in big endian).
   * @param word word to decode (in big endian).
   */
  Instruction(Word word);


  /**
   * Encode the instruction to a Word (in big endian).
   * @return the instruction encoded.
   */
  Word encode() const;

  /**
   * Decode the instruction (in big endian).
   * @param word word to decode (in big endian).
   */
  void decode(Word word);


  // data
  Uint8 code;                   /**< Code of the instruction */

  Uint8 first:4;                /**< First operand of the operation (destiny) */
  Uint8 second:4;               /**< Second operand of the operation
                                     (first source) */

  union {
    // third is not defined because I don't know how the data is aligned in
    // all the CPUs/compilers
    // Uint8 third:4;
    Uint16 data;                /**< Data or third operand of the operation
                                     (second source) */
    Offset offset;              /**< Memory offset */
  };
};

}
}

#endif // SIMPLEWORLD_CPU_INSTRUCTION_HPP
