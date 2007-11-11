/**
 * @file db/bug.hpp
 * Information about a Bug.
 *
 * begin:     Mon, 01 Jan 2007 08:41:25 +0100
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

#ifndef __DB_BUG_HPP__
#define __DB_BUG_HPP__

#include <vector>

#include <db/alivebug.hpp>
#include <db/cpu.hpp>
#include <db/egg.hpp>

namespace SimpleWorld
{
namespace DB
{

/**
 * Information about a bug.
 */
class Bug: public AliveBug
{
public:
  /**
   * Constructor.
   * @param db database.
   * @param bug id of the bug.
   * @exception DBError if there is a error in the database.
   * @exception IDNotFound if the ID is not found in the table.
   */
  Bug(DB* db, ID id);

  /**
   * Constructor to convert a egg into a bug.
   * @param db database.
   * @param egg egg.
   * @exception DBError if there is a error in the database.
   */
  Bug(DB* db, Egg* egg);


  /**
   * Update the data of the class with the database.
   * changed is set to false.
   * The update() is propagated to the cpu.
   * @exception DBError if there is an error in the database.
   * @exception IDNotFound if the ID is not found in the table.
   */
  void update();

  /**
   * Update the database with the data of the class in changed or force are
   * true.
   * changed is set to false.
   * The update_db() is propagated to the cpu but without force. If the
   * update_db() in the cpu must be forced, a explicit call to
   * cpu::update_db(true) must be made.
   * @param force force the update of the database.
   * @exception DBError if there is an error in the database.
   */
  void update_db(bool force = false);

  /**
   * Remove the data from the database.
   * changed is set to false.
   * The remove() is propagated to the code.
   * @exception DBError if there is an error in the database.
   */
  void remove();


  // Data
  CPU cpu;
};

}
}

#endif // __DB_BUG_HPP__
