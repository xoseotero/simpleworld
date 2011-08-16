/**
 * @file simpleworld/db/environment.cpp
 * Information about the environment of the world.
 *
 *  Copyright (C) 2007-2011  Xosé Otero <xoseotero@gmail.com>
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

#include <iostream>
#include <string>

#include <boost/format.hpp>

#include <sqlite3.h>

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
 * @param id id of the environment.
 */
Environment::Environment(DB* db, ID id)
  : Table("Environment", db, id)
{
}


/**
 * Insert a environment.
 * @param db database.
 * @param time time passed since the creation of the World.
 * @param size_x size of the World (x coord).
 * @param size_y size of the World (y coord).
 * @param time_rot time needed to rot the food.
 * @param size_rot size that is substracted to the food.
 * @param mutations_probability probability (0.0-1.0) that the code mutates.
 * @param time_birth time needed to convert a egg into a bug.
 * @param time_mutate time to mutate the code of a old bug.
 * @param time_laziness time without doing a action to consider a bug lazy.
 * @param energy_laziness energy substracted for laziness.
 * @param attack_multiplier multiplier for the energy of a attack.
 * @param time_nothing time needed to do the action nothing.
 * @param time_myself time needed to do the action myself.
 * @param time_detect time needed to do the action detect.
 * @param time_info time needed to do the action info.
 * @param time_move time needed to do the action move.
 * @param time_turn time needed to do the action turn.
 * @param time_attack time needed to do the action attack.
 * @param time_eat time needed to do the action eat.
 * @param time_egg time needed to do the action egg.
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
                       Time time_rot, Energy size_rot,
                       double mutations_probability, Time time_birth,
                       Time time_mutate, Time time_laziness,
                       Energy energy_laziness, double attack_multiplier,
                       Time time_nothing, Time time_myself, Time time_detect,
                       Time time_info, Time time_move, Time time_turn,
                       Time time_attack, Time time_eat, Time time_egg,
                       Energy energy_nothing, Energy energy_myself,
                       Energy energy_detect, Energy energy_info,
                       Energy energy_move, Energy energy_turn,
                       Energy energy_attack, Energy energy_eat,
                       Energy energy_egg)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
INSERT INTO Environment(time, size_x, size_y,\n\
                        time_rot, size_rot, mutations_probability,\n\
                        time_birth, time_mutate, time_laziness,\n\
                        energy_laziness, attack_multiplier,\n\
                        time_nothing, time_myself, time_detect, time_info,\n\
                        time_move, time_turn, time_attack, time_eat,\n\
                        time_egg,\n\
                        energy_nothing, energy_myself, energy_detect,\n\
                        energy_info, energy_move, energy_turn,\n\
                        energy_attack, energy_eat, energy_egg)\n\
VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?,\n\
       ?, ?, ?, ?, ?, ?);", -1,
                         &stmt, NULL)) {
    std::cout << sqlite3_errmsg(db->db()) << std::endl;
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  }
  sqlite3_bind_int(stmt, 1, time);
  sqlite3_bind_int(stmt, 2, size_x);
  sqlite3_bind_int(stmt, 3, size_y);
  sqlite3_bind_int(stmt, 4, time_rot);
  sqlite3_bind_int(stmt, 5, size_rot);
  sqlite3_bind_double(stmt, 6, mutations_probability);
  sqlite3_bind_int(stmt, 7, time_birth);
  sqlite3_bind_int(stmt, 8, time_mutate);
  sqlite3_bind_int(stmt, 9, time_laziness);
  sqlite3_bind_int(stmt, 10, energy_laziness);
  sqlite3_bind_double(stmt, 11, attack_multiplier);
  sqlite3_bind_int(stmt, 12, time_nothing);
  sqlite3_bind_int(stmt, 13, time_myself);
  sqlite3_bind_int(stmt, 14, time_detect);
  sqlite3_bind_int(stmt, 15, time_info);
  sqlite3_bind_int(stmt, 16, time_move);
  sqlite3_bind_int(stmt, 17, time_turn);
  sqlite3_bind_int(stmt, 18, time_attack);
  sqlite3_bind_int(stmt, 19, time_eat);
  sqlite3_bind_int(stmt, 20, time_egg);
  sqlite3_bind_int(stmt, 21, energy_nothing);
  sqlite3_bind_int(stmt, 22, energy_myself);
  sqlite3_bind_int(stmt, 23, energy_detect);
  sqlite3_bind_int(stmt, 24, energy_info);
  sqlite3_bind_int(stmt, 25, energy_move);
  sqlite3_bind_int(stmt, 26, energy_turn);
  sqlite3_bind_int(stmt, 27, energy_attack);
  sqlite3_bind_int(stmt, 28, energy_eat);
  sqlite3_bind_int(stmt, 29, energy_egg);
  if (sqlite3_step(stmt) != SQLITE_DONE) {
    std::cout << sqlite3_errmsg(db->db()) << std::endl;;
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  }
  sqlite3_finalize(stmt);

  return sqlite3_last_insert_rowid(db->db());
}

/**
 * Delete a environment.
 * @param db database.
 * @param id id of the environment.
 * @exception DBException if there is an error with the deletion.
 */
void Environment::remove(DB* db, ID id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
DELETE FROM Environment\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_bind_int64(stmt, 1, id);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_finalize(stmt);
}


/**
 * Set the id of the environment.
 * @param id the id.
 * @exception DBException if there is an error with the update.
 */
void Environment::id(ID id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Environment\n\
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
 * Get the time passed since the creation of the World.
 * @return the time.
 * @exception DBException if there is an error with the query.
 */
Time Environment::time() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT time\n\
FROM Environment\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Environment")
                                            % this->id_));
  Time time = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return time;
}

/**
 * Set the time passed since the creation of the World.
 * @param time the new time.
 * @exception DBException if there is an error with the update.
 */
void Environment::time(Time time)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Environment\n\
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
 * Get size of the World (x coord).
 * @return the size.
 * @exception DBException if there is an error with the query.
 */
Coord Environment::size_x() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT size_x\n\
FROM Environment\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Environment")
                                            % this->id_));
  Coord size_x = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return size_x;
}

/**
 * Get size of the World (y coord).
 * @return the size.
 * @exception DBException if there is an error with the query.
 */
Coord Environment::size_y() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT size_y\n\
FROM Environment\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Environment")
                                            % this->id_));
  Coord size_y = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return size_y;
}

/**
 * Set size of the World (x coord).
 * @param size_x the new size.
 * @exception DBException if there is an error with the update.
 */
void Environment::size_x(Coord size_x)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Environment\n\
SET size_x = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, size_x);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

/**
 * Set size of the World (x coord).
 * @param size_y the new size.
 * @exception DBException if there is an error with the update.
 */
void Environment::size_y(Coord size_y)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Environment\n\
SET size_y = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, size_y);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
* Get the time needed needed to rot the food.
* @return the time.
* @exception DBException if there is an error with the update.
*/
Time Environment::time_rot() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
    SELECT time_rot\n\
    FROM Environment\n\
    WHERE id = ?;", -1, &stmt, NULL)) {
    std::cout << sqlite3_errmsg(this->db_->db()) << std::endl;
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  }
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
    id %1% not found in table Environment")
    % this->id_));
  Time time_rot = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return time_rot;
}

/**
* Get the time needed to rot the food.
* @param time_rot the new time.
* @exception DBException if there is an error with the update.
*/
void Environment::time_rot(Time time_rot)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
    UPDATE Environment\n\
    SET time_rot = ?\n\
    WHERE id = ?;", -1, &stmt, NULL)) {
    std::cout << sqlite3_errmsg(this->db_->db()) << std::endl;
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  }
  sqlite3_bind_int(stmt, 1, time_rot);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE) {
    std::cout << sqlite3_errmsg(this->db_->db()) << std::endl;
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  }
  sqlite3_finalize(stmt);
}

/**
* Get the size that is substracted to the food.
* @return the size.
* @exception DBException if there is an error with the update.
*/
Energy Environment::size_rot() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
    SELECT size_rot\n\
    FROM Environment\n\
    WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
    id %1% not found in table Environment")
    % this->id_));
  Energy size_rot = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return size_rot;
}

/**
* Set the size that is substracted to the food.
* @param size_rot the new size.
* @exception DBException if there is an error with the update.
*/
void Environment::size_rot(Energy size_rot)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
    UPDATE Environment\n\
    SET size_rot = ?\n\
    WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, size_rot);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
 * Get the probability (0.0-1.0) that the code mutates.
 * @return the probability.
 * @exception DBException if there is an error with the query.
 */
double Environment::mutations_probability() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT mutations_probability\n\
FROM Environment\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Environment")
                                            % this->id_));
  double mutations_probability = sqlite3_column_double(stmt, 0);
  sqlite3_finalize(stmt);

  return mutations_probability;
}

/**
 * Set the probability (0.0-1.0) that the code mutates.
 * @param mutations_probability the new probability.
 * @exception DBException if there is an error with the update.
 */
void Environment::mutations_probability(double mutations_probability)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Environment\n\
SET mutations_probability = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_double(stmt, 1, mutations_probability);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
 * Get the time needed to convert a egg into a bug.
 * @return the time.
 * @exception DBException if there is an error with the query.
 */
Time Environment::time_birth() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT time_birth\n\
FROM Environment\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Environment")
                                            % this->id_));
  Time time_birth = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return time_birth;
}

/**
 * Set the time needed to convert a egg into a bug.
 * @param time_birth the new time.
 * @exception DBException if there is an error with the update.
 */
void Environment::time_birth(Time time_birth)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Environment\n\
SET time_birth = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, time_birth);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
 * Get the time to mutate the code of a old bug.
 * @return the time.
 * @exception DBException if there is an error with the query.
 */
Time Environment::time_mutate() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT time_mutate\n\
FROM Environment\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Environment")
                                            % this->id_));
  Time time_mutate = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return time_mutate;
}

/**
 * Set the time to mutate the code of a old bug.
 * @param time_mutate the new time.
 * @exception DBException if there is an error with the update.
 */
void Environment::time_mutate(Time time_mutate)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Environment\n\
SET time_mutate = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, time_mutate);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
 * Get the time without doing a action to consider a bug lazy.
 * @return the time.
 * @exception DBException if there is an error with the query.
 */
Time Environment::time_laziness() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT time_laziness\n\
FROM Environment\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Environment")
                                            % this->id_));
  Time time_laziness = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return time_laziness;
}

/**
 * Set the time without doing a action to consider a bug lazy.
 * @param time_laziness the new time.
 * @exception DBException if there is an error with the update.
 */
void Environment::time_laziness(Time time_laziness)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Environment\n\
SET time_laziness = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, time_laziness);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
 * Get the energy substracted for laziness.
 * @return the energy.
 * @exception DBException if there is an error with the query.
 */
Energy Environment::energy_laziness() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT energy_laziness\n\
FROM Environment\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Environment")
                                            % this->id_));
  Energy energy_laziness = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return energy_laziness;
}

/**
 * Set the energy substracted for laziness.
 * @param energy_laziness the new energy.
 * @exception DBException if there is an error with the update.
 */
void Environment::energy_laziness(Energy energy_laziness)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Environment\n\
SET energy_laziness = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, energy_laziness);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
 * Get the multiplier for the energy of a attack.
 * @return the multiplier.
 * @exception DBException if there is an error with the query.
 */
double Environment::attack_multiplier() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT attack_multiplier\n\
FROM Environment\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Environment")
                                            % this->id_));
  double attack_multiplier = sqlite3_column_double(stmt, 0);
  sqlite3_finalize(stmt);

  return attack_multiplier;
}

/**
 * Set the multiplier for the energy of a attack.
 * @param attack_multiplier the new multiplier.
 * @exception DBException if there is an error with the update.
 */
void Environment::attack_multiplier(double attack_multiplier)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Environment\n\
SET attack_multiplier = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_double(stmt, 1, attack_multiplier);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
* Get the time needed to do the action nothing.
* @return the time.
* @exception DBException if there is an error with the query.
*/
Time Environment::time_nothing() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
    SELECT time_nothing\n\
    FROM Environment\n\
    WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
    id %1% not found in table Environment")
    % this->id_));
  Time time_nothing = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return time_nothing;
}

/**
* Set the time needed to do the action nothing.
* @param time_nothing the new time.
* @exception DBException if there is an error with the update.
*/
void Environment::time_nothing(Time time_nothing)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
    UPDATE Environment\n\
    SET time_nothing = ?\n\
    WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, time_nothing);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
* Get the time needed to do the action myself.
* @return the time.
* @exception DBException if there is an error with the query.
*/
Time Environment::time_myself() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
    SELECT time_myself\n\
    FROM Environment\n\
    WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
    id %1% not found in table Environment")
    % this->id_));
  Time time_myself = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return time_myself;
}

/**
* Set the time needed to do the action myself.
* @param time_myself the new time.
* @exception DBException if there is an error with the update.
*/
void Environment::time_myself(Time time_myself)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
    UPDATE Environment\n\
    SET time_myself = ?\n\
    WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, time_myself);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
* Get the time needed to do the action detect.
* @return the time.
* @exception DBException if there is an error with the query.
*/
Time Environment::time_detect() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
    SELECT time_detect\n\
    FROM Environment\n\
    WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
    id %1% not found in table Environment")
    % this->id_));
  Time time_detect = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return time_detect;
}

/**
* Set the time needed to do the action detect.
* @param time_detect the new time.
* @exception DBException if there is an error with the update.
*/
void Environment::time_detect(Time time_detect)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
    UPDATE Environment\n\
    SET time_detect = ?\n\
    WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, time_detect);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
* Get the time needed to do the action info.
* @return the time.
* @exception DBException if there is an error with the query.
*/
Time Environment::time_info() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
    SELECT time_info\n\
    FROM Environment\n\
    WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
    id %1% not found in table Environment")
    % this->id_));
  Time time_info = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return time_info;
}

/**
* Set the time needed to do the action info.
* @param time_info the new time.
* @exception DBException if there is an error with the update.
*/
void Environment::time_info(Time time_info)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
    UPDATE Environment\n\
    SET time_info = ?\n\
    WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, time_info);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
* Get the time needed to do the action move.
* @return the time.
* @exception DBException if there is an error with the query.
*/
Time Environment::time_move() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
    SELECT time_move\n\
    FROM Environment\n\
    WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
    id %1% not found in table Environment")
    % this->id_));
  Time time_move = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return time_move;
}

/**
* Set the time needed to do the action move.
* @param time_move the new time.
* @exception DBException if there is an error with the update.
*/
void Environment::time_move(Time time_move)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
    UPDATE Environment\n\
    SET time_move = ?\n\
    WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, time_move);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
* Get the time needed to do the action turn.
* @return the time.
* @exception DBException if there is an error with the query.
*/
Time Environment::time_turn() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
    SELECT time_turn\n\
    FROM Environment\n\
    WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
    id %1% not found in table Environment")
    % this->id_));
  Time time_turn = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return time_turn;
}

/**
* Set the time needed to do the action turn.
* @param time_turn the new time.
* @exception DBException if there is an error with the update.
*/
void Environment::time_turn(Time time_turn)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
    UPDATE Environment\n\
    SET time_turn = ?\n\
    WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, time_turn);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
* Get the time needed to do the action attack.
* @return the Time.
* @exception DBException if there is an error with the query.
*/
Time Environment::time_attack() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
    SELECT time_attack\n\
    FROM Environment\n\
    WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
    id %1% not found in table Environment")
    % this->id_));
  Time time_attack = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return time_attack;
}

/**
* Set the time needed to do the action attack.
* @param time_attack the new time.
* @exception DBException if there is an error with the update.
*/
void Environment::time_attack(Time time_attack)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
    UPDATE Environment\n\
    SET time_attack = ?\n\
    WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, time_attack);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
* Get the time needed to do the action eat.
* @return the time.
* @exception DBException if there is an error with the query.
*/
Time Environment::time_eat() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
    SELECT time_eat\n\
    FROM Environment\n\
    WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
    id %1% not found in table Environment")
    % this->id_));
  Time time_eat = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return time_eat;
}

/**
* Set the time needed to do the action eat.
* @param energy_eat the new time.
* @exception DBException if there is an error with the update.
*/
void Environment::time_eat(Time time_eat)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
    UPDATE Environment\n\
    SET time_eat = ?\n\
    WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, time_eat);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
* Get the time needed to do the action egg.
* @return the time.
* @exception DBException if there is an error with the query.
*/
Time Environment::time_egg() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
    SELECT time_egg\n\
    FROM Environment\n\
    WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
    id %1% not found in table Environment")
    % this->id_));
  Time time_egg = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return time_egg;
}

/**
* Set the time needed to do the action egg.
* @param time_egg the new time.
* @exception DBException if there is an error with the update.
*/
void Environment::time_egg(Time time_egg)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
    UPDATE Environment\n\
    SET time_egg = ?\n\
    WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, time_egg);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
 * Get the energy used to do the action nothing.
 * @return the energy.
 * @exception DBException if there is an error with the query.
 */
Energy Environment::energy_nothing() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT energy_nothing\n\
FROM Environment\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Environment")
                                            % this->id_));
  Energy energy_nothing = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return energy_nothing;
}

/**
 * Set the energy used to do the action nothing.
 * @param energy_nothing the new energy.
 * @exception DBException if there is an error with the update.
 */
void Environment::energy_nothing(Energy energy_nothing)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Environment\n\
SET energy_nothing = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, energy_nothing);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
 * Get the energy used to do the action myself.
 * @return the energy.
 * @exception DBException if there is an error with the query.
 */
Energy Environment::energy_myself() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT energy_myself\n\
FROM Environment\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Environment")
                                            % this->id_));
  Energy energy_myself = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return energy_myself;
}

/**
 * Set the energy used to do the action myself.
 * @param energy_myself the new energy.
 * @exception DBException if there is an error with the update.
 */
void Environment::energy_myself(Energy energy_myself)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Environment\n\
SET energy_myself = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, energy_myself);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
 * Get the energy used to do the action detect.
 * @return the energy.
 * @exception DBException if there is an error with the query.
 */
Energy Environment::energy_detect() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT energy_detect\n\
FROM Environment\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Environment")
                                            % this->id_));
  Energy energy_detect = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return energy_detect;
}

/**
 * Set the energy used to do the action detect.
 * @param energy_detect the new energy.
 * @exception DBException if there is an error with the update.
 */
void Environment::energy_detect(Energy energy_detect)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Environment\n\
SET energy_detect = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, energy_detect);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
 * Get the energy used to do the action info.
 * @return the energy.
 * @exception DBException if there is an error with the query.
 */
Energy Environment::energy_info() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT energy_info\n\
FROM Environment\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Environment")
                                            % this->id_));
  Energy energy_info = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return energy_info;
}

/**
 * Set the energy used to do the action info.
 * @param energy_info the new energy.
 * @exception DBException if there is an error with the update.
 */
void Environment::energy_info(Energy energy_info)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Environment\n\
SET energy_info = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, energy_info);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
 * Get the energy used to do the action move.
 * @return the energy.
 * @exception DBException if there is an error with the query.
 */
Energy Environment::energy_move() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT energy_move\n\
FROM Environment\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Environment")
                                            % this->id_));
  Energy energy_move = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return energy_move;
}

/**
 * Set the energy used to do the action move.
 * @param energy_move the new energy.
 * @exception DBException if there is an error with the update.
 */
void Environment::energy_move(Energy energy_move)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Environment\n\
SET energy_move = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, energy_move);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
 * Get the energy used to do the action turn.
 * @return the energy.
 * @exception DBException if there is an error with the query.
 */
Energy Environment::energy_turn() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT energy_turn\n\
FROM Environment\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Environment")
                                            % this->id_));
  Energy energy_turn = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return energy_turn;
}

/**
 * Set the energy used to do the action turn.
 * @param energy_turn the new energy.
 * @exception DBException if there is an error with the update.
 */
void Environment::energy_turn(Energy energy_turn)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Environment\n\
SET energy_turn = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, energy_turn);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
 * Get the energy used to do the action attack.
 * @return the energy.
 * @exception DBException if there is an error with the query.
 */
Energy Environment::energy_attack() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT energy_attack\n\
FROM Environment\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Environment")
                                            % this->id_));
  Energy energy_attack = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return energy_attack;
}

/**
 * Set the energy used to do the action attack.
 * @param energy_attack the new energy.
 * @exception DBException if there is an error with the update.
 */
void Environment::energy_attack(Energy energy_attack)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Environment\n\
SET energy_attack = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, energy_attack);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
 * Get the energy used to do the action eat.
 * @return the energy.
 * @exception DBException if there is an error with the query.
 */
Energy Environment::energy_eat() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT energy_eat\n\
FROM Environment\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Environment")
                                            % this->id_));
  Energy energy_eat = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return energy_eat;
}

/**
 * Set the energy used to do the action eat.
 * @param energy_eat the new energy.
 * @exception DBException if there is an error with the update.
 */
void Environment::energy_eat(Energy energy_eat)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Environment\n\
SET energy_eat = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, energy_eat);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}


/**
 * Get the energy used to do the action egg.
 * @return the energy.
 * @exception DBException if there is an error with the query.
 */
Energy Environment::energy_egg() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT energy_egg\n\
FROM Environment\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Environment")
                                            % this->id_));
  Energy energy_egg = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return energy_egg;
}

/**
 * Set the energy used to do the action egg.
 * @param energy_egg the new energy.
 * @exception DBException if there is an error with the update.
 */
void Environment::energy_egg(Energy energy_egg)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Environment\n\
SET energy_egg = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, energy_egg);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

}
}
