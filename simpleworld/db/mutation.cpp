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
 * @param bug_id id of the bug.
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
 * @param positon where the mutation happened.
 * @param original the previous value of the code.
 * @param mutated the new value of the code.
 * @return the id of the new row.
 * @exception DBException if there is an error with the insertion.
 */
ID Mutation::insert(DB* db, ID bug_id, Time time, Uint32 position,
                    Uint32 original, Uint32 mutated)
{
  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
INSERT INTO Mutation(bug_id, time, position, original, mutated)\n\
VALUES(?, ?, ?, ?, ?);");
    sql.bind(1, static_cast<sqlite3x::int64_t>(bug_id));
    sql.bind(2, static_cast<int>(time));
    sql.bind(3, static_cast<int>(position));
    sql.bind(4, static_cast<int>(original));
    sql.bind(5, static_cast<int>(mutated));

    sql.executenonquery();
    return db->insertid();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + db->errormsg() + ")");
  }
}

/**
 * Insert a addition of code.
 * @param db database.
 * @param bug_id id of the bug.
 * @param time when the mutation happened.
 * @param positon where the mutation happened.
 * @param mutated the new word.
 * @return the id of the new row.
 * @exception DBException if there is an error with the insertion.
 */
ID Mutation::insert_addition(DB* db, ID bug_id, Time time, Uint32 position,
                             Uint32 mutated)
{
  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
INSERT INTO Mutation(bug_id, time, position, mutated)\n\
VALUES(?, ?, ?, ?);");
    sql.bind(1, static_cast<sqlite3x::int64_t>(bug_id));
    sql.bind(2, static_cast<int>(time));
    sql.bind(3, static_cast<int>(position));
    sql.bind(4, static_cast<int>(mutated));

    sql.executenonquery();
    return db->insertid();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + db->errormsg() + ")");
  }
}

/**
 * Insert a deletion of code.
 * @param db database.
 * @param bug_id id of the bug.
 * @param time when the mutation happened.
 * @param positon where the mutation happened.
 * @param original the deleted word.
 * @return the id of the new row.
 * @exception DBException if there is an error with the insertion.
 */
ID Mutation::insert_deletion(DB* db, ID bug_id, Time time, Uint32 position,
                             Uint32 original)
{
  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
INSERT INTO Mutation(bug_id, time, position, original)\n\
VALUES(?, ?, ?, ?);");
    sql.bind(1, static_cast<sqlite3x::int64_t>(bug_id));
    sql.bind(2, static_cast<int>(time));
    sql.bind(3, static_cast<int>(position));
    sql.bind(4, static_cast<int>(original));

    sql.executenonquery();
    return db->insertid();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + db->errormsg() + ")");
  }
}

/**
 * Delete a mutation.
 * @param db database.
 * @param id id of the mutation.
 * @exception DBException if there is an error with the deletion.
 */
void Mutation::remove(DB* db, ID id)
{
  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
DELETE FROM Mutation\n\
WHERE id = ?;");
    sql.bind(1, id);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + db->errormsg() + ")");
  }
}


/**
 * Set the id of the mutation.
 * @param id the new id.
 * @exception DBException if there is an error with the update.
 */
void Mutation::id(ID id)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Mutation\n\
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
 * Get the id of the bug.
 * @return the id of the bug.
 * @exception DBException if there is an error with the query.
 */
ID Mutation::bug_id() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT bug_id\n\
FROM Mutation\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Mutation")
                                              % this->id_));

    return cursor.getint64(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db_->errormsg() + ")");
  }
}

/**
 * Set the id of the bug.
 * @param world_id the new id.
 * @exception DBException if there is an error with the update.
 */
void Mutation::bug_id(ID bug_id)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Mutation\n\
SET bug_id = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<sqlite3x::int64_t>(bug_id));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
		    " (" + this->db_->errormsg() + ")");
  }
}


/**
 * Get when the mutation happened.
 * @return the time.
 * @exception DBException if there is an error with the query.
 */
Time Mutation::time() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT time\n\
FROM Mutation\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Mutation")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db_->errormsg() + ")");
  }
}

/**
 * Set when the mutation happened.
 * @param size the new size.
 * @exception DBException if there is an error with the update.
 */
void Mutation::time(Time time)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Mutation\n\
SET time = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<int>(time));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
		    " (" + this->db_->errormsg() + ")");
  }
}


/**
 * Get where the mutation happened.
 * @return the position.
 * @exception DBException if there is an error with the query.
 */
Uint32 Mutation::position() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT position\n\
FROM Mutation\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Mutation")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db_->errormsg() + ")");
  }
}

/**
 * Set where the mutation happened.
 * @param position the new position.
 * @exception DBException if there is an error with the update.
 */
void Mutation::position(Uint32 position)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Mutation\n\
SET position = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<int>(position));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
		    " (" + this->db_->errormsg() + ")");
  }
}

/**
 * Get the previous value of the code.
 * original is NULL if the mutation is the addition of code.
 * @return the previous value.
 * @exception DBException if there is an error with the query.
 */
Uint32 Mutation::original() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT original\n\
FROM Mutation\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Mutation")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db_->errormsg() + ")");
  }
}

/**
 * Set the previous value of the code.
 * @param original the previous value.
 * @exception DBException if there is an error with the query.
 */
void Mutation::original(Uint32 original)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Mutation\n\
SET original = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<int>(original));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
		    " (" + this->db_->errormsg() + ")");
  }
}

/**
 * Get the new value of the code.
 * mutated is NULL if the mutation is the deletion of code.
 * @return the new value.
 * @exception DBException if there is an error with the query.
 */
Uint32 Mutation::mutated() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT mutated\n\
FROM Mutation\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Mutation")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db_->errormsg() + ")");
  }
}

/**
 * Set the new value of the code.
 * @param mutated the new value.
 * @exception DBException if there is an error with the query.
 */
void Mutation::mutated(Uint32 mutated)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Mutation\n\
SET mutated = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<int>(mutated));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
		    " (" + this->db_->errormsg() + ")");
  }
}

}
}
