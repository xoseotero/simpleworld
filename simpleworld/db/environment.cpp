/**
 * @file simpleworld/db/environment.cpp
 * Information about the environment of the world.
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
#include "environment.hpp"

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
Environment::Environment(DB* db, ID id)
  : Table("Environment", db, id)
{
}


/**
 * Insert a environment.
 * @param db database.
 * @param time cycles since the creation of the World.
 * @param size_x size of the World (x coord).
 * @param size_y size of the World (y coord).
 * @param mutations_probability probability (0.0-1.0) that the code mutates.
 * @param time_birth cycles to convert a egg into a bug.
 * @param time_mutate cycles to mutate the code of a old bug.
 * @param time_laziness cycles without doing a action to consider a bug lazy.
 * @param energy_laziness energy substracted for laziness.
 * @param attack_multiplier multiplier for the energy of a attack.
 * @param energy_nothing energy needed to do the action nothing.
 * @param energy_myself energy needed to do the action myself.
 * @param energy_detect energy needed to do the action detect.
 * @param energy_info energy needed to do the action info.
 * @param energy_move energy needed to do the action move.
 * @param energy_turn energy needed to do the action turn.
 * @param energy_attack energy needed to do the action attack.
 * @param energy_eat energy needed to do the action eat.
 * @param energy_egg energy needed to do egg
 * @return the id of the new row.
 * @exception DBException if there is an error with the insertion.
 */
ID Environment::insert(DB* db, Time time, Coord size_x, Coord size_y,
		       double mutations_probability, Time time_birth,
		       Time time_mutate, Time time_laziness,
		       Energy energy_laziness, double attack_multiplier,
		       Energy energy_nothing, Energy energy_myself,
		       Energy energy_detect, Energy energy_info,
		       Energy energy_move, Energy energy_turn,
		       Energy energy_attack, Energy energy_eat,
		       Energy energy_egg)
{
  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
INSERT INTO Environment(time, size_x, size_y,\n\
                        mutations_probability, time_birth, time_mutate,\n\
                        time_laziness, energy_laziness,\n\
                        attack_multiplier,\n\
                        energy_nothing, energy_myself, energy_detect,\n\
                        energy_info, energy_move, energy_turn,\n\
                        energy_attack, energy_eat, energy_egg)\n\
VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
    sql.bind(1, static_cast<int>(time));
    sql.bind(2, static_cast<int>(size_x));
    sql.bind(3, static_cast<int>(size_y));

    sql.bind(4, mutations_probability);
    sql.bind(5, static_cast<int>(time_birth));
    sql.bind(6, static_cast<int>(time_mutate));

    sql.bind(7, static_cast<int>(time_laziness));
    sql.bind(8, static_cast<int>(energy_laziness));

    sql.bind(9, attack_multiplier);

    sql.bind(10, static_cast<int>(energy_nothing));
    sql.bind(11, static_cast<int>(energy_myself));
    sql.bind(12, static_cast<int>(energy_detect));
    sql.bind(13, static_cast<int>(energy_info));
    sql.bind(14, static_cast<int>(energy_move));
    sql.bind(15, static_cast<int>(energy_turn));
    sql.bind(16, static_cast<int>(energy_attack));
    sql.bind(17, static_cast<int>(energy_eat));
    sql.bind(18, static_cast<int>(energy_egg));

    sql.executenonquery();
    return db->insertid();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + db->errormsg() + ")");
  }
}

/**
 * Delete a environment.
 * @param db database.
 * @param id id of the environment.
 * @exception DBException if there is an error with the deletion.
 */
void Environment::remove(DB* db, ID id)
{
  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
DELETE FROM Environment\n\
WHERE id = ?;");
    sql.bind(1, id);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + db->errormsg() + ")");
  }
}


/**
 * Set the id of the environment.
 * @param id the id.
 * @exception DBException if there is an error with the update.
 */
void Environment::id(ID id)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Environment\n\
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
 * Get cycles since the creation of the World.
 * @return the cycles.
 * @exception DBException if there is an error with the query.
 */
Time Environment::time() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT time\n\
FROM Environment\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
time %1% not found in table Environment")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set cycles since the creation of the World.
 * @param time the new cycles.
 * @exception DBException if there is an error with the update.
 */
void Environment::time(Time time)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Environment\n\
SET time = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<int>(time));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}


/**
 * Get size of the World (x coord).
 * @return the size.
 * @exception DBException if there is an error with the query.
 */
Coord Environment::size_x() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT size_x\n\
FROM Environment\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
time %1% not found in table Environment")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Get size of the World (y coord).
 * @return the size.
 * @exception DBException if there is an error with the query.
 */
Coord Environment::size_y() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT size_y\n\
FROM Environment\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
time %1% not found in table Environment")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set size of the World (x coord).
 * @param size_x the new size.
 * @exception DBException if there is an error with the update.
 */
void Environment::size_x(Coord size_x)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Environment\n\
SET size_x = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<int>(size_x));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set size of the World (x coord).
 * @param size_y the new size.
 * @exception DBException if there is an error with the update.
 */
void Environment::size_y(Coord size_y)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Environment\n\
SET size_y = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<int>(size_y));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}


/**
 * Get the probability (0.0-1.0) that the code mutates.
 * @return the probability.
 * @exception DBException if there is an error with the query.
 */
double Environment::mutations_probability() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT mutations_probability\n\
FROM Environment\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
time %1% not found in table Environment")
                                              % this->id_));

    return cursor.getdouble(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set the probability (0.0-1.0) that the code mutates.
 * @param mutations_probability the new probability.
 * @exception DBException if there is an error with the update.
 */
void Environment::mutations_probability(double mutations_probability)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Environment\n\
SET mutations_probability = ?\n\
WHERE id = ?;");
    sql.bind(1, mutations_probability);
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}


/**
 * Get the cycles to convert a egg into a bug.
 * @return the cycles.
 * @exception DBException if there is an error with the query.
 */
Time Environment::time_birth() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT time_birth\n\
FROM Environment\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
time %1% not found in table Environment")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set the cycles to convert a egg into a bug.
 * @param time_birth the new cycles.
 * @exception DBException if there is an error with the update.
 */
void Environment::time_birth(Time time_birth)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Environment\n\
SET time_birth = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<int>(time_birth));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}


/**
 * Get the cycles to mutate the code of a old bug.
 * @return the cycles.
 * @exception DBException if there is an error with the query.
 */
Time Environment::time_mutate() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT time_mutate\n\
FROM Environment\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
time %1% not found in table Environment")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set the cycles to mutate the code of a old bug.
 * @param time_mutate the new time.
 * @exception DBException if there is an error with the update.
 */
void Environment::time_mutate(Time time_mutate)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Environment\n\
SET time_mutate = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<int>(time_mutate));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}


/**
 * Get the cycles without doing a action to consider a bug lazy.
 * @return the time.
 * @exception DBException if there is an error with the query.
 */
Time Environment::time_laziness() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT time_laziness\n\
FROM Environment\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
time %1% not found in table Environment")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set the cycles without doing a action to consider a bug lazy.
 * @param time_laziness the new time.
 * @exception DBException if there is an error with the update.
 */
void Environment::time_laziness(Time time_laziness)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Environment\n\
SET time_laziness = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<int>(time_laziness));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}


/**
 * Get the energy substracted for laziness.
 * @return the energy.
 * @exception DBException if there is an error with the query.
 */
Energy Environment::energy_laziness() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT energy_laziness\n\
FROM Environment\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
time %1% not found in table Environment")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set the energy substracted for laziness.
 * @param energy_laziness the new energy.
 * @exception DBException if there is an error with the update.
 */
void Environment::energy_laziness(Energy energy_laziness)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Environment\n\
SET energy_laziness = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<int>(energy_laziness));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}


/**
 * Get the multiplier for the energy of a attack.
 * @return the multiplier.
 * @exception DBException if there is an error with the query.
 */
double Environment::attack_multiplier() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT attack_multiplier\n\
FROM Environment\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
time %1% not found in table Environment")
                                              % this->id_));

    return cursor.getdouble(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set the multiplier for the energy of a attack.
 * @param attack_multiplier the new multiplier.
 * @exception DBException if there is an error with the update.
 */
void Environment::attack_multiplier(double attack_multiplier)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Environment\n\
SET attack_multiplier = ?\n\
WHERE id = ?;");
    sql.bind(1, attack_multiplier);
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}


/**
 * Get the energy used to do the action nothing.
 * @return the energy.
 * @exception DBException if there is an error with the query.
 */
Energy Environment::energy_nothing() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT energy_nothing\n\
FROM Environment\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
time %1% not found in table Environment")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set the energy used to do the action nothing.
 * @param energy_nothing the new energy.
 * @exception DBException if there is an error with the update.
 */
void Environment::energy_nothing(Energy energy_nothing)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Environment\n\
SET energy_nothing = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<int>(energy_nothing));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}


/**
 * Get the energy used to do the action myself.
 * @return the energy.
 * @exception DBException if there is an error with the query.
 */
Energy Environment::energy_myself() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT energy_myself\n\
FROM Environment\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
time %1% not found in table Environment")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set the energy used to do the action myself.
 * @param energy_myself the new energy.
 * @exception DBException if there is an error with the update.
 */
void Environment::energy_myself(Energy energy_myself)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Environment\n\
SET energy_myself = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<int>(energy_myself));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}


/**
 * Get the energy used to do the action detect.
 * @return the energy.
 * @exception DBException if there is an error with the query.
 */
Energy Environment::energy_detect() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT energy_detect\n\
FROM Environment\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
time %1% not found in table Environment")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set the energy used to do the action detect.
 * @param energy_detect the new energy.
 * @exception DBException if there is an error with the update.
 */
void Environment::energy_detect(Energy energy_detect)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Environment\n\
SET energy_detect = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<int>(energy_detect));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}


/**
 * Get the energy used to do the action info.
 * @return the energy.
 * @exception DBException if there is an error with the query.
 */
Energy Environment::energy_info() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT energy_info\n\
FROM Environment\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
time %1% not found in table Environment")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set the energy used to do the action info.
 * @param energy_info the new energy.
 * @exception DBException if there is an error with the update.
 */
void Environment::energy_info(Energy energy_info)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Environment\n\
SET energy_info = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<int>(energy_info));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}


/**
 * Get the energy used to do the action move.
 * @return the energy.
 * @exception DBException if there is an error with the query.
 */
Energy Environment::energy_move() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT energy_move\n\
FROM Environment\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
time %1% not found in table Environment")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set the energy used to do the action move.
 * @param energy_move the new energy.
 * @exception DBException if there is an error with the update.
 */
void Environment::energy_move(Energy energy_move)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Environment\n\
SET energy_move = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<int>(energy_move));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}


/**
 * Get the energy used to do the action turn.
 * @return the energy.
 * @exception DBException if there is an error with the query.
 */
Energy Environment::energy_turn() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT energy_turn\n\
FROM Environment\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
time %1% not found in table Environment")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set the energy used to do the action turn.
 * @param energy_turn the new energy.
 * @exception DBException if there is an error with the update.
 */
void Environment::energy_turn(Energy energy_turn)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Environment\n\
SET energy_turn = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<int>(energy_turn));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}


/**
 * Get the energy used to do the action attack.
 * @return the energy.
 * @exception DBException if there is an error with the query.
 */
Energy Environment::energy_attack() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT energy_attack\n\
FROM Environment\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
time %1% not found in table Environment")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set the energy used to do the action attack.
 * @param energy_attack the new energy.
 * @exception DBException if there is an error with the update.
 */
void Environment::energy_attack(Energy energy_attack)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Environment\n\
SET energy_attack = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<int>(energy_attack));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}


/**
 * Get the energy used to do the action eat.
 * @return the energy.
 * @exception DBException if there is an error with the query.
 */
Energy Environment::energy_eat() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT energy_eat\n\
FROM Environment\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
time %1% not found in table Environment")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set the energy used to do the action eat.
 * @param energy_eat the new energy.
 * @exception DBException if there is an error with the update.
 */
void Environment::energy_eat(Energy energy_eat)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Environment\n\
SET energy_eat = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<int>(energy_eat));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}


/**
 * Get the energy used to do the action egg.
 * @return the energy.
 * @exception DBException if there is an error with the query.
 */
Energy Environment::energy_egg() const
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT energy_egg\n\
FROM Environment\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw EXCEPTION(DBException, boost::str(boost::format("\
time %1% not found in table Environment")
                                              % this->id_));

    return cursor.getint(0);
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

/**
 * Set the energy used to do the action egg.
 * @param energy_egg the new energy.
 * @exception DBException if there is an error with the update.
 */
void Environment::energy_egg(Energy energy_egg)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
UPDATE Environment\n\
SET energy_egg = ?\n\
WHERE id = ?;");
    sql.bind(1, static_cast<int>(energy_egg));
    sql.bind(2, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(DBException, std::string(e.what()) +
                    " (" + this->db()->errormsg() + ")");
  }
}

}
}
