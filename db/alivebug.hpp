/**
 * @file db/alivebug.hpp
 * Information about a alive bug.
 *
 * begin:     Wed, 15 Aug 2007 13:56:05 +0200
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

#ifndef __DB_ALIVEBUG_HPP__
#define __DB_ALIVEBUG_HPP__

#include <vector>

#include <simpleworld/element.hpp>
#include <db/types.hpp>
#include <db/db.hpp>
#include <db/bugelement.hpp>

namespace SimpleWorld
{
namespace DB
{

/**
 * Information about a bug.
 */
class AliveBug: public BugElement
{
public:
  /**
   * Constructor.
   * @param db database.
   * @param id id of the bug.
   * @param type type of element
   * @param position position of the bug.
   * @exception DBError if there is a error in the database.
   * @exception IDNotFound if the ID is not found in the table.
   */
  AliveBug(DB* db, ID id, ElementType type, Position position);

  /**
   * Constructor to insert data.
   * @param db database.
   * @param type type of element
   * @exception DBError if there is a error in the database.
   */
  AliveBug(DB* db, ElementType type);


  // Data
  Energy energy;
};

}
}

#endif // __DB_ALIVEBUG_HPP__
