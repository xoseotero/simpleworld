/**
 * @file simpleworld/db/code.cpp
 * Code of a bug.
 *
 * begin:     Thu, 01 Mar 2007 13:00:43 +0100
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

#include <boost/format.hpp>

#include <simpleworld/config.hpp>

#include <cassert>

#include <sqlite3x/sqlite3x.hpp>

#include <simpleworld/cpu/types.hpp>
#include "exception.hpp"
#include "code.hpp"
#include "common.hpp"

namespace SimpleWorld
{
namespace DB
{

/**
 * Constructor.
 * @param db database.
 * @param bug_id id of the bug.
 * @exception DBException if there is a error in the database.
 * @exception DBException if the ID is not found in the table.
 */
Code::Code(DB* db, ID bug_id)
  : Table(db, bug_id), code(&this->changed)
{
  this->update();
}

/**
 * Constructor to insert data.
 * @param db database.
 * @exception DBException if there is a error in the database.
 */
Code::Code(DB* db)
  : Table(db), code(&this->changed)
{
}


/**
 * Search the mutations of the code.
 * @param db database.
 * @param mutations pointer to the mutations.
 * @param bug_id ID of the bug.
 */
static void init_mutations(std::vector<Mutation>* mutations,
                           DB* db, ID bug_id)
{
  mutations->clear();

  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
SELECT id\n\
FROM Mutation\n\
WHERE bug_id = ?;");
    sql.bind(1, bug_id);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    while (cursor.step())
      mutations->push_back(Mutation(db, cursor.getint64(0)));
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, e.what());
  }
}

/**
 * Propagate the update() to all the mutations.
 * @param mutations pointer to the mutations.
 */
static void update_mutations(std::vector<Mutation>* mutations)
{
  std::vector<Mutation>::iterator iter = mutations->begin();
  while (iter != mutations->end()) {
    (*iter).update();
    ++iter;
  }
}

/**
 * Propagate the update_db() to all the mutations.
 * @param mutations pointer to the mutations.
 */
static void update_db_mutations(std::vector<Mutation>* mutations)
{
  std::vector<Mutation>::iterator iter = mutations->begin();
  while (iter != mutations->end()) {
    (*iter).update_db();
    ++iter;
  }
}

/**
 * Propagate the insert() to all the mutations.
 * @param mutations pointer to the mutations.
 * @param bug_id ID of the bug.
 */
static void insert_mutations(std::vector<Mutation>* mutations, ID bug_id)
{
  std::vector<Mutation>::iterator iter = mutations->begin();
  while (iter != mutations->end()) {
    (*iter).insert(bug_id);
    ++iter;
  }
}

// It's not needed to propagate the remove() because the database do it
///**
// * Propagate the remove() to all the mutations.
// * @param mutations pointer to the mutations.
// */
//static void update_remove(std::vector<Mutation>* mutations)
//{
//  std::vector<Mutation>::iterator iter = mutations->begin();
//  while (iter != mutations->end()) {
//    (*iter).remove();
//    ++iter;
//  }
//}


/**
 * Update the data of the class with the database.
 * changed is set to false.
 * The update() is propagated to the mutations.
 * @exception DBException if there is a error in the database.
 * @exception DBException if the ID is not found in the table.
 */
void Code::update()
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT size, code\n\
FROM Code\n\
WHERE bug_id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
bug_id %1% not found in table Code")
                                              % this->id_));

    this->size = cursor.getint(0);
    this->code = get_memory(&cursor, 1);
    assert(this->code.size() == this->size);

    sqlite3x::sqlite3_command sql_mutations(*this->db_,
                                            "\
SELECT id FROM Mutation\n\
WHERE bug_id = ?;");
    sqlite3x::sqlite3_cursor cursor_mutations(sql_mutations.executecursor());
    while (cursor_mutations.step())
      this->mutations.push_back(Mutation(this->db_,
                                         cursor_mutations.getint64(0)));
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, e.what());
  }


  init_mutations(&this->mutations, this->db_, this->id_);
  update_mutations(&this->mutations);

  Table::update();
}

/**
 * Update the database with the data of the class in changed or force are
 * true.
 * changed is set to false.
 * The update_db() is propagated to the mutations but without force. If
 * the update_db() in the mutations must be forced, a explicit call to the
 * mutations::update_db(true) must be made.
 * @param force force the update of the database.
 * @exception DBException if there is a error in the database.
 */
void Code::update_db(bool force)
{
  if (this->changed or force) {
    sqlite3x::sqlite3_command sql(*this->db_);

    try {
      sql.prepare("\
UPDATE Code\n\
SET size = ?, code = ?\n\
WHERE bug_id = ?;");
      sql.bind(1, static_cast<int>(this->size));
      bind_memory(&sql, 2, this->code);
      sql.bind(3, this->id_);

      sql.executenonquery();
    } catch (const sqlite3x::database_error& e) {
      throw EXCEPTION(DBException, e.what());
    }
  }


  update_db_mutations(&this->mutations);

  Table::update_db(force);
}

/**
 * Insert the data in the database with a specific id.
 * The ID is updated.
 * changed is set to false.
 * The insert() is propagated to the mutations.
 * @param bug_id id of the bug.
 * @exception DBException if there is an error in the database.
 */
void Code::insert(ID bug_id)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
INSERT INTO Code(bug_id, size, code)\n\
VALUES(?, ?, ?);");
    sql.bind(1, bug_id);
    sql.bind(2, static_cast<int>(this->size));
    bind_memory(&sql, 3, this->code);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, e.what());
  }


  insert_mutations(&this->mutations, bug_id);

  Table::insert(bug_id);
}

/**
 * Remove the data from the database.
 * changed is set to false.
 * The remove() is propagated to the mutations.
 * @exception DBException if there is an error in the database.
 */
void Code::remove()
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
DELETE FROM Code\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, e.what());
  }


  // It's not needed to propagate the remove() because the database do it
  //remove_mutations(&this->mutations);

  Table::remove();
}

}
}
