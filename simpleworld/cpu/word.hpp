/**
 * @file simpleworld/cpu/word.hpp
 * Access bytes in a half word/word and change the byte ordering.
 *
 *  Copyright (C) 2004-2010  Xosé Otero <xoseotero@gmail.com>
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

#ifndef SIMPLEWORLD_CPU_WORD_HPP
#define SIMPLEWORLD_CPU_WORD_HPP

#include <simpleworld/ints.hpp>
#include <simpleworld/cpu/types.hpp>


namespace simpleworld
{
namespace cpu
{

/**
 * Return the value of a byte in a word.
 * @param word Word.
 * @param byte number of byte to get.
 * @return the byte value.
 * @exception CPUException if byte > 3
 */
QuarterWord get_byte(Word word, Uint8 byte);

/**
 * Return the value of a byte in a half word.
 * @param hword Half word.
 * @param byte number of byte to get.
 * @return the byte value.
 * @exception CPUException if byte > 1
 */
QuarterWord get_byte(HalfWord hword, Uint8 byte);


/**
 * Set the value of a byte in a word.
 * @param word Word.
 * @param byte Number of byte to change.
 * @param value New value for the byte.
 * @return the byte value.
 * @exception CPUException if byte > 3
 */
void set_byte(Word* word, Uint8 byte, QuarterWord value);

/**
 * Set the value of a byte in a half word.
 * @param hword Half word.
 * @param byte Number of byte to change.
 * @param value New value for the byte.
 * @return the byte value.
 * @exception CPUException if byte > 1
 */
void set_byte(HalfWord* word, Uint8 byte, QuarterWord value);


/**
 * Change the byte order of a word.
 * Big Endian <-> Little Endian.
 * @param word Word to change.
 * @return the word with the order changed.
 */
Word change_byte_order(Word word);

/**
 * Change the byte order of a half word.
 * Big Endian <-> Little Endian.
 * @param hword Half word to change.
 * @return the half word with the order changed.
 */
HalfWord change_byte_order(HalfWord hword);

}
}

#endif // SIMPLEWORLD_CPU_WORD_HPP
