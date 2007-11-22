/**
 * @file simpleworld/cpu/codeerror.hpp
 * Exception thrown when a error in the code is found.
 *
 * begin:     Fri, 16 Nov 2007 05:06:56 +0100
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
#ifndef __SIMPLEWORLD_CPU_CODEERROR_HPP__
#define __SIMPLEWORLD_CPU_CODEERROR_HPP__

#include <string>

#include <simpleworld/ints.hpp>
#include <simpleworld/cpu/exception.hpp>

namespace SimpleWorld
{
namespace CPU
{

/**
 * Exception thrown when a error in the code is found.
 */
class CodeError: public CPUException
{
public:
  /**
   * Constructor.
   * @param file File where the exception is raised.
   * @param line Line where the exception is raised.
   * @param function Function where the exception is raised.
   * @param what What happened.
   */
  CodeError(std::string file, Uint32 line, std::string function,
            std::string what) throw ()
    : CPUException(file, line, function, what)
  {}
};

}
}

#endif // __SIMPLEWORLD_CPU_CODEERROR_HPP__
