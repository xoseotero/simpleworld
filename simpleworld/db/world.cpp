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
  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
INSERT INTO World(position_x, position_y, orientation)\n\
VALUES(?, ?, ?);");
    sql.bind(1, static_cast<int>(position_x));
    sql.bind(2, static_cast<int>(position_y));
    sql.bind(3, static_cast<int>(orientation));

    sql.executenonquery();
    return db->insertid();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + db->errormsg() + ")");
  }
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
  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
INSERT INTO World(position_x, position_y)\n\
VALUES(?, ?);");
    sql.bind(1, static_cast<int>(position_x));
    sql.bind(2, static_cast<int>(position_y));

    sql.executenonquery();
    return db->insertid();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + db->errormsg() + ")");
  }
}

/**
 * Delete a element from the world.
 * @param db database.
 * @param id id of the bug.
 * @exception DBException if there is an error with the deletion.
 */
void World::remove(DB* db, ID id)
{
  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
DELETE FROM World\n\
WHERE id = ?;");
    sql.bind(1, id);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + db->errormsg() + ")");
  }
}


/**
 * Set the id of the element.
 * @param bug_id the new id.
 * @exception DBException if there is an error with the update.
 */
void World::id(ID id)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE World\n\
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
 * Get position in the x coordinate.
 * @return the position.
 * @exception DBException if there is an error with the query.
 */
Coord World::position_x() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT position_x\n\
FROM World\n\
WHERE id = ?");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table World")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Get position in the y coordinate.
 * @return the position.
 * @exception DBException if there is an error with the query.
 */
Coord World::position_y() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT position_y\n\
FROM World\n\
WHERE id = ?");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table World")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set position in the x coordinate.
 * @param bug_id the new position.
 * @exception DBException if there is an error with the update.
 */
void World::position_x(Coord position_x)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE World\n\
SET position_x = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<int>(position_x));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set position in the y coordinate.
 * @param bug_id the new position.
 * @exception DBException if there is an error with the update.
 */
void World::position_y(Coord position_y)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE World\n\
SET position_y = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<int>(position_y));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}


/**
 * Get the orientation.
 * @return the orientation.
 * @exception DBException if there is an error with the query.
 */
Orientation World::orientation() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT orientation\n\
FROM World\n\
WHERE id = ?");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table World")
                                              % this->id_));

    return static_cast<Orientation>(cursor.getint(0));
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set the orientation.
 * @param bug_id the new orientation.
 * @exception DBException if there is an error with the update.
 */
void World::orientation(Orientation orientation)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE World\n\
SET orientation = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<int>(orientation));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

}
}
