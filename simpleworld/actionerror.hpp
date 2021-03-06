/**
 * @file simpleworld/actionerror.hpp
 * Class thrown when a error about a action is found.
 *
 *  Copyright (C) 2007-2013  Xosé Otero <xoseotero@gmail.com>
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
#ifndef SIMPLEWORLD_ACTIONERROR_HPP
#define SIMPLEWORLD_ACTIONERROR_HPP

#include <string>

#include <simpleworld/ints.hpp>
#include <simpleworld/worlderror.hpp>

namespace simpleworld
{

/**
 * Class thrown when a error about a action is found.
 */
class ActionError: public WorldError
{
public:
  /**
   * Constructor.
   * @param file File where the exception is raised.
   * @param line Line where the exception is raised.
   * @param function Function where the exception is raised.
   * @param info What happened.
   */
  ActionError(std::string file, Uint32 line, std::string function,
              std::string info) throw ()
    : WorldError(file, line, function, info)
  {}
};

}

#endif // SIMPLEWORLD_ACTIONERROR_HPP
