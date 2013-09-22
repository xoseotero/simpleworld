/**
 * @file src/common/fakeisa.hpp
 * Fake ISA that adds a "world" operation that does nothing.
 *
 *  Copyright (C) 2013  Xosé Otero <xoseotero@gmail.com>
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

#ifndef FAKEISA_HPP
#define FAKEISA_HPP

#include <simpleworld/cpu/isa.hpp>
namespace sw = simpleworld;
namespace cpu = simpleworld::cpu;

/**
 * Fake ISA that adds a "world" operation that does nothing.
 */
class FakeISA: public cpu::ISA
{
public:
  /**
   * Constructor.
   */
  FakeISA();
};


/**
 * Global variable with the fake ISA.
 */
extern const FakeISA fakeisa;

#endif // FAKEISA_HPP
