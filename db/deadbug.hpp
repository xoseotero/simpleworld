/**
 * @file db/deadbug.hpp
 * Information about a dead bug.
 *
 * begin:     Wed, 15 Aug 2007 13:49:28 +0200
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

#ifndef __DB_DEADBUG_HPP__
#define __DB_DEADBUG_HPP__

#include <vector>

#include <db/types.hpp>
#include <db/db.hpp>
#include <db/bugelement.hpp>
#include <db/egg.hpp>
#include <db/bug.hpp>

namespace SimpleWorld
{
namespace DB
{

/**
 * Information about a bug.
 */
class DeadBug: public BugElement
{
public:
  /**
   * Constructor.
   * @param db database.
   * @param bug id of the bug.
   * @exception DBError if there is a error in the database.
   * @exception IDNotFound if the ID is not found in the table.
   */
  DeadBug(DB* db, ID id);

  /**
   * Constructor to convert a egg into a dead bug.
   * @param db database.
   * @param egg egg.
   * @exception DBError if there is a error in the database.
   */
  DeadBug(DB* db, Egg* egg);

  /**
   * Constructor to convert a bug into a dead bug.
   * @param db database.
   * @param bug bug.
   * @exception DBError if there is a error in the database.
   */
  DeadBug(DB* db, Bug* bug);


  /**
   * Update the data of the class with the database.
   * changed is set to false.
   * @exception DBError if there is an error in the database.
   * @exception IDNotFound if the ID is not found in the table.
   */
  void update();

  /**
   * Update the database with the data of the class in changed or force are
   * true.
   * changed is set to false.
   * @param force force the update of the database.
   * @exception DBError if there is an error in the database.
   */
  void update_db(bool force = false);

  /**
   * Remove the data from the database.
   * changed is set to false.
   * @exception DBError if there is an error in the database.
   */
  void remove();


  // Data
  Time dead;

  ID killer_id;
};

}
}

#endif // __DB_DEADBUG_HPP__
