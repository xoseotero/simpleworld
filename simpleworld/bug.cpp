/**
 * @file simpleworld/bug.cpp
 * A bug in Simple World.
 *
 *  Copyright (C) 2007-2011  Xosé Otero <xoseotero@gmail.com>
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

#include <cassert>

#include "config.hpp"

#ifdef DEBUG
#include <iostream>

#include <boost/format.hpp>
#endif // DEBUG

#include "simpleworld.hpp"
#include "types.hpp"
#include "bug.hpp"
#include "cpu/types.hpp"
#include "db/bug.hpp"

namespace simpleworld
{

/**
 * Constructor.
 * @param sw world where the bug lives.
 * @param id id of the bug.
 * @exception DBException if there is a error in the database.
 */
Bug::Bug(SimpleWorld* sw, db::ID id)
  : Element(ElementBug), db::Bug(sw, id), db::AliveBug(sw, id),
    db::World(sw, this->world_id()), world(sw),
    regs(db::AliveBug::registers()), mem(db::Bug::code()),
    cpu(&this->regs, &this->mem, this)
{
}


/**
 * The bug has been attacked.
 */
void Bug::attacked()
{
#ifdef DEBUG
  std::cout << boost::str(boost::format("\
Bug[%1%] attacked")
                          % db::Bug::id_)
    << std::endl;
#endif // DEBUG

  this->cpu.interrupt(INTERRUPT_WORLDEVENT, EventAttack);
}

/**
 * The code of the bug has been mutated.
 */
void Bug::mutated()
{
#ifdef DEBUG
  std::cout << boost::str(boost::format("\
Bug[%1%] mutated")
                          % db::Bug::id_)
    << std::endl;
#endif // DEBUG

  // The content of the memory has changed and must be updated
  this->mem.update();
  this->cpu.interrupt(INTERRUPT_WORLDEVENT, EventMutation);
}


/**
 * Check if colname is NULL.
 * @param colname name of the column.
 * @return true if colname is NULL, else false.
 */
bool Bug::is_null(const std::string& colname) const
{
  if (colname == "father_id")
    return db::Bug::is_null(colname);
  else if (colname == "time_last_action" or colname == "action_time")
    return db::AliveBug::is_null(colname);
  else if (colname == "orientation")
    return db::World::is_null(colname);
  else
    return false;
}

/**
 * Set colname as NULL.
 * @param colname name of the column.
 */
void Bug::set_null(const std::string& colname)
{
  if (colname == "father_id")
    db::Bug::set_null(colname);
  else if (colname == "time_last_action" or colname == "action_time")
    db::AliveBug::set_null(colname);
  else if (colname == "orientation")
    db::World::set_null(colname);
}

}
