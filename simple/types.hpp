/**
 * @file simple/types.h
 * Types definition.
 * @todo In C99 there is the stdint.h where is defined intX_t and uintX_t, but
 * are not used.
 *
 * begin:     Sat, 11 Dec 2004 11:45:52 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2004, 2006, Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef __SIMPLE_TYPES_H__
#define __SIMPLE_TYPES_H__

#include <simple/config.hpp>

/**
 * Namespace for Simple World.
 */
namespace SimpleWorld
{

// Signed integers
typedef signed char Sint8;           /**< Signed integer with 8 bits long */
typedef short Sint16;                /**< Signed integer with 16 bits long */
typedef long Sint32;                 /**< Signed integer with 32 bits long */
#ifdef HAVE_LONG_LONG
typedef long long Sint64;            /**< Signed integer with 64 bits long */
#define SINT64(number) (number ## LL)
#else
# ifdef HAVE__INT64
typedef __int64 Sint64;              /**< Signed integer with 64 bits long */
#define SINT64(number) (number ## i64)
# endif // HAVE__INT64
#endif // HAVE_LONG_LONG

// Unsigned integers
typedef unsigned char Uint8;         /**< Unsigned integer with 8 bits long */
typedef unsigned short Uint16;       /**< Unsigned integer with 16 bits long */
typedef unsigned long Uint32;        /**< Unsigned integer with 32 bits long */
#ifdef HAVE_LONG_LONG
typedef unsigned long long Uint64;   /**< Unsigned integer with 64 bits long */
#define UINT64(number) (number ## ULL)
#else
# ifdef HAVE__INT64
typedef unsigned __int64 Uint64;     /**< Signed integer with 64 bits long */
#define UINT64(number) (number ## ui64)
# endif // HAVE__INT64
#endif // HAVE_LONG_LONG

}

#endif // __SIMPLE_TYPES_H__
