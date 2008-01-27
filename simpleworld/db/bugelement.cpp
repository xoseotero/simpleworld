/**
 * @file simpleworld/db/bugelement.cpp
 * A bug on the World.
 *
 * begin:     Thu, 06 Sep 2007 04:42:47 +0200
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

#include "bugelement.hpp"

namespace SimpleWorld
{
namespace DB
{

/**
 * Constructor.
 * @param db database.
 * @param id id of the bug.
 * @param type type of element
 * @param position position of the element.
 */
BugElement::BugElement(DB* db, ID id, ElementType type, Position position)
  : Element(type, position), Table(db, id), code(db, id)
{
  this->update();
}

/**
 * Constructor to insert data.
 * @param db database.
 * @param type type of element
 */
BugElement::BugElement(DB* db, ElementType type)
  // The position at this moment is unknown
  : Element(type, this->position), Table(db), code(db)
{
}


/**
 * Update the data of the class with the database.
 * changed is set to false.
 * The update() is propagated to the code.
 * @exception DBException if there is a error in the database.
 * @exception DBException if the ID is not found in the table.
 */
void BugElement::update()
{
  this->code.update();


  Table::update();
}

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
void BugElement::update_db(bool force)
{
  this->code.update_db();


  Table::update_db();
}

/**
 * Insert the data in the database.
 * The ID is updated.
 * changed is set to false.
 * The insert() is propagated to the code.
 * @exception DBException if there is an error in the database.
 */
void BugElement::insert()
{
  this->code.insert(this->id_);


  Table::insert();
}

/**
 * Remove the data from the database.
 * changed is set to false.
 * The remove() is propagated to the code.
 * @exception DBException if there is an error in the database.
 */
void BugElement::remove()
{
  // It's not needed to propagate the remove() because the database do it
  // this->code.remove();


  Table::remove();
}

}
}
