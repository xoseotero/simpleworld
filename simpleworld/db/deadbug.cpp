/**
 * @file simpleworld/db/deadbug.cpp
 * Information about a dead bug.
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
#include "bug.hpp"
#include "deadbug.hpp"

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
DeadBug::DeadBug(DB* db, ID bug_id)
  : Table("DeadBug", db, bug_id)
{
}


/**
 * Insert a dead bug without birth nor killer_id.
 * @param db database.
 * @param bug_id id of the bug.
 * @param death when the bug had dead.
 * @return the id of the new row (the same as bug_id).
 * @exception DBException if there is an error with the insertion.
 */
ID DeadBug::insert(DB* db, ID bug_id, Time death)
{
  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
INSERT INTO DeadBug(bug_id, death)\n\
VALUES(?, ?);");
    sql.bind(1, static_cast<sqlite3x::int64_t>(bug_id));
    sql.bind(2, static_cast<int>(death));

    sql.executenonquery();
    return db->insertid();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + db->errormsg() + ")");
  }
}

/**
 * Insert a dead bug with birth but withoutr killer_id.
 * @param db database.
 * @param bug_id id of the bug.
 * @param birth when the bug had born.
 * @param death when the bug had dead.
 * @return the id of the new row (the same as bug_id).
 * @exception DBException if there is an error with the insertion.
 */
ID DeadBug::insert(DB* db, ID bug_id, Time birth, Time death)
{
  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
INSERT INTO DeadBug(bug_id, birth, death)\n\
VALUES(?, ?, ?);");
    sql.bind(1, static_cast<sqlite3x::int64_t>(bug_id));
    sql.bind(2, static_cast<int>(birth));
    sql.bind(3, static_cast<int>(death));

    sql.executenonquery();
    return db->insertid();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + db->errormsg() + ")");
  }
}

/**
 * Insert a dead bug with killer_id but without birth.
 * @param db database.
 * @param bug_id id of the bug.
 * @param death when the bug had dead.
 * @param killer_id who had killed the bug.
 * @return the id of the new row (the same as bug_id).
 * @exception DBException if there is an error with the insertion.
 */
ID DeadBug::insert(DB* db, ID bug_id, Time death, ID killer_id)
{
  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
INSERT INTO DeadBug(bug_id, death, killer_id)\n\
VALUES(?, ?, ?);");
    sql.bind(1, static_cast<sqlite3x::int64_t>(bug_id));
    sql.bind(2, static_cast<int>(death));
    sql.bind(3, static_cast<sqlite3x::int64_t>(killer_id));

    sql.executenonquery();
    return db->insertid();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + db->errormsg() + ")");
  }
}

/**
 * Insert a dead bug with birth and killer_id.
 * @param db database.
 * @param bug_id id of the bug.
 * @param birth when the bug had born.
 * @param death when the bug had dead.
 * @param killer_id who had killed the bug.
 * @return the id of the new row (the same as bug_id).
 * @exception DBException if there is an error with the insertion.
 */
ID DeadBug::insert(DB* db, ID bug_id, Time birth, Time death, ID killer_id)
{
  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
INSERT INTO DeadBug(bug_id, birth, death, killer_id)\n\
VALUES(?, ?, ?, ?);");
    sql.bind(1, static_cast<sqlite3x::int64_t>(bug_id));
    sql.bind(2, static_cast<int>(birth));
    sql.bind(3, static_cast<int>(death));
    sql.bind(4, static_cast<sqlite3x::int64_t>(killer_id));

    sql.executenonquery();
    return db->insertid();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + db->errormsg() + ")");
  }
}

/**
 * Insert a dead bug from a egg without killer_id.
 * The egg is deleted.
 * @param db database.
 * @param egg egg to kill.
 * @param death when the egg had dead.
 * @return the id of the new row (the same as the egg id).
 * @exception DBException if there is an error with the insertion.
 */
ID DeadBug::insert(DB* db, Egg* egg, Time death)
{
  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
INSERT INTO DeadBug(bug_id, death)\n\
VALUES(?, ?);");
    sql.bind(1, static_cast<sqlite3x::int64_t>(egg->bug_id()));
    sql.bind(2, static_cast<int>(death));

    sql.executenonquery();
    ID id = db->insertid();

    Egg::remove(db, egg->id());

    return id;
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + db->errormsg() + ")");
  }
}

/**
 * Insert a dead bug from a egg with killer_id.
 * The egg is deleted.
 * @param db database.
 * @param egg egg to kill.
 * @param death when the egg had dead.
 * @param killer_id who had killed the egg.
 * @return the id of the new row (the same as the egg id).
 * @exception DBException if there is an error with the insertion.
 */
ID DeadBug::insert(DB* db, Egg* egg, Time death, ID killer_id)
{
  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
INSERT INTO DeadBug(bug_id, death, killer_id)\n\
VALUES(?, ?, ?);");
    sql.bind(1, static_cast<sqlite3x::int64_t>(egg->bug_id()));
    sql.bind(2, static_cast<int>(death));
    sql.bind(3, static_cast<sqlite3x::int64_t>(killer_id));

    sql.executenonquery();
    ID id = db->insertid();

    Egg::remove(db, egg->id());

    return id;
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + db->errormsg() + ")");
  }
}

/**
 * Insert a dead bug from a alive bug without killer_id.
 * The alive bug is deleted.
 * @param db database.
 * @param bug bug to kill.
 * @param death when the bug had dead.
 * @return the id of the new row (the same as the bug id).
 * @exception DBException if there is an error with the insertion.
 */
ID DeadBug::insert(DB* db, AliveBug* alivebug, Time death)
{
  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
INSERT INTO DeadBug(bug_id, birth, death)\n\
VALUES(?, ?, ?);");
    sql.bind(1, static_cast<sqlite3x::int64_t>(alivebug->bug_id()));
    sql.bind(2, static_cast<int>(alivebug->birth()));
    sql.bind(3, static_cast<int>(death));

    sql.executenonquery();
    ID id = db->insertid();

    AliveBug::remove(db, alivebug->id());

    return id;
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + db->errormsg() + ")");
  }
}

/**
 * Insert a dead bug from a alive bug with killer_id.
 * The alive bug is deleted.
 * @param db database.
 * @param bug bug to kill.
 * @param death when the bug had dead.
 * @param killer_id who had killed the bug.
 * @return the id of the new row (the same as the bug id).
 * @exception DBException if there is an error with the insertion.
 */
ID DeadBug::insert(DB* db, AliveBug* alivebug, Time death, ID killer_id)
{
  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
INSERT INTO DeadBug(bug_id, birth, death, killer_id)\n\
VALUES(?, ?, ?, ?);");
    sql.bind(1, static_cast<sqlite3x::int64_t>(alivebug->bug_id()));
    sql.bind(2, static_cast<int>(alivebug->birth()));
    sql.bind(3, static_cast<int>(death));
    sql.bind(4, static_cast<sqlite3x::int64_t>(killer_id));

    sql.executenonquery();
    ID id = db->insertid();

    AliveBug::remove(db, alivebug->id());

    return id;
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + db->errormsg() + ")");
  }
}

/**
 * Delete a dead bug.
 * @param db database.
 * @param bug_id id of the bug..
 * @exception DBException if there is an error with the deletion.
 */
void DeadBug::remove(DB* db, ID bug_id)
{
  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
DELETE FROM DeadBug\n\
WHERE bug_id = ?;");
    sql.bind(1, bug_id);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + db->errormsg() + ")");
  }
}


/**
 * Get the id of the bug.
 * @return the id.
 * @exception DBException if there is an error with the query.
 */
ID DeadBug::bug_id() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT bug_id\n\
FROM DeadBug\n\
WHERE bug_id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table DeadBug")
                                              % this->id_));

    return cursor.getint64(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set the id of the bug.
 * @param id the new id.
 * @exception DBException if there is an error with the update.
 */
void DeadBug::bug_id(ID bug_id)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE DeadBug\n\
SET bug_id = ?\n\
WHERE bug_id = ?;");
    sql.bind(1, static_cast<sqlite3x::int64_t>(bug_id));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}


/**
 * Get when the bug had born.
 * It's NULL if the bug haven't born (it was a egg when died).
 * @return the time.
 * @exception DBException if there is an error with the query.
 */
Time DeadBug::birth() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT birth\n\
FROM DeadBug\n\
WHERE bug_id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table DeadBug")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set when the bug had born.
 * @param birth the new time.
 * @exception DBException if there is an error with the update.
 */
void DeadBug::birth(Time birth)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE DeadBug\n\
SET birth = ?\n\
WHERE bug_id = ?;");
    sql.bind(1, static_cast<int>(birth));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Get when the bug had dead.
 * @return the time.
 * @exception DBException if there is an error with the query.
 */
Time DeadBug::death() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT death\n\
FROM DeadBug\n\
WHERE bug_id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table DeadBug")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set when the bug had dead.
 * @param death the new time.
 * @exception DBException if there is an error with the update.
 */
void DeadBug::death(Time death)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE DeadBug\n\
SET death = ?\n\
WHERE bug_id = ?;");
    sql.bind(1, static_cast<int>(death));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Get who had killed the bug.
 * It's NULL if the bug haven't been killed by a bug.
 * @return the id.
 * @exception DBException if there is an error with the query.
 */
ID DeadBug::killer_id() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT killer_id\n\
FROM DeadBug\n\
WHERE bug_id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table DeadBug")
                                              % this->id_));

    return cursor.getint64(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set who had killed the bug.
 * @param killer_id the id.
 * @exception DBException if there is an error with the update.
 */
void DeadBug::killer_id(ID killer_id)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE DeadBug\n\
SET killer_id = ?\n\
WHERE bug_id = ?;");
    sql.bind(1, static_cast<sqlite3x::int64_t>(killer_id));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

}
}
