/**
 * @file simple/exception.h
 * Exception class father of the rest of Simple World exceptions.
 *
 * begin:     Sat, 11 Dec 2004 23:28:42 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2004, 2006, Xosé Otero <xoseotero@users.sourceforge.net>
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
#ifndef __SIMPLE_EXCEPTION_H__
#define __SIMPLE_EXCEPTION_H__

#include <stdexcept>
#include <string>

#include <simple/types.hpp>

namespace SimpleWorld
{

/**
 * Dummy exception class father of the rest of Simple CPU exceptions.
 *
 * It hasn't any data.
 */
class Exception
{
public:
  /**
   * Constructor.
   * @param file File where the exception is raised.
   * @param line Line where the exception is raised.
   */
  Exception(std::string file = "", Uint32 line = 0) throw ()
    : file_(file), line_(line)
  {}


  /**
   * File where the exception is raised.
   * @return the file name.
   */
  const std::string& file() const { return this->file_; }

  /**
   * Line where the exception is raised.
   * @return the line number.
   */
  Uint32 line() const { return this->line_; }

private:
  std::string file_;
  Uint32 line_;
};

}

#endif // __SIMPLE_EXCEPTION_H__
