/**
 * @file db/mutations.h
 * Mutations of a bug.
 *
 * begin:     Thu, 01 Mar 2007 16:59:31 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2007, Xosé Otero <xoseotero@users.sourceforge.net>
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

#ifndef __DB_MUTATIONS_H__
#define __DB_MUTATIONS_H__

#include <vector>

#include <simple/types.hpp>
#include <db/types.hpp>
#include <db/db.hpp>
#include <db/table.hpp>

namespace SimpleWorld
{
namespace DB
{

/**
 * Mutations of a bug.
 */
class Mutations: public Table
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
  Mutations(DB* db, ID bug_id);

public:
  /**
   * Destructor.
  */
  ~Mutations();


  /**
   * Structure for each mutation in the code.
   */
  struct Mutation {
    Uint16 position;
    enum {
      mutation,
      addition,
      deletion
    } type;
    // If type is addition, then the value of original is garbage.
    Word original;
    // If type is deletion, then the value of mutated is garbage.
    Word mutated;
  };

  // Data
  std::vector<Mutation> mutations;


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

#endif // __DB_MUTATIONS_H__

