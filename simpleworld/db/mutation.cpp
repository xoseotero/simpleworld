/**
 * @file simpleworld/db/mutation.cpp
 * A mutation of a bug.
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

#include <string>

#include <boost/format.hpp>

#include <sqlite3.h>

#include "exception.hpp"
#include "mutation.hpp"
#include <boost/concept_check.hpp>

namespace simpleworld
{
namespace db
{

/**
 * Constructor.
 * It's not checked if the id is in the table, only when accessing the data
 * the id is checked.
 * @param db database.
 * @param id id of the mutation.
 */
Mutation::Mutation(DB* db, ID bug_id)
  : Table("Mutation", db, bug_id)
{
}


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
ID Mutation::insert(DB* db, ID bug_id, Time time, Uint32 position,
                    Uint32 original, Uint32 mutated)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
INSERT INTO Mutation(bug_id, time, position, original, mutated)\n\
VALUES(?, ?, ?, ?, ?);", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_bind_int64(stmt, 1, bug_id);
  sqlite3_bind_int(stmt, 2, time);
  sqlite3_bind_int(stmt, 3, position);
  sqlite3_bind_int(stmt, 4, original);
  sqlite3_bind_int(stmt, 5, mutated);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_finalize(stmt);

  return sqlite3_last_insert_rowid(db->db());
}

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
ID Mutation::insert_addition(DB* db, ID bug_id, Time time, Uint32 position,
                             Uint32 mutated)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
INSERT INTO Mutation(bug_id, time, position, mutated)\n\
VALUES(?, ?, ?, ?);", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_bind_int64(stmt, 1, bug_id);
  sqlite3_bind_int(stmt, 2, time);
  sqlite3_bind_int(stmt, 3, position);
  sqlite3_bind_int(stmt, 4, mutated);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_finalize(stmt);

  return sqlite3_last_insert_rowid(db->db());
}

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
ID Mutation::insert_deletion(DB* db, ID bug_id, Time time, Uint32 position,
                             Uint32 original)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
INSERT INTO Mutation(bug_id, time, position, original)\n\
VALUES(?, ?, ?, ?);", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_bind_int64(stmt, 1, bug_id);
  sqlite3_bind_int(stmt, 2, time);
  sqlite3_bind_int(stmt, 3, position);
  sqlite3_bind_int(stmt, 4, original);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_finalize(stmt);

  return sqlite3_last_insert_rowid(db->db());
}

/**
 * Delete a mutation.
 * @param db database.
 * @param id id of the mutation.
 * @exception DBException if there is an error with the deletion.
 */
void Mutation::remove(DB* db, ID id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
DELETE FROM Mutation\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_bind_int64(stmt, 1, id);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_finalize(stmt);
}


/**
 * Set the id of the mutation.
 * @param id the new id.
 * @exception DBException if there is an error with the update.
 */
void Mutation::id(ID id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Mutation\n\
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
 * Get the id of the bug.
 * @return the id of the bug.
 * @exception DBException if there is an error with the query.
 */
ID Mutation::bug_id() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT bug_id\n\
FROM Mutation\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Mutation")
					    % this->id_));
  ID id = sqlite3_column_int64(stmt, 0);
  sqlite3_finalize(stmt);

  return id;
}

/**
 * Set the id of the bug.
 * @param bug_id the new id.
 * @exception DBException if there is an error with the update.
 */
void Mutation::bug_id(ID bug_id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Mutation\n\
SET bug_id = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, bug_id);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
 * Get when the mutation happened.
 * @return the time.
 * @exception DBException if there is an error with the query.
 */
Time Mutation::time() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT time\n\
FROM Mutation\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Mutation")
					    % this->id_));
  Time time = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return time;
}

/**
 * Set when the mutation happened.
 * @param time the new time.
 * @exception DBException if there is an error with the update.
 */
void Mutation::time(Time time)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Mutation\n\
SET time = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, time);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
 * Get where the mutation happened.
 * @return the position.
 * @exception DBException if there is an error with the query.
 */
Uint32 Mutation::position() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT position\n\
FROM Mutation\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Mutation")
					    % this->id_));
  Uint32 position = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return position;
}

/**
 * Set where the mutation happened.
 * @param position the new position.
 * @exception DBException if there is an error with the update.
 */
void Mutation::position(Uint32 position)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Mutation\n\
SET position = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, position);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

/**
 * Get the previous value of the code.
 * original is NULL if the mutation is the addition of code.
 * @return the previous value.
 * @exception DBException if there is an error with the query.
 */
Uint32 Mutation::original() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT original\n\
FROM Mutation\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Mutation")
					    % this->id_));
  Uint32 original = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return original;
}

/**
 * Set the previous value of the code.
 * @param original the previous value.
 * @exception DBException if there is an error with the query.
 */
void Mutation::original(Uint32 original)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Mutation\n\
SET original = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, original);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

/**
 * Get the new value of the code.
 * mutated is NULL if the mutation is the deletion of code.
 * @return the new value.
 * @exception DBException if there is an error with the query.
 */
Uint32 Mutation::mutated() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT mutated\n\
FROM Mutation\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Mutation")
					    % this->id_));
  Uint32 mutated = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return mutated;
}

/**
 * Set the new value of the code.
 * @param mutated the new value.
 * @exception DBException if there is an error with the query.
 */
void Mutation::mutated(Uint32 mutated)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Mutation\n\
SET mutated = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, mutated);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

}
}
