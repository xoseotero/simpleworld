/**
 * @file simpleworld/db/cpu.hpp
 * CPU of a bug.
 *
 * begin:     Mon, 01 Jan 2007 08:46:13 +0100
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

#ifndef __SIMPLEWORLD_DB_CPU_HPP__
#define __SIMPLEWORLD_DB_CPU_HPP__

#include <string>

#include <simpleworld/db/types.hpp>
#include <simpleworld/db/memory.hpp>
#include <simpleworld/db/db.hpp>
#include <simpleworld/db/table.hpp>

namespace SimpleWorld
{
namespace DB
{

/**
 * CPU of a bug.
 */
class CPU: public Table
{
public:
  /**
   * Constructor.
   * @param db database.
   * @param bug_id id of the bug.
   * @exception DBError if there is a error in the database.
   * @exception IDNotFound if the ID is not found in the table.
   */
  CPU(DB* db, ID bug_id);

  /**
   * Constructor to insert data.
   * @param db database.
   * @exception DBError if there is a error in the database.
   */
  CPU(DB* db);


  /**
   * Update the data of the class with the database.
   * changed is set to false.
   * @exception DBError if there is a error in the database.
   * @execption IDNotFound if the ID is not found in the table.
   */
  void update();

  /**
   * Update the database with the data of the class in changed or force are
   * true.
   * changed is set to false.
   * @param force force the update of the database.
   * @exception DBError if there is a error in the database.
   */
  void update_db(bool force = false);

  /**
   * Insert the data in the database with a specific id.
   * The ID is updated.
   * changed is set to false.
   * @param id id of the row.
   * @exception DBError if there is an error in the database.
   */
  void insert(ID bug_id);

  /**
   * Remove the data from the database.
   * changed is set to false.
   * @exception DBError if there is an error in the database.
   */
  void remove();


  // Data
  Memory registers;
};

}
}

#endif // __SIMPLEWORLD_DB_CPU_HPP__
