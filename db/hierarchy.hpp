/**
 * @file db/hierarchy.h
 * Hierarchy of a bug.
 *
 * begin:     Wed, 21 Feb 2007 11:32:42 +0100
 * last:      $Date$
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

#ifndef __DB_HIERARCHY_H__
#define __DB_HIERARCHY_H__

#include <vector>

#include <db/types.hpp>
#include <db/db.hpp>
#include <db/table.hpp>

namespace SimpleWorld
{
namespace DB
{

/**
 * Hierarchy of a bug.
 */
class Hierarchy: public Table
{
  friend class DB;

protected:
  /**
   * Constructor.
   * @param db database.
   * @param bug_id id of the bug.
   * @exception DBError if there is a error in the database.
   * @exception IDNotFound if the ID is not found in the table.
   */
  Hierarchy(DB* db, ID bug_id);

public:
  /**
   * Destructor.
  */
  ~Hierarchy();


  // Data
  std::vector<ID> parents;
  std::vector<ID> sons;


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

#endif // __DB_HIERARCHY_H__
