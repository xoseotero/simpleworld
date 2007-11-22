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

#include <iostream>
#include <typeinfo>

#include <simpleworld/config.hpp>

#ifdef HAVE_CXXABI_H
#include <cxxabi.h>
#endif // HAVE_CXXABI_H

#include <boost/format.hpp>

#include "printexc.hpp"

std::ostream& operator <<(std::ostream& os, const sw::Exception& exc)
{
  const char* class_name = typeid(exc).name();

#ifdef HAVE_CXXABI_H
  int status;
  const char* demangled = abi::__cxa_demangle(class_name, NULL, NULL, &status);
  if (status == 0)
    class_name = demangled;
#endif // HAVE_CXXABI_H

  return os << boost::format("\
Exception %1% thrown:\n\
\twhere:\tfile: %2%, line: %3%, function: %4%\n\
\twhy:\t%5%")
    % class_name
    % exc.file
    % exc.line
    % exc.function
    % exc.what
    << std::endl;
}
