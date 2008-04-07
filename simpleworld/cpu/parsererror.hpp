/**
 * @file simpleworld/cpu/parsererror.hpp
 * Exception thrown when a error in the parser is found.
 *
 * begin:     Fri, 16 Nov 2007 05:34:33 +0100
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
#ifndef SIMPLEWORLD_CPU_PARSERERROR_HPP
#define SIMPLEWORLD_CPU_PARSERERROR_HPP

#include <string>

#include <simpleworld/ints.hpp>
#include <simpleworld/cpu/exception.hpp>

namespace simpleworld
{
namespace cpu
{

/**
 * Exception thrown when a error in the parser is found.
 */
class ParserError: public CPUException
{
public:
  /**
   * Constructor.
   * @param file File where the exception is raised.
   * @param line Line where the exception is raised.
   * @param function Function where the exception is raised.
   * @param what What happened.
   */
  ParserError(std::string file, Uint32 line, std::string function,
              std::string what) throw ()
    : CPUException(file, line, function, what)
  {}
};

}
}

#endif // SIMPLEWORLD_CPU_PARSERERROR_HPP
