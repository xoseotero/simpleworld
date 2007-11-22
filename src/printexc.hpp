/**
 * @file src/printexc.cpp
 * Print a exception.
 *
 * begin:     Thu, 22 Nov 2007 06:03:01 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2007  Xosé Otero <xoseotero@users.sourceforge.net>
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

#ifndef __PRINTEXC_HPP__
#define __PRINTEXC_HPP__

#include <simpleworld/exception.hpp>
namespace sw = SimpleWorld;

/**
 * Print a exception.
 * @param os Where to print.
 * @param exc Exception to print.
 * @return A reference to os.
 */
std::ostream& operator <<(std::ostream& os, const sw::Exception& exc);

#endif // __PRINTEXC_HPP__
