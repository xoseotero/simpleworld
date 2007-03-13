/**
 * @file cpu/exception.h
 * Exception class father of the rest of CPU exceptions.
 *
 * begin:     Sat, 11 Dec 2004 23:28:42 +0100
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2004, 2006-2007 Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef __CPU_EXCEPTION_H__
#define __CPU_EXCEPTION_H__

#include <string>

#include <simple/types.hpp>
#include <simple/exception.hpp>

namespace SimpleWorld
{
namespace CPU
{

/**
 * Exception class father of the rest of CPU exceptions.
 *
 * It hasn't any data.
 */
class CPUException: public Exception
{
public:
  /**
   * Constructor.
   * @param file File where the exception is raised.
   * @param line Line where the exception is raised.
   */
  CPUException(std::string file = "", Uint32 line = 0) throw ()
    : Exception(file, line)
  {}
};

}
}

#endif // __CPU_EXCEPTION_H__
