/**
 * @file simpleworld/db/bug.cpp
 * Information about a bug.
 *
 *  Copyright (C) 2007-2010  Xosé Otero <xoseotero@gmail.com>
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

#include <sqlite3x.hpp>

#include "exception.hpp"
#include "bug.hpp"

namespace simpleworld
{
namespace db
{

/**
 * Constructor.
 * It's not checked if the id is in the table, only when accessing the data
 * the id is checked.
 * @param db database.
 * @param bug_id id of the bug.
 */
Bug::Bug(DB* db, ID id)
  : Table("Bug", db, id)
{
}


/**
 * Insert a bug with father.
 * @param db database.
 * @param father_id id of the father.
 * @param code code of the bug.
 * @param size size of the code.
 * @return the id of the new row.
 * @exception DBException if there is an error with the insertion.
 */
ID Bug::insert(DB* db, ID father_id, const void* code, Uint32 size)
{
  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
INSERT INTO Bug(father_id, code)\n\
VALUES(?, ?);");
    sql.bind(1, static_cast<sqlite3x::int64_t>(father_id));
    sql.bind(2, code, size);

    sql.executenonquery();
    return db->insertid();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + db->errormsg() + ")");
  }
}

/**
 * Insert a bug without father.
 * @param db database.
 * @param code code of the bug.
 * @param size size of the code.
 * @return the id of the new row.
 * @exception DBException if there is an error with the insertion.
 */
ID Bug::insert(DB* db, const void* code, Uint32 size)
{
  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
INSERT INTO Bug(code)\n\
VALUES(?);");
    sql.bind(1, code, size);

    sql.executenonquery();
    return db->insertid();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + db->errormsg() + ")");
  }
}

/**
 * Delete a bug.
 * @param db database.
 * @param id id of the bug.
 * @exception DBException if there is an error with the deletion.
 */
void Bug::remove(DB* db, ID id)
{
  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
DELETE FROM Bug\n\
WHERE id = ?;");
    sql.bind(1, id);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + db->errormsg() + ")");
  }
}


/**
 * Set the id of the bug.
 * @param id the new id.
 * @exception DBException if there is an error with the update.
 */
void Bug::id(ID id)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Bug\n\
SET id = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<sqlite3x::int64_t>(id));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }

  this->id_ = id;
}


/**
 * Get the id of the father.
 * father_id is NULL if the bug hasn't got a father.
 * @return the id of the father.
 * @exception DBException if there is an error with the query.
 */
ID Bug::father_id() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT father_id\n\
FROM Bug\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Bug")
                                              % this->id_));

    return cursor.getint64(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set the id of the father.
 * @param father_id the id of the father.
 * @exception DBException if there is an error with the query.
 */
void Bug::father_id(ID father_id)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Bug\n\
SET father_id = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<sqlite3x::int64_t>(father_id));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}


/**
 * Get the code of the bug.
 * @return the code of the bug.
 * @exception DBException if there is an error with the query.
 */
Blob Bug::code() const
{
  return Blob(this->db_, "Bug", "code", this->id_);
}


/**
 * Get the mutations of the bug.
 * @return the mutations.
 * @exception DBException if there is an error with the query.
 */
std::vector<ID> Bug::mutations() const
{
  std::vector<ID> ids;
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT id\n\
FROM Mutation\n\
WHERE bug_id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor = sql.executecursor();
    while (cursor.step())
      ids.push_back(cursor.getint64(0));
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }

  return ids;
}

}
}
