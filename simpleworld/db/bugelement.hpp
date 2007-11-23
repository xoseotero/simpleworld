/**
 * @file simpleworld/db/bugelement.hpp
 * A bug on the World.
 *
 * begin:     Thu, 16 Aug 2007 21:51:46 +0200
 * last:      $Date$
 *
 *  Copyright (C) 2007  Xosé Antón Otero Ferreira <xoseotero@gmail.com>
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

#ifndef SIMPLEWORLD_DB_BUGELEMENT_HPP
#define SIMPLEWORLD_DB_BUGELEMENT_HPP

#include <simpleworld/types.hpp>
#include <simpleworld/element.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/table.hpp>
#include <simpleworld/db/code.hpp>

namespace SimpleWorld
{
namespace DB
{

/**
 * A bug on the World.
 */
class BugElement: public Element, public Table
{
public:
  /**
   * Constructor.
   * @param db database.
   * @param id id of the bug.
   * @param type type of element
   * @param position position of the element.
   */
  BugElement(DB* db, ID id, ElementType type, Position position);

  /**
   * Constructor to insert data.
   * @param db database.
   * @param type type of element
   */
  BugElement(DB* db, ElementType type);


  /**
   * Update the data of the class with the database.
   * changed is set to false.
   * The update() is propagated to the code.
   * @exception DBException if there is a error in the database.
   * @exception DBException if the ID is not found in the table.
   */
  void update();

  /**
   * Update the database with the data of the class in changed or force are
   * true.
   * changed is set to false.
   * The update_db() is propagated to the code but without force. If the
   * update_db() in the code must be forced, a explicit call to
   * code::update_db(true) must be made.
   * @param force force the update of the database.
   * @exception DBException if there is a error in the database.
   */
  void update_db(bool force = false);

  /**
   * Insert the data in the database.
   * The ID is updated.
   * changed is set to false.
   * The insert() is propagated to the code.
   * @exception DBException if there is an error in the database.
   */
  void insert();

  /**
   * Remove the data from the database.
   * changed is set to false.
   * The remove() is propagated to the code.
   * @exception DBException if there is an error in the database.
   */
  void remove();


  // Data
  Orientation orientation;

  Time birth;

  ID father_id;

  Code code;
};

}
}

#endif // SIMPLEWORLD_DB_BUGELEMENT_HPP
