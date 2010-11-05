/**
 * @file simpleworld/bug.hpp
 * A bug in Simple World.
 *
 *  Copyright (C) 2007-2010  Xosé Otero <xoseotero@gmail.com>
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

#include <simpleworld/element.hpp>
#include <simpleworld/dbmemory.hpp>
#include <simpleworld/cpu.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/bug.hpp>
#include <simpleworld/db/alivebug.hpp>
#include <simpleworld/db/world.hpp>

namespace simpleworld
{

class SimpleWorld;

class Bug: public Element, public db::Bug, public db::AliveBug,
           public db::World
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
   * The id of the table.
   * @return the ID.
   */
  db::ID id() const { return db::Bug::id_; }
  void id(db::ID id) { db::Bug::id(id); }


  db::ID world_id() const { return db::AliveBug::world_id(); }
  void world_id(db::ID world_id) {
    db::AliveBug::world_id(world_id);
    db::World::id_ = world_id;
  }


  /**
   * The bug has been attacked.
   */
  virtual void attacked();

  /**
   * The code of the bug has been mutated.
   */
  virtual void mutated();


  /**
   * Check if colname is NULL.
   * @param colname name of the column.
   * @return true if colname is NULL, else false.
   */
  bool is_null(const std::string& colname) const;

  /**
   * Set colname as NULL.
   * @param colname name of the column.
   */
  void set_null(const std::string& colname);


  SimpleWorld* world;

  DBMemory regs;
  DBMemory mem;
  CPU cpu;
};

}

#endif // SIMPLEWORLD_BUG_HPP
