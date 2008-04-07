/**
 * @file simpleworld/db/exception.hpp
 * Exception class for DB exceptions.
 *
 * begin:     Mon, 01 Jan 2007 08:49:24 +0100
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
#ifndef SIMPLEWORLD_DB_EXCEPTION_HPP
#define SIMPLEWORLD_DB_EXCEPTION_HPP

#include <string>

#include <simpleworld/ints.hpp>
#include <simpleworld/exception.hpp>

namespace simpleworld
{
namespace db
{

/**
 * Exception class for DB exceptions.
 */
class DBException: public Exception
{
public:
    /**
     * Constructor.
     * @param file File where the exception is raised.
     * @param line Line where the exception is raised.
     * @param function Function where the exception is raised.
     * @param what What happened.
     */
  DBException(std::string file, Uint32 line, std::string function,
              std::string what) throw ()
    : Exception(file, line, function, what)
  {}
};

}
}

#endif // SIMPLEWORLD_DB_EXCEPTION_HPP
