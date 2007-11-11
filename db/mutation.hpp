/**
 * @file db/mutation.hpp
 * A mutation of a bug.
 *
 * begin:     Thu, 01 Mar 2007 16:59:31 +0100
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

#ifndef __DB_MUTATION_HPP__
#define __DB_MUTATION_HPP__

#include <vector>

#include <simple/types.hpp>
#include <cpu/types.hpp>
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
class Mutation: public Table
{
public:
  /**
   * Constructor.
   * @param db database.
   * @param id id of the mutation.
   * @exception DBError if there is a error in the database.
   * @exception IDNotFound if the ID is not found in the table.
   */
  Mutation(DB* db, ID id);

  /**
   * Constructor to insert data.
   * @param db database.
   * @exception DBError if there is a error in the database.
   */
  Mutation(DB* db);


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
  Uint16 position;

  enum {
    mutation,
    addition,
    deletion
  } type;

  // If type is addition, then the value of original is garbage.
  CPU::Word original;
  // If type is deletion, then the value of mutated is garbage.
  CPU::Word mutated;

  ID bug_id;
};

}
}

#endif // __DB_MUTATION_HPP__