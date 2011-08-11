/**
 * @file simpleworld/db/mutation.hpp
 * A mutation of a bug.
 *
 *  Copyright (C) 2007-2011  Xosé Otero <xoseotero@gmail.com>
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

#ifndef SIMPLEWORLD_DB_MUTATION_HPP
#define SIMPLEWORLD_DB_MUTATION_HPP

#include <simpleworld/ints.hpp>
#include <simpleworld/types.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/db.hpp>
#include <simpleworld/db/table.hpp>

namespace simpleworld
{
namespace db
{

/**
 * Mutations of a bug.
 */
class Mutation: public Table
{
public:
  /**
   * Constructor.
   * It's not checked if the id is in the table, only when accessing the data
   * the id is checked.
   * @param db database.
   * @param id id of the mutation.
   */
  Mutation(DB* db, ID id);


  /**
   * Insert a mutation of code.
   * @param db database.
   * @param bug_id id of the bug.
   * @param time when the mutation happened.
   * @param position where the mutation happened.
   * @param original the previous value of the code.
   * @param mutated the new value of the code.
   * @return the id of the new row.
   * @exception DBException if there is an error with the insertion.
   */
  static ID insert(DB* db, ID bug_id, Time time, Uint32 position,
                   Uint32 original, Uint32 mutated);

  /**
   * Insert a addition of code.
   * @param db database.
   * @param bug_id id of the bug.
   * @param time when the mutation happened.
   * @param position where the mutation happened.
   * @param mutated the new word.
   * @return the id of the new row.
   * @exception DBException if there is an error with the insertion.
   */
  static ID insert_addition(DB* db, ID bug_id, Time time, Uint32 position,
                            Uint32 mutated);

  /**
   * Insert a deletion of code.
   * @param db database.
   * @param bug_id id of the bug.
   * @param time when the mutation happened.
   * @param position where the mutation happened.
   * @param original the deleted word.
   * @return the id of the new row.
   * @exception DBException if there is an error with the insertion.
   */
  static ID insert_deletion(DB* db, ID bug_id, Time time, Uint32 position,
                            Uint32 original);

  /**
   * Delete a mutation.
   * @param db database.
   * @param id id of the mutation.
   * @exception DBException if there is an error with the deletion.
   */
  static void remove(DB* db, ID id);


  /**
   * Get the id of the mutation.
   * @return the id of the mutation.
   * @exception DBException if there is an error with the query.
   */
  ID id() const { return db::Table::id(); }

  /**
   * Set the id of the mutation.
   * @param id the new id.
   * @exception DBException if there is an error with the update.
   */
  void id(ID id);


  /**
   * Get the id of the bug.
   * @return the id of the bug.
   * @exception DBException if there is an error with the query.
   */
  ID bug_id() const;

  /**
   * Set the id of the bug.
   * @param bug_id the new id.
   * @exception DBException if there is an error with the update.
   */
  void bug_id(ID bug_id);


  /**
   * Get when the mutation happened.
   * @return the time.
   * @exception DBException if there is an error with the query.
   */
  Time time() const;

  /**
   * Set when the mutation happened.
   * @param time the new time.
   * @exception DBException if there is an error with the update.
   */
  void time(Time time);


  /**
   * Get where the mutation happened.
   * @return the position.
   * @exception DBException if there is an error with the query.
   */
  Uint32 position() const;

  /**
   * Set where the mutation happened.
   * @param position the new position.
   * @exception DBException if there is an error with the update.
   */
  void position(Uint32 position);

  /**
   * Get the previous value of the code.
   * original is NULL if the mutation is the addition of code.
   * @return the previous value.
   * @exception DBException if there is an error with the query.
   */
  Uint32 original() const;

  /**
   * Set the previous value of the code.
   * @param original the previous value.
   * @exception DBException if there is an error with the query.
   */
  void original(Uint32 original);

  /**
   * Get the new value of the code.
   * mutated is NULL if the mutation is the deletion of code.
   * @return the new value.
   * @exception DBException if there is an error with the query.
   */
  Uint32 mutated() const;

  /**
   * Set the new value of the code.
   * @param mutated the new value.
   * @exception DBException if there is an error with the query.
   */
  void mutated(Uint32 mutated);
};

}
}

#endif // SIMPLEWORLD_DB_MUTATION_HPP
