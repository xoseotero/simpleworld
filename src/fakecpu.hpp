/**
 * @file src/fakecpu.hpp
 * Fake CPU that add a "world" operation that does nothing.
 *
 * begin:     Sun, 04 Nov 2007 20:26:14 +0100
 * last:      $Date$
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

#ifndef FAKECPU_HPP
#define FAKECPU_HPP

#include <simpleworld/cpu/cpu.hpp>
namespace sw = simpleworld;
namespace cpu = simpleworld::cpu;

/**
 * Fake CPU that add a "world" operation that does nothing.
 */
class FakeCPU: public cpu::CPU
{
public:
  /**
   * Constructor.
   * The registers size can change to allow at least 16 registers.
   * @param registers registers of the CPU.
   * @param memory memory of the CPU.
   */
  FakeCPU(cpu::Memory* registers, cpu::Memory* memory);
};

#endif // FAKECPU_HPP
