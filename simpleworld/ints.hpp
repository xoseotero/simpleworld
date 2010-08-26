/**
 * @file simpleworld/ints.hpp
 * Definition of the integer types used in Simple World.
 *
 *  Copyright (C) 2004, 2006-2008  Xosé Otero <xoseotero@gmail.com>
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
#ifndef SIMPLEWORLD_INTS_HPP
#define SIMPLEWORLD_INTS_HPP

// __STDC_CONSTANT_MACROS must be defined to get INT64_C and UINT64_C defined
#define __STDC_CONSTANT_MACROS
#include <boost/cstdint.hpp>

#ifdef BOOST_NO_INT64_T
#error 64 bits integers not found
#endif


/**
 * Namespace for Simple World.
 */
namespace simpleworld
{

// Signed integers
typedef boost::int8_t Sint8;    /**< Signed integer with 8 bits long */
typedef boost::int16_t Sint16;  /**< Signed integer with 16 bits long */
typedef boost::int32_t Sint32;  /**< Signed integer with 32 bits long */
typedef boost::int64_t Sint64;  /**< Signed integer with 64 bits long */

// Unsigned integers
typedef boost::uint8_t Uint8;   /**< Unsigned integer with 8 bits long */
typedef boost::uint16_t Uint16; /**< Unsigned integer with 16 bits long */
typedef boost::uint32_t Uint32; /**< Unsigned integer with 32 bits long */
typedef boost::uint64_t Uint64; /**< Unsigned integer with 64 bits long */


// Macros to convert a value to 64bits
#define SINT64(number) INT64_C(number)
#define UINT64(number) UINT64_C(number)

}

#endif // SIMPLEWORLD_INTS_HPP
