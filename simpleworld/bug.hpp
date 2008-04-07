/**
 * @file simpleworld/bug.hpp
 * A bug in Simple World.
 *
 * begin:     Sat, 21 Jul 2007 12:01:19 +0200
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

#ifndef SIMPLEWORLD_BUG_HPP
#define SIMPLEWORLD_BUG_HPP

#include <map>

#include <simpleworld/egg.hpp>
#include <simpleworld/cpu/cpu.hpp>
#include <simpleworld/db/bug.hpp>

#define INTERRUPT_WORLDACTION (0x5)
#define INTERRUPT_WORLDEVENT (0x6)

namespace simpleworld
{

class SimpleWorld;

class Bug: public db::Bug, public cpu::CPU
{
public:
  /**
   * Constructor.
   * @param sw world where the bug lives.
   * @param id id of the bug.
   * @exception DBException if there is a error in the database.
   */
  Bug(SimpleWorld* sw, db::ID id);


  /**
   * The bug has been attacked.
   */
  virtual void attacked();


  SimpleWorld* world;
};

}

#endif // SIMPLEWORLD_BUG_HPP
