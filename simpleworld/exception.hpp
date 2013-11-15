/**
 * @file simpleworld/exception.hpp
 * Exception base class father of the rest of Simple World exceptions.
 *
 *  Copyright (C) 2004-2013  Xosé Otero <xoseotero@gmail.com>
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
#ifndef SIMPLEWORLD_EXCEPTION_HPP
#define SIMPLEWORLD_EXCEPTION_HPP

#include <exception>
#include <string>

#include <boost/current_function.hpp>

#include <simpleworld/ints.hpp>

namespace simpleworld
{

/**
 * Exception base class father of the rest of Simple CPU exceptions.
 */
class Exception: public std::exception
{
public:
  /**
   * Constructor.
   * @param file File where the exception is raised.
   * @param line Line where the exception is raised.
   * @param function Function where the exception is raised.
   * @param info What happened.
   */
  Exception(std::string file, Uint32 line, std::string function,
            std::string info) throw ()
    : std::exception(), file(file), line(line), function(function), info(info)
  {}

  /**
   * Destructor.
   */
  ~Exception() throw() {}


  /**
   * Return the information about the exception.
   * @return Info.
   */
  const char* what() const throw() { return this->info.c_str(); }


  std::string file;             /**< File where the exception is raised. */
  Uint32 line;                  /**< Line where the exception is raised. */
  std::string function;         /**< Function where the exception is raised. */
  std::string info;             /**< What happened */
};


/**
 * Macro to set the automatic values of the exceptions.
 * @param exception Exception to call.
 * @param info What happened.
 */
#define EXCEPTION(exception, info) \
  (exception(__FILE__, __LINE__, BOOST_CURRENT_FUNCTION, info))

/**
 * Macro to set the automatic values of the exceptions with 1 extra argument.
 * @param exception Exception to call.
 * @param info What happened.
 * @param extra Extra argument.
 */
#define EXCEPTION1(exception, info, extra) \
  (exception(__FILE__, __LINE__, BOOST_CURRENT_FUNCTION, info, extra))


}

#endif // SIMPLEWORLD_EXCEPTION_HPP
