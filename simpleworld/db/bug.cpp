/**
 * @file simpleworld/db/bug.cpp
 * Information about a bug.
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

#include <stack>

#include <boost/format.hpp>

#include <sqlite3.h>

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
 * @param id id of the bug.
 */
Bug::Bug(DB* db, ID id)
  : Table("Bug", db, id)
{
}


/**
 * Insert a bug with father.
 * @param db database.
 * @param creation when the egg was created.
 * @param father_id id of the father.
 * @param code code of the bug.
 * @param size size of the code.
 * @return the id of the new row.
 * @exception DBException if there is an error with the insertion.
 */
ID Bug::insert(DB* db, Time creation, ID father_id,
               const void* code, Uint32 size)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
INSERT INTO Bug(creation, father_id, code)\n\
VALUES(?, ?, ?);", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_bind_int(stmt, 1, creation);
  sqlite3_bind_int64(stmt, 2, father_id);
  sqlite3_bind_blob(stmt, 3, code, size, SQLITE_TRANSIENT);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_finalize(stmt);

  return sqlite3_last_insert_rowid(db->db());
}

/**
 * Insert a bug without father.
 * @param db database.
 * @param creation when the egg was created.
 * @param code code of the bug.
 * @param size size of the code.
 * @return the id of the new row.
 * @exception DBException if there is an error with the insertion.
 */
ID Bug::insert(DB* db, Time creation, const void* code, Uint32 size)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
INSERT INTO Bug(creation, code)\n\
VALUES(?, ?);", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_bind_int(stmt, 1, creation);
  sqlite3_bind_blob(stmt, 2, code, size, SQLITE_TRANSIENT);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_finalize(stmt);

  return sqlite3_last_insert_rowid(db->db());
}

/**
 * Delete a bug.
 * @param db database.
 * @param id id of the bug.
 * @exception DBException if there is an error with the deletion.
 */
void Bug::remove(DB* db, ID id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
DELETE FROM Bug\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_bind_int64(stmt, 1, id);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_finalize(stmt);
}


/**
 * Set the id of the bug.
 * @param id the new id.
 * @exception DBException if there is an error with the update.
 */
void Bug::id(ID id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Bug\n\
SET id = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, id);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);

  this->id_ = id;
}


/**
* Get when the egg was created.
* @return the time.
* @exception DBException if there is an error with the query.
*/
Time Bug::creation() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT creation\n\
FROM Bug\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Bug")
                                            % this->id_));
  Time creation = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return creation;
}

/**
* Set when the egg was created.
* @param creation the new time.
* @exception DBException if there is an error with the update.
*/
void Bug::creation(Time creation)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Bug\n\
SET creation = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, creation);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
 * Get the id of the father.
 * father_id is NULL if the bug hasn't got a father.
 * @return the id of the father.
 * @exception DBException if there is an error with the query.
 */
ID Bug::father_id() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT father_id\n\
FROM Bug\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Bug")
                                            % this->id_));
  ID id = sqlite3_column_int64(stmt, 0);
  sqlite3_finalize(stmt);

  return id;
}

/**
 * Set the id of the father.
 * @param father_id the id of the father.
 * @exception DBException if there is an error with the query.
 */
void Bug::father_id(ID father_id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Bug\n\
SET father_id = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, father_id);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

/**
* Get the ancestors of the bug.
* @return the ancestors.
* @exception DBException if there is an error with the query.
*/
std::vector<ID> Bug::ancestors() const
{
  std::vector<ID> ids;

  if (not this->is_null("father_id")) {
    Bug father(this->db_, this->father_id());
    std::vector<ID> father_ancestors = father.ancestors();
    ids.insert(ids.end(), father_ancestors.begin(), father_ancestors.end());
    ids.push_back(father.id());
  }

  return ids;
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
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT id\n\
FROM Mutation\n\
WHERE bug_id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);

  bool done = false;
  std::vector<ID> ids;
  while (not done)
    switch (sqlite3_step(stmt)) {
    case SQLITE_DONE:
      done = true;
      break;
    case SQLITE_ROW:
      ids.push_back(sqlite3_column_int64(stmt, 0));
      break;
    default:
      throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
    }

  sqlite3_finalize(stmt);

  return ids;
}

/**
* Get the mutations of the ancestors of the bug and the bug.
* @return the mutations.
* @exception DBException if there is an error with the query.
*/
std::vector<ID> Bug::all_mutations() const
{
  std::vector<ID> ids;

  std::vector<ID> ancestors = this->ancestors();
  std::vector<ID>::const_iterator iter = ancestors.begin();
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT id\n\
FROM Mutation\n\
WHERE bug_id = ? AND time < ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 2, this->creation());
  while (iter != ancestors.end()) {
    sqlite3_bind_int64(stmt, 1, *iter);

    bool done = false;
    while (not done)
      switch (sqlite3_step(stmt)) {
      case SQLITE_DONE:
        done = true;
        break;
      case SQLITE_ROW:
        ids.push_back(sqlite3_column_int64(stmt, 0));
        break;
      default:
        throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
      }

    sqlite3_reset(stmt);

    ++iter;
  }
  sqlite3_finalize(stmt);

  std::vector<ID> mutations = this->mutations();
  ids.insert(ids.begin(), mutations.begin(), mutations.end());

  return ids;
}

}
}
