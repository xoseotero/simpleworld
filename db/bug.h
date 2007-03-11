/**
 * @file db/bug.h
 * Information about a Bug.
 *
 * begin:     Mon, 01 Jan 2007 08:41:25 +0100
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2007, Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __DB_BUG_H__
#define __DB_BUG_H__

#include <vector>

#include <db/types.h>
#include <db/db.h>
#include <db/table.h>

namespace SimpleWorld
{
namespace DB
{

/**
 * Information about a bug.
 */
class Bug: public Table
{
  friend class DB;

protected:
  /**
   * Constructor.
   * @param db database.
   * @param bug id of the bug.
   * @exception DBError if there is a error in the database.
   * @exception IDNotFound if the ID is not found in the table.
   */
  Bug(DB* db, ID id);

public:
  /**
   * Destructor.
   */
  ~Bug();


  // Data
  SortOrder order_world;

  Energy energy;

  Position position_x;
  Position position_y;
  Orientation orientation;

  Time birth;
  Time dead;

  ID killer;


  /**
   * Update the data of the class with the database.
   * @exception DBError if there is a error in the database.
   * @execption IDNotFound if the ID is not found in the table.
   */
  void update();

  /**
   * Update the database with the data of the class.
   * @exception DBError if there is a error in the database.
   */
  void update_db();
};

}
}

#endif // __DB_BUG_H__
