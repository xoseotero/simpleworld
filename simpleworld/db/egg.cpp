/**
 * @file simpleworld/db/egg.cpp
 * Information about an egg.
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

#include <sqlite3x.hpp>

#include "exception.hpp"
#include "egg.hpp"

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
Egg::Egg(DB* db, ID bug_id)
  : Table("Egg", db, bug_id)
{
}


/**
 * Insert a egg.
 * @param db database.
 * @param bug_id id of the bug.
 * @param world_id id of the world.
 * @param energy energy.
 * @param conception when the egg was created.
 * @return the id of the new row (the same as bug_id).
 * @exception DBException if there is an error with the insertion.
 */
ID Egg::insert(DB* db, ID bug_id, ID world_id, Energy energy, Time conception)
{
  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
INSERT INTO Egg(bug_id, world_id, energy, conception)\n\
VALUES(?, ?, ?, ?);");
    sql.bind(1, static_cast<sqlite3x::int64_t>(bug_id));
    sql.bind(2, static_cast<sqlite3x::int64_t>(world_id));
    sql.bind(3, static_cast<int>(energy));
    sql.bind(4, static_cast<int>(conception));

    sql.executenonquery();
    return db->insertid();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + db->errormsg() + ")");
  }
}

/**
 * Delete a egg.
 * @param db database.
 * @param bug_id id of the egg.
 * @exception DBException if there is an error with the deletion.
 */
void Egg::remove(DB* db, ID id)
{
  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
DELETE FROM Egg\n\
WHERE bug_id = ?;");
    sql.bind(1, id);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + db->errormsg() + ")");
  }
}


/**
 * Get the id of the egg.
 * @return the id.
 * @exception DBException if there is an error with the query.
 */
ID Egg::bug_id() const
{
  return this->id_;
}

/**
 * Set the id of the egg.
 * @param id the new id.
 * @exception DBException if there is an error with the update.
 */
void Egg::bug_id(ID bug_id)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Egg\n\
SET bug_id = ?\n\
WHERE bug_id = ?;");
    sql.bind(1, static_cast<sqlite3x::int64_t>(bug_id));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }

  this->id_ = bug_id;
}


/**
 * Get the id of the world.
 * @return the id.
 * @exception DBException if there is an error with the query.
 */
ID Egg::world_id() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT world_id\n\
FROM Egg\n\
WHERE bug_id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Egg")
                                              % this->id_));

    return cursor.getint64(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set the id of the world.
 * @param id the new id.
 * @exception DBException if there is an error with the update.
 */
void Egg::world_id(ID world_id)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Egg\n\
SET world_id = ?\n\
WHERE bug_id = ?;");
    sql.bind(1, static_cast<sqlite3x::int64_t>(world_id));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}


/**
 * Get the energy of the egg.
 * @return the energy.
 * @exception DBException if there is an error with the query.
 */
Energy Egg::energy() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT energy\n\
FROM Egg\n\
WHERE bug_id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Egg")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set the energy of the egg.
 * @param energy the new energy.
 * @exception DBException if there is an error with the update.
 */
void Egg::energy(Energy energy)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Egg\n\
SET energy = ?\n\
WHERE bug_id = ?;");
    sql.bind(1, static_cast<int>(energy));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Get when the egg was created.
 * @return the time.
 * @exception DBException if there is an error with the query.
 */
Time Egg::conception() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT conception\n\
FROM Egg\n\
WHERE bug_id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Egg")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set when the egg was created.
 * @param conception the new time.
 * @exception DBException if there is an error with the update.
 */
void Egg::conception(Time conception)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Egg\n\
SET conception = ?\n\
WHERE bug_id = ?;");
    sql.bind(1, static_cast<int>(conception));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

}
}
