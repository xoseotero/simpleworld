/**
 * @file simpleworld/exception.hpp
 * Exception base class father of the rest of Simple World exceptions.
 *
 * begin:     Sat, 11 Dec 2004 23:28:42 +0100
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
#ifndef __SIMPLEWORLD_EXCEPTION_HPP__
#define __SIMPLEWORLD_EXCEPTION_HPP__

#include <string>

#include <boost/current_function.hpp>

#include <simpleworld/ints.hpp>

namespace SimpleWorld
{

/**
 * Exception base class father of the rest of Simple CPU exceptions.
 */
class Exception
{
public:
  /**
   * Constructor.
   * @param file File where the exception is raised.
   * @param line Line where the exception is raised.
   * @param function Function where the exception is raised.
   * @param what What happened.
   */
  Exception(std::string file, Uint32 line, std::string function,
            std::string what) throw ()
    : file(file), line(line), function(function), what(what)
  {}

  virtual ~Exception() {}


  std::string file;             /**< File where the exception is raised. */
  Uint32 line;                  /**< Line where the exception is raised. */
  std::string function;         /**< Function where the exception is raised. */
  std::string what;             /**< What happened */
};


/**
 * Macro to set the automatic values of the exceptions.
 * @param exception Exception to call.
 * @param what Waht happened.
 */
#define EXCEPTION(exception, what) \
  (exception(__FILE__, __LINE__, BOOST_CURRENT_FUNCTION, what))

}

#endif // __SIMPLEWORLD_EXCEPTION_HPP__
