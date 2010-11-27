/**
 * @file simpleworld/db/world.cpp
 * Information about the world.
 *
 *  Copyright (C) 2010  Xosé Otero <xoseotero@gmail.com>
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
#include "world.hpp"

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
World::World(DB* db, ID id)
  : Table("World", db, id)
{
}


/**
 * Insert a element in the world with orientation.
 * @param db database.
 * @param position_x position in the x coordinate of the element.
 * @param position_y position in the y coordinate of the element.
 * @param orientation orientation of the element.
 * @return the id of the new row.
 * @exception DBException if there is an error with the insertion.
 */
ID World::insert(DB* db, Coord position_x, Coord position_y,
                 Orientation orientation)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
INSERT INTO World(position_x, position_y, orientation)\n\
VALUES(?, ?, ?);", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_bind_int(stmt, 1, position_x);
  sqlite3_bind_int(stmt, 2, position_y);
  sqlite3_bind_int(stmt, 3, orientation);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_finalize(stmt);

  return sqlite3_last_insert_rowid(db->db());
}

/**
 * Insert a element in the world without orientation.
 * @param db database.
 * @param position_x position in the x coordinate of the element.
 * @param position_y position in the y coordinate of the element.
 * @return the id of the new row.
 * @exception DBException if there is an error with the insertion.
 */
ID World::insert(DB* db, Coord position_x, Coord position_y)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
INSERT INTO World(position_x, position_y)\n\
VALUES(?, ?);", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_bind_int(stmt, 1, position_x);
  sqlite3_bind_int(stmt, 2, position_y);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_finalize(stmt);

  return sqlite3_last_insert_rowid(db->db());
}

/**
 * Delete a element from the world.
 * @param db database.
 * @param id id of the bug.
 * @exception DBException if there is an error with the deletion.
 */
void World::remove(DB* db, ID id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
DELETE FROM World\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_bind_int64(stmt, 1, id);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_finalize(stmt);
}


/**
 * Set the id of the element.
 * @param bug_id the new id.
 * @exception DBException if there is an error with the update.
 */
void World::id(ID id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE World\n\
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
 * Get position in the x coordinate.
 * @return the position.
 * @exception DBException if there is an error with the query.
 */
Coord World::position_x() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT position_x\n\
FROM World\n\
WHERE id = ?", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table World")
					    % this->id_));
  Coord position_x = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return position_x;
}

/**
 * Get position in the y coordinate.
 * @return the position.
 * @exception DBException if there is an error with the query.
 */
Coord World::position_y() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT position_y\n\
FROM World\n\
WHERE id = ?", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table World")
					    % this->id_));
  Coord position_y = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return position_y;
}

/**
 * Set position in the x coordinate.
 * @param bug_id the new position.
 * @exception DBException if there is an error with the update.
 */
void World::position_x(Coord position_x)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE World\n\
SET position_x = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, position_x);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

/**
 * Set position in the y coordinate.
 * @param bug_id the new position.
 * @exception DBException if there is an error with the update.
 */
void World::position_y(Coord position_y)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE World\n\
SET position_y = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, position_y);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
 * Get the orientation.
 * @return the orientation.
 * @exception DBException if there is an error with the query.
 */
Orientation World::orientation() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT orientation\n\
FROM World\n\
WHERE id = ?", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table World")
					    % this->id_));
  Orientation orientation =
    static_cast<Orientation>(sqlite3_column_int(stmt, 0));
  sqlite3_finalize(stmt);

  return orientation;
}

/**
 * Set the orientation.
 * @param bug_id the new orientation.
 * @exception DBException if there is an error with the update.
 */
void World::orientation(Orientation orientation)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE World\n\
SET orientation = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, static_cast<int>(orientation));
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

}
}
