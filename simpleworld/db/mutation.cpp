/**
 * @file simpleworld/db/mutation.cpp
 * A mutation of a bug.
 *
 *  Copyright (C) 2007-2008  Xosé Otero <xoseotero@users.sourceforge.net>
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

#include <string>

#include <boost/format.hpp>

#include "exception.hpp"
#include "mutation.hpp"

namespace simpleworld
{
namespace db
{

/**
 * Constructor.
 * @param db database.
 * @param id id of the mutation.
 * @exception DBException if there is a error in the database.
 * @exception DBException if the ID is not found in the table.
 */
Mutation::Mutation(DB* db, ID bug_id)
  : Table(db, bug_id)
{
  this->update();
}

/**
 * Constructor to insert data.
 * @param db database.
 * @exception DBException if there is a error in the database.
 */
Mutation::Mutation(DB* db)
  : Table(db)
{
}


/**
 * Update the data of the class with the database.
 * changed is set to false.
 * @exception DBException if there is a error in the database.
 * @exception DBException if the ID is not found in the table.
 */
void Mutation::update()
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT time, position, original, mutated, bug_id\n\
FROM Mutation\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Mutation")
                                              % this->id_));

    this->time = cursor.getint(0);
    this->position = cursor.getint(1);
    this->original = cursor.getint(2);
    this->mutated = cursor.getint(3);
    if (cursor.isnull(2)) {
      this->add_null("original");
      this->type = Mutation::addition;
    } else if (cursor.isnull(3)) {
      this->add_null("mutated");
      this->type = Mutation::deletion;
    } else
      this->type = Mutation::mutation;
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }


  Table::update();
}

/**
 * Update the database with the data of the class in changed or force are
 * true.
 * changed is set to false.
 * @param force force the update of the database.
 * @exception DBException if there is a error in the database.
 */
void Mutation::update_db(bool force)
{
  if (this->changed or force) {
    sqlite3x::sqlite3_command sql(*this->db_);

    try {
      sql.prepare("\
UPDATE Mutation\n\
SET time = ?, position = ?, original = ?, mutated = ?, bug_id = ?\n\
WHERE id = ?;");
      sql.bind(1, static_cast<int>(this->time));
      sql.bind(2, static_cast<int>(this->position));
      if (this->type == Mutation::addition)
        sql.bind(3);
      else
        sql.bind(3, static_cast<int>(this->original));
      if (this->type == Mutation::deletion)
        sql.bind(4);
      else
        sql.bind(4, static_cast<int>(this->mutated));
      sql.bind(5, static_cast<int>(this->bug_id));
      sql.bind(6, this->id_);

      sql.executenonquery();
    } catch (const sqlite3x::database_error& e) {
      throw EXCEPTION(DBException, std::string(e.what()) +
                      " (" + this->db()->errormsg() + ")");
    }
  }


  Table::update_db(force);
}

/**
 * Insert the data in the database with a specific id.
 * The ID is updated.
 * changed is set to false.
 * @param bug_id id of the bug.
 * @exception DBException if there is an error in the database.
 */
void Mutation::insert(ID bug_id)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
INSERT INTO Mutation(time, position, original, mutated, bug_id)\n\
VALUES(?, ?, ?, ?, ?);");
    sql.bind(1, static_cast<int>(this->time));
    sql.bind(2, static_cast<int>(this->position));
    if (this->type == Mutation::addition)
      sql.bind(3);
    else
      sql.bind(3, static_cast<int>(this->original));
    if (this->type == Mutation::deletion)
      sql.bind(4);
    else
      sql.bind(4, static_cast<int>(this->mutated));
    sql.bind(5, bug_id);

    sql.executenonquery();
    this->id_ = this->db_->insertid();
    this->bug_id = bug_id;
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }


  Table::insert();
}

/**
 * Remove the data from the database.
 * changed is set to false.
 * @exception DBException if there is an error in the database.
 */
void Mutation::remove()
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
DELETE FROM Mutation\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }


  Table::remove();
}

}
}
