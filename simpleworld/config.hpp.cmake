/**
 * @file simpleworld/config.h
 * Types definition.
 *
 * begin:     Tue, 23 Jan 2007 00:00:00 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2007, Xosé Otero <xoseotero@users.sourceforge.net>
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
#ifndef __SIMPLEWORLD_CONFIG_H__
#define __SIMPLEWORLD_CONFIG_H__

#define EXIT_SUCCESS 0

#cmakedefine IS_BIG_ENDIAN
#cmakedefine IS_LITTLE_ENDIAN

#cmakedefine UNIX
#cmakedefine WIN32

#cmakedefine HAVE_LONG_LONG
#cmakedefine HAVE__INT64

#cmakedefine HAVE_XYSSL
#cmakedefine HAVE_OPENSSL

#endif // __SIMPLEWORLD_CONFIG_H__
