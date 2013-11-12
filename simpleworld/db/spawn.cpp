/**
 * @file simpleworld/db/spawn.cpp
 * Information about a spawn.
 *
 *  Copyright (C) 2007-2013  Xosé Otero <xoseotero@gmail.com>
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

#include <sqlite3.h>

#include "exception.hpp"
#include "spawn.hpp"

namespace simpleworld
{
namespace db
{

/**
 * Constructor.
 * It's not checked if the id is in the table, only when accessing the data
 * the id is checked.
 * @param db database.
 * @param id id of the spawn.
 */
Spawn::Spawn(DB* db, ID id)
  : Table("Spawn", db, id)
{
}


/**
 * Insert a spawn.
 * @param db database.
 * @param code ID of the code of the new bugs.
 * @param frequency the frequency of the spawns.
 * @param max maximum number of elements in the region.
 * @param start_x x coord of the start of the region.
 * @param start_y y coord of the start of the region.
 * @param end_x x coord of the end of the region.
 * @param end_y y coord of the end of the region.
 * @param energy energy of new bugs.
 * @return the id of the new row (the same as bug_id).
 * @exception DBException if there is an error with the insertion.
 */
ID Spawn::insert(DB* db, ID code_id, Time frequency, Uint16 max,
                 Coord start_x, Coord start_y, Coord end_x, Coord end_y,
		 Energy energy)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
INSERT INTO Spawn(code_id, frequency, max, start_x, start_y, end_x, end_y,\n\
                  energy)\n\
VALUES(?, ?, ?, ?, ?, ?, ?, ?);", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_bind_int64(stmt, 1, code_id);
  sqlite3_bind_int(stmt, 2, frequency);
  sqlite3_bind_int(stmt, 3, max);
  sqlite3_bind_int(stmt, 4, start_x);
  sqlite3_bind_int(stmt, 5, start_y);
  sqlite3_bind_int(stmt, 6, end_x);
  sqlite3_bind_int(stmt, 7, end_y);
  sqlite3_bind_int(stmt, 8, energy);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_finalize(stmt);

  return sqlite3_last_insert_rowid(db->db());
}

/**
 * Delete a spawn.
 * @param db database.
 * @param id id of the spawn.
 * @exception DBException if there is an error with the deletion.
 */
void Spawn::remove(DB* db, ID id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
DELETE FROM Spawn\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_bind_int64(stmt, 1, id);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_finalize(stmt);
}


/**
 * Set the id of the spawn.
 * @param id the new id.
 * @exception DBException if there is an error with the update.
 */
void Spawn::id(ID id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Spawn\n\
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
 * Get the id of the code of new bugs.
 * @return the code.
 * @exception DBException if there is an error with the query.
 */
ID Spawn::code_id() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT code_id\n\
FROM Spawn\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table AliveBug")
    % this->id_));
  ID code_id = sqlite3_column_int64(stmt, 0);
  sqlite3_finalize(stmt);

  return code_id;
}

/**
 * Set the id of the code of new bugs.
 * @param the id of the code.
 * @exception DBException if there is an error with the query.
 */
void Spawn::code_id(ID code_id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Spawn\n\
SET code_id = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, code_id);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
 * Get the frequency of the spawns.
 * @return the frequency.
 * @exception DBException if there is an error with the query.
 */
Time Spawn::frequency() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT frequency\n\
FROM Spawn\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Spawn")
                                            % this->id_));
  Time frequency = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return frequency;
}

/**
 * Set the frequency of the spawns.
 * @param frequency the new frequency.
 * @exception DBException if there is an error with the update.
 */
void Spawn::frequency(Time frequency)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Spawn\n\
SET frequency = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, frequency);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
 * Get the maximum number of elements in the region.
 * @return the maximum.
 * @exception DBException if there is an error with the query.
 */
Uint16 Spawn::max() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT max\n\
FROM Spawn\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Spawn")
                                            % this->id_));
  Uint16 max = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return max;
}

/**
 * Set the maximum number of elements in the region.
 * @param max the new maximum.
 * @exception DBException if there is an error with the update.
 */
void Spawn::max(Uint16 max)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Spawn\n\
SET max = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, max);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

/**
 * Get the x coord of the start of the region.
 * @return the x coord.
 * @exception DBException if there is an error with the query.
 */
Coord Spawn::start_x() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT start_x\n\
FROM Spawn\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Spawn")
                                            % this->id_));
  Coord start_x = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return start_x;
}

/**
 * Set the x coord of the start of the region.
 * @param start_x the new x coord.
 * @exception DBException if there is an error with the update.
 */
void Spawn::start_x(Coord start_x)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Spawn\n\
SET start_x = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, start_x);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

/**
 * Get the y coord of the start of the region.
 * @return the y coord.
 * @exception DBException if there is an error with the query.
 */
Coord Spawn::start_y() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT start_y\n\
FROM Spawn\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Spawn")
                                            % this->id_));
  Coord start_y = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return start_y;
}

/**
 * Set the y coord of the start of the region.
 * @param start_y the new x coord.
 * @exception DBException if there is an error with the update.
 */
void Spawn::start_y(Coord start_y)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Spawn\n\
SET start_y = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, start_y);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

/**
 * Get the x coord of the end of the region.
 * @return the x coord.
 * @exception DBException if there is an error with the query.
 */
Coord Spawn::end_x() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT end_x\n\
FROM Spawn\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Spawn")
                                            % this->id_));
  Coord end_x = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return end_x;
}

/**
 * Set the x coord of the end of the region.
 * @param end_x the new x coord.
 * @exception DBException if there is an error with the update.
 */
void Spawn::end_x(Coord end_x)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Spawn\n\
SET end_x = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, end_x);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

/**
 * Get the y coord of the end of the region.
 * @return the y coord.
 * @exception DBException if there is an error with the query.
 */
Coord Spawn::end_y() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT end_y\n\
FROM Spawn\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Spawn")
                                            % this->id_));
  Coord end_y = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return end_y;
}

/**
 * Set the y coord of the end of the region.
 * @param end_y the new y coord.
 * @exception DBException if there is an error with the update.
 */
void Spawn::end_y(Coord end_y)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Spawn\n\
SET end_y = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, end_y);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
 * Get the energy of new bugs.
 * @return the energy.
 * @exception DBException if there is an error with the query.
 */
Energy Spawn::energy() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT energy\n\
FROM Spawn\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Spawn")
                                            % this->id_));
  Energy energy = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return energy;
}

/**
 * Set the energy of new bugs.
 * @param energy the new energy.
 * @exception DBException if there is an error with the update.
 */
void Spawn::energy(Energy energy)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Spawn\n\
SET energy = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, energy);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

}
}
