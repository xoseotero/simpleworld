/**
 * @file cpu/word.h
 * Access bytes in a word and change the byte ordering.
 *
 * begin:     Sat, 11 Dec 2004 22:06:28 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2004, 2006-2007  Xosé Otero <xoseotero@users.sourceforge.net>
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

#ifndef __CPU_WORD_H__
#define __CPU_WORD_H__

#include <simple/types.hpp>
#include <cpu/types.hpp>
#include <cpu/exception.hpp>


namespace SimpleWorld
{
namespace CPU
{

/**
 * Word exception.
 * It's raised if you try to access to a not existing bit.
 */
class ByteOutOfRange: public std::out_of_range, public CPUException
{
public:
  /**
   * Constructor.
   * @param file File where the exception is raised
   * @param line Line where the exception is raised
   * @param what Reason for the exception
   */
  ByteOutOfRange(std::string file = "", Uint32 line = 0,
                 const std::string& what = "Bit out of range") throw ()
    : out_of_range(what), CPUException(file, line)
  {}

  /**
   * Destructor.
   */
  ~ByteOutOfRange() throw () {}
};


/**
 * Return the value of a byte in a word.
 * @param word Word.
 * @param byte number of byte to get.
 * @return the byte value.
 * @exception ByteOutOfRange byte > 3
 */
Uint8 get_byte(Word word, Uint8 byte);

/**
 * Set the value of a byte in a word.
 * @param word Word.
 * @param byte Number of byte to change.
 * @param value New value for the byte.
 * @return the byte value.
 * @exception ByteOutOfRange byte > 3
 */
void set_byte(Word* word, Uint8 byte, Uint8 value);

/**
 * Change the byte order of a word.
 * Big Endian <-> Little Endian.
 * @param word Word to change.
 * @return the word with the order changed.
 */
Word change_byte_order(Word word);

/**
 * Change the order of the bytes in a word.
 * If the normal order of the byte is A B C D, the new order is B A D C.
 * @param word Word to change.
 * @return the word with the order changed.
 */
Word change_byte_order_middle(Word word);

}
}

#endif // __CPU_WORD_H__
