/**
 * @file simpleworld/db/food.cpp
 * Information about the food
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

#include "exception.hpp"
#include "food.hpp"

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
Food::Food(DB* db, ID id)
  : Table("Food", db, id)
{
}


/**
 * Insert a food.
 * @param db database.
 * @param world_id id of the world.
 * @param size size.
 * @return the id of the new row.
 * @exception DBException if there is an error with the insertion.
 */
ID Food::insert(DB* db, ID world_id, Energy size)
{
  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
INSERT INTO Food(world_id, size)\n\
VALUES(?, ?);");
    sql.bind(1, static_cast<sqlite3x::int64_t>(world_id));
    sql.bind(2, static_cast<int>(size));

    sql.executenonquery();
    return db->insertid();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + db->errormsg() + ")");
  }
}

/**
 * Delete a food.
 * @param db database.
 * @param id id of the food.
 * @exception DBException if there is an error with the deletion.
 */
void Food::remove(DB* db, ID id)
{
  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
DELETE FROM Food\n\
WHERE id = ?;");
    sql.bind(1, id);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + db->errormsg() + ")");
  }
}


/**
 * Set the id of the food.
 * @param id the new id.
 * @exception DBException if there is an error with the update.
 */
void Food::id(ID id)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Food\n\
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
 * Get the id of the world.
 * @return the id.
 * @exception DBException if there is an error with the query.
 */
ID Food::world_id() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT world_id\n\
FROM Food\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Food")
                                              % this->id_));

    return cursor.getint64(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set the id of the world.
 * @param world_id the new id.
 * @exception DBException if there is an error with the update.
 */
void Food::world_id(ID world_id)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Food\n\
SET world_id = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<sqlite3x::int64_t>(world_id));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}


/**
 * Get the size of the food.
 * @return the size.
 * @exception DBException if there is an error with the query.
 */
Energy Food::size() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT size\n\
FROM Food\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Food")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set the size of the food.
 * @param size the new size.
 * @exception DBException if there is an error with the update.
 */
void Food::size(Energy size)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Food\n\
SET size = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<int>(size));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

}
}
