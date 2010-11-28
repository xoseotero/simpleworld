/**
 * @file simpleworld/cpu/memoryerror.hpp
 * Exception thrown when a error about the memory is found.
 *
 *  Copyright (C) 2007  Xosé Otero <xoseotero@gmail.com>
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
#ifndef SIMPLEWORLD_CPU_MEMORYERROR_HPP
#define SIMPLEWORLD_CPU_MEMORYERROR_HPP

#include <string>

#include <simpleworld/ints.hpp>
#include <simpleworld/cpu/types.hpp>
#include <simpleworld/cpu/exception.hpp>

namespace simpleworld
{
namespace cpu
{

/**
 * Exception thrown when a error about the memory is found.
 */
class MemoryError: public CPUException
{
public:
  /**
   * Constructor.
   * @param file File where the exception is raised.
   * @param line Line where the exception is raised.
   * @param function Function where the exception is raised.
   * @param what What happened.
   * @param address Wrong address.
   */
  MemoryError(std::string file, Uint32 line, std::string function,
              std::string what, Address address) throw ()
    : CPUException(file, line, function, what), address(address)
  {}

  Address address;
};

}
}

#endif // SIMPLEWORLD_CPU_MEMORYERROR_HPP
