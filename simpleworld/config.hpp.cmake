/**
 * @file simpleworld/config.hpp
 * Configuration.
 *
 *  Copyright (C) 2007-2010  Xosé Otero <xoseotero@gmail.com>
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
#ifndef SIMPLEWORLD_CONFIG_H
#define SIMPLEWORLD_CONFIG_H

#define EXIT_SUCCESS 0

#cmakedefine IS_BIG_ENDIAN
#cmakedefine IS_LITTLE_ENDIAN

#cmakedefine UNIX
#cmakedefine WIN32

#cmakedefine HAVE_CXXABI_H

#endif // SIMPLEWORLD_CONFIG_H
