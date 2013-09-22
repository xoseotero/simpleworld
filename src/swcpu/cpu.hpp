/**
 * @file src/swcpu/cpu.hpp
 * FakeCPU subclass that shows information about the execution.
 *
 *  Copyright (C) 2010-2013  Xosé Otero <xoseotero@gmail.com>
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

#ifndef CPU_HPP
#define CPU_HPP

#include <string>

#include <simpleworld/cpu/types.hpp>
#include <simpleworld/cpu/memory.hpp>
#include <simpleworld/cpu/object.hpp>
#include <simpleworld/cpu/cpu.hpp>
namespace cpu = simpleworld::cpu;

#include "memoryfile.hpp"


/**
 * FakeCPU subclass that shows information about the execution.
 */
class CPU: public cpu::CPU, cpu::Object
{
public:
  /**
   * Constructor.
   * @param filename filename from where to load the code.
   * @exception FileAccessError problem with the file.
   */
  CPU(const std::string& filename) throw ();

  /**
   * Execute the next instruction.
   * @exception CPUStopped A stop instruction was found
   */
  void next();

protected:
  cpu::Memory registers_;
  MemoryFile memory_;

  cpu::Address last_access;
};

#endif // CPU_HPP
