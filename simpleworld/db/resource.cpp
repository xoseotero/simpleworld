/**
 * @file simpleworld/db/resource.cpp
 * Information about a resource.
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

#include <sqlite3.h>

#include "exception.hpp"
#include "resource.hpp"

namespace simpleworld
{
namespace db
{

/**
 * Constructor.
 * It's not checked if the id is in the table, only when accessing the data
 * the id is checked.
 * @param db database.
 * @param id id of the resource.
 */
Resource::Resource(DB* db, ID id)
  : Table("Resource", db, id)
{
}


/**
 * Insert a resource.
 * @param db database.
 * @param frequency the frequency of the resource.
 * @param max maximum number of elements in the region.
 * @param start_x x coord of the start of the region.
 * @param start_y y coord of the start of the region.
 * @param end_x x coord of the end of the region.
 * @param end_y y coord of the end of the region.
 * @param size size of new food.
 * @return the id of the new row (the same as bug_id).
 * @exception DBException if there is an error with the insertion.
 */
ID Resource::insert(DB* db, Time frequency, Uint16 max,
                    Coord start_x, Coord start_y, Coord end_x, Coord end_y,
                    Energy size)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
INSERT INTO Resource(frequency, max, start_x, start_y, end_x, end_y, size)\n\
VALUES(?, ?, ?, ?, ?, ?, ?);", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_bind_int(stmt, 1, frequency);
  sqlite3_bind_int(stmt, 2, max);
  sqlite3_bind_int(stmt, 3, start_x);
  sqlite3_bind_int(stmt, 4, start_y);
  sqlite3_bind_int(stmt, 5, end_x);
  sqlite3_bind_int(stmt, 6, end_y);
  sqlite3_bind_int(stmt, 7, size);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_finalize(stmt);

  return sqlite3_last_insert_rowid(db->db());
}

/**
 * Delete a resource.
 * @param db database.
 * @param id id of the resource.
 * @exception DBException if there is an error with the deletion.
 */
void Resource::remove(DB* db, ID id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
DELETE FROM Resource\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_bind_int64(stmt, 1, id);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_finalize(stmt);
}


/**
 * Set the id of the resource.
 * @param id the new id.
 * @exception DBException if there is an error with the update.
 */
void Resource::id(ID id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Resource\n\
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
 * Get the frequency of the resource.
 * @return the frequency.
 * @exception DBException if there is an error with the query.
 */
Time Resource::frequency() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT frequency\n\
FROM Resource\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Resource")
                                            % this->id_));
  Time frequency = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return frequency;
}

/**
 * Set the frequency of the resource.
 * @param frequency the new frequency.
 * @exception DBException if there is an error with the update.
 */
void Resource::frequency(Time frequency)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Resource\n\
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
Uint16 Resource::max() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT max\n\
FROM Resource\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Resource")
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
void Resource::max(Uint16 max)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Resource\n\
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
Coord Resource::start_x() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT start_x\n\
FROM Resource\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Resource")
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
void Resource::start_x(Coord start_x)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Resource\n\
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
Coord Resource::start_y() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT start_y\n\
FROM Resource\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Resource")
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
void Resource::start_y(Coord start_y)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Resource\n\
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
Coord Resource::end_x() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT end_x\n\
FROM Resource\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Resource")
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
void Resource::end_x(Coord end_x)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Resource\n\
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
Coord Resource::end_y() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT end_y\n\
FROM Resource\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Resource")
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
void Resource::end_y(Coord end_y)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Resource\n\
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
 * Get the size of new food.
 * @return the size.
 * @exception DBException if there is an error with the query.
 */
Energy Resource::size() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT size\n\
FROM Resource\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Resource")
                                            % this->id_));
  Energy size = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return size;
}

/**
 * Set the size of new food.
 * @param size the new size.
 * @exception DBException if there is an error with the update.
 */
void Resource::size(Energy size)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Resource\n\
SET size = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, size);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

}
}
