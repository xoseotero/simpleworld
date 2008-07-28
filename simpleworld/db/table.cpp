/**
 * @file simpleworld/db/table.cpp
 * Base class for the tables.
 *
 * begin:     Tue, 13 Feb 2007 11:20:17 +0100
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

#include "table.hpp"

namespace simpleworld
{
namespace db
{

/**
 * Constructor.
 * @param db database.
 * @param id id of the row.
 * @exception DBException if there is a error in the database.
 * @exception DBException if the ID is not found in the table.
 */
Table::Table(DB* db, ID id)
  : db_(db), id_(id)
{
  this->update();
}

/**
 * Constructor to insert data.
 * insert(id) must be called before any call to update(), update_db() or
 * remove().
 * inserted is set to false.
 * @param db database.
 * @exception DBException if there is a error in the database.
 */
Table::Table(DB* db)
  : inserted(false), db_(db)
{
}

/**
 * Destructor.
 */
Table::~Table()
{
}


/**
 * Update the data of the class with the database.
 * inserted is set to true.
 * changed is set to false.
 * @exception DBException if there is a error in the database.
 * @exception DBException if the ID is not found in the table.
 */
void Table::update()
{
  this->inserted = true;
  this->changed = false;
}

/**
 * Update the database with the data of the class in changed or force are
 * true.
 * changed is set to false.
 * @param force force the update of the database.
 * @exception DBException if there is a error in the database.
 */
void Table::update_db(bool force)
{
  this->changed = false;
}

/**
 * Insert the data in the database.
 * The ID is updated.
 * inserted is set to true.
 * changed is set to false.
 * @exception DBException if there is an error in the database.
 */
void Table::insert()
{
  this->inserted = true;
  this->changed = false;
}

/**
 * Insert the data in the database with a specific id.
 * The ID is updated.
 * inserted is set to true.
 * changed is set to false.
 * @param id id of the row.
 * @exception DBException if there is an error in the database.
 */
void Table::insert(ID id)
{
  this->id_ = id;

  this->inserted = true;
  this->changed = false;
}

/**
 * Remove the data from the database.
 * inserted is set to false.
 * changed is set to false.
 * @exception DBException if there is an error in the database.
 */
void Table::remove()
{
  this->inserted = false;
  this->changed = false;
}

}
}
