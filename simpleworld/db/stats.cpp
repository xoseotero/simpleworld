/**
 * @file simpleworld/db/stats.cpp
 * Information about the stats
 *
 *  Copyright (C) 2011  Xosé Otero <xoseotero@gmail.com>
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
#include "bug.hpp"
#include "stats.hpp"

namespace simpleworld
{
namespace db
{

/**
 * Constructor.
 * It's not checked if the id is in the table, only when accessing the data
 * the id is checked.
 * @param db database.
 * @param id id of the stats.
 */
Stats::Stats(DB* db, ID id)
  : Table("Stats", db, id)
{
}


/**
 * Insert stats.
 * @param db database.
 * @param time the time.
 * @param alive the number of alive bugs.
 * @param eggs the number of eggs.
 * @param food the number of food.
 * @param energy the energy of the bugs/eggs.
 * @param mutations the mutations of the bugs/eggs.
 * @param age the age of the bugs/eggs.
 * @param last_births the number of bugs born since the last entry.
 * @param last_sons the number of sons born since the last entry.
 * @param last_deaths the number of bugs dead since the last entry.
 * @param last_kills the number of kills since the last entry.
 * @param last_mutations the number of mutations since the last entry.
 * @return the id of the new row.
 * @exception DBException if there is an error with the insertion.
 */
ID Stats::insert(DB* db, Time time, Uint32 alive, Uint32 eggs, Uint32 food,
                 Uint32 energy, Uint32 mutations, Uint32 age,
                 Uint32 last_births, Uint32 last_sons, Uint32 last_deaths,
                 Uint32 last_kills, Uint32 last_mutations)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
INSERT INTO Stats(time, alive, eggs, food, energy, mutations, age,\n\
                  last_births, last_sons, last_deaths, last_kills,\n\
                  last_mutations)\n\
VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_bind_int(stmt, 1, time);
  sqlite3_bind_int(stmt, 2, alive);
  sqlite3_bind_int(stmt, 3, eggs);
  sqlite3_bind_int(stmt, 4, food);
  sqlite3_bind_int(stmt, 5, energy);
  sqlite3_bind_int(stmt, 6, mutations);
  sqlite3_bind_int(stmt, 7, age);
  sqlite3_bind_int(stmt, 8, last_births);
  sqlite3_bind_int(stmt, 9, last_sons);
  sqlite3_bind_int(stmt, 10, last_deaths);
  sqlite3_bind_int(stmt, 11, last_kills);
  sqlite3_bind_int(stmt, 12, last_mutations);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_finalize(stmt);

  return sqlite3_last_insert_rowid(db->db());
}

/**
 * Insert the current stats.
 * @param db database.
 * @return the id of the new row.
 * @exception DBException if there is an error with the insertion.
 */
ID Stats::insert(DB* db)
{
  sqlite3_stmt* stmt;

  if (sqlite3_prepare_v2(db->db(), "\
SELECT max(time)\n\
FROM Environment;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  Time time = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  if (sqlite3_prepare_v2(db->db(), "\
SELECT count(*)\n\
FROM AliveBug;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  Uint32 alive = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  if (sqlite3_prepare_v2(db->db(), "\
SELECT count(*)\n\
FROM Egg;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  Uint32 eggs = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  if (sqlite3_prepare_v2(db->db(), "\
SELECT count(*)\n\
FROM Food;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  Uint32 food = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  if (sqlite3_prepare_v2(db->db(), "\
SELECT total(energy)\n\
FROM AliveBug;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  Uint32 energy = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  Uint32 mutations = 0;
  std::vector<ID> alive_bugs = db->alive_bugs();
  for (std::vector<ID>::const_iterator alive_bug = alive_bugs.begin();
       alive_bug != alive_bugs.end();
       ++alive_bug)
    mutations += Bug(db, *alive_bug).all_mutations().size();

  if (sqlite3_prepare_v2(db->db(), "\
SELECT total((SELECT max(time)\n\
              FROM Environment) - birth)\n\
FROM AliveBug;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  Uint32 age = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  if (sqlite3_prepare_v2(db->db(), "\
SELECT count(*) - (SELECT total(last_deaths)\n\
                   FROM Stats)\n\
FROM DeadBug;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  Uint32 last_deaths = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  if (sqlite3_prepare_v2(db->db(), "\
SELECT count(*) - (SELECT total(last_kills)\n\
                   FROM Stats)\n\
FROM DeadBug\n\
WHERE killer_id IS NOT NULL;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  Uint32 last_kills = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  if (sqlite3_prepare_v2(db->db(), "\
SELECT count(*) +\n\
       (SELECT count(*)\n\
        FROM DeadBug) -\n\
       (SELECT total(last_births)\n\
        FROM Stats)\n\
FROM AliveBug;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  Uint32 last_births = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  if (sqlite3_prepare_v2(db->db(), "\
SELECT count(*) +\n\
       (SELECT count(*)\n\
        FROM DeadBug\n\
        WHERE bug_id IN (SELECT id\n\
                         FROM Bug\n\
                         WHERE father_id IS NOT NULL)) -\n\
       (SELECT total(last_sons)\n\
        FROM Stats)\n\
FROM AliveBug\n\
WHERE bug_id IN (SELECT id\n\
                 FROM Bug\n\
                 WHERE father_id IS NOT NULL);", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  Uint32 last_sons = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  if (sqlite3_prepare_v2(db->db(), "\
SELECT count(*) - (SELECT total(last_mutations)\n\
                   FROM Stats)\n\
FROM Mutation;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  Uint32 last_mutations = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return Stats::insert(db, time, alive, eggs, food, energy, mutations, age,
                       last_births, last_sons, last_deaths, last_kills,
                       last_mutations);
}

/**
 * Delete stats.
 * @param db database.
 * @param id id of the stats.
 * @exception DBException if there is an error with the deletion.
 */
void Stats::remove(DB* db, ID id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db->db(), "\
DELETE FROM Stats\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_bind_int64(stmt, 1, id);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(db->db()));
  sqlite3_finalize(stmt);
}


/**
 * Set the id of the stats.
 * @param id the new id.
 * @exception DBException if there is an error with the update.
 */
void Stats::id(ID id)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Stats\n\
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
 * Get the time.
 * @return the time.
 * @exception DBException if there is an error with the query.
 */
Time Stats::time() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT time\n\
FROM Stats\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Stats")
                                            % this->id_));
  Time time = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return time;
}

/**
 * Set the time.
 * @param time the new time.
 * @exception DBException if there is an error with the query.
 */
void Stats::time(Time time)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Stats\n\
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
 * Get the number of alive bugs.
 * @return the number of alive bugs.
 * @exception DBException if there is an error with the query.
 */
Uint32 Stats::alive() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT alive\n\
FROM Stats\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Stats")
                                            % this->id_));
  Uint32 alive = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return alive;
}

/**
 * Set the number of alive bugs.
 * @param alive the number of alive bugs.
 * @exception DBException if there is an error with the query.
 */
void Stats::alive(Uint32 alive)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Stats\n\
SET alive = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, alive);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

/**
 * Get the number of eggs.
 * @return the number of egg.
 * @exception DBException if there is an error with the query.
 */
Uint32 Stats::eggs() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT eggs\n\
FROM Stats\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Stats")
                                            % this->id_));
  Uint32 eggs = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return eggs;
}

/**
 * Set the number of eggs.
 * @param eggs the number of egg.
 * @exception DBException if there is an error with the query.
 */
void Stats::eggs(Uint32 eggs)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Stats\n\
SET eggs = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, eggs);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

/**
 * Get the number of food.
 * @return the number of food.
 * @exception DBException if there is an error with the query.
 */
Uint32 Stats::food() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT food\n\
FROM Stats\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Stats")
                                            % this->id_));
  Uint32 food = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return food;
}

/**
 * set the number of food.
 * @param food the number of food.
 * @exception DBException if there is an error with the query.
 */
void Stats::food(Uint32 food)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Stats\n\
SET food = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, food);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

/**
 * Get the energy of the bugs/eggs.
 * @return the energy.
 * @exception DBException if there is an error with the query.
 */
Uint32 Stats::energy() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT energy\n\
FROM Stats\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Stats")
                                            % this->id_));
  Uint32 energy = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return energy;
}

/**
 * Set the energy of the bugs/eggs.
 * @param energy the energy.
 * @exception DBException if there is an error with the query.
 */
void Stats::energy(Uint32 energy)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Stats\n\
SET energy = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, energy);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

/**
 * Get the number of mutations of the bugs/eggs.
 * @return the number of mutations.
 * @exception DBException if there is an error with the query.
 */
Uint32 Stats::mutations() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT mutations\n\
FROM Stats\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Stats")
                                            % this->id_));
  Uint32 mutations = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return mutations;
}

/**
 * Set the number of mutations of the bugs/eggs.
 * @param mutations the number of mutations.
 * @exception DBException if there is an error with the query.
 */
void Stats::mutations(Uint32 mutations)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Stats\n\
SET mutations = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, mutations);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

/**
 * Get the age of the bugs/eggs.
 * @return the age.
 * @exception DBException if there is an error with the query.
 */
Uint32 Stats::age() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT age\n\
FROM Stats\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Stats")
                                            % this->id_));
  Uint32 age = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return age;
}

/**
 * Set the age of the bugs/eggs.
 * @param age the age.
 * @exception DBException if there is an error with the query.
 */
void Stats::age(Uint32 age)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Stats\n\
SET age = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, age);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

/**
 * Get the number of bugs born since the last entry.
 * @return the number of bugs born.
 * @exception DBException if there is an error with the query.
 */
Uint32 Stats::last_births() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT last_births\n\
FROM Stats\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Stats")
                                            % this->id_));
  Uint32 last_births = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return last_births;
}

/**
 * Set the number of bugs born since the last entry.
 * @param last_births the number of bugs born.
 * @exception DBException if there is an error with the query.
 */
void Stats::last_births(Uint32 last_births)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Stats\n\
SET last_births = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, last_births);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

/**
 * Get the number of sons born since the last entry.
 * @return the number of sons born.
 * @exception DBException if there is an error with the query.
 */
Uint32 Stats::last_sons() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT last_sons\n\
FROM Stats\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Stats")
                                            % this->id_));
  Uint32 last_sons = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return last_sons;
}

/**
 * Set the number of sons born since the last entry.
 * @param last_sons the number of sons born.
 * @exception DBException if there is an error with the query.
 */
void Stats::last_sons(Uint32 last_sons)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Stats\n\
SET last_sons = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, last_sons);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

/**
 * Get the number of bugs dead since the last entry.
 * @return the number of bugs dead.
 * @exception DBException if there is an error with the query.
 */
Uint32 Stats::last_deaths() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT last_deaths\n\
FROM Stats\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Stats")
                                            % this->id_));
  Uint32 last_deaths = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return last_deaths;
}

/**
 * Set the number of bugs dead since the last entry.
 * @param last_deaths the number of bugs dead.
 * @exception DBException if there is an error with the query.
 */
void Stats::last_deaths(Uint32 last_deaths)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Stats\n\
SET last_deaths = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, last_deaths);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

/**
 * Get the number of kills since the last entry.
 * @return the number of kills.
 * @exception DBException if there is an error with the query.
 */
Uint32 Stats::last_kills() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT last_kills\n\
FROM Stats\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Stats")
                                            % this->id_));
  Uint32 last_kills = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return last_kills;
}

/**
 * Set the number of kills since the last entry.
 * @param last_kills the number of kills.
 * @exception DBException if there is an error with the query.
 */
void Stats::last_kills(Uint32 last_kills)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Stats\n\
SET last_kills = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, last_kills);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

/**
 * Get the number of mutations since the last entry.
 * @return the number of mutations.
 * @exception DBException if there is an error with the query.
 */
Uint32 Stats::last_mutations() const
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
SELECT last_mutations\n\
FROM Stats\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int64(stmt, 1, this->id_);
  if (sqlite3_step(stmt) != SQLITE_ROW)
    throw EXCEPTION(DBException, boost::str(boost::format("\
id %1% not found in table Stats")
                                            % this->id_));
  Uint32 last_mutations = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return last_mutations;
}

/**
 * Set the number of mutations since the last entry.
 * @param last_mutations the number of mutations.
 * @exception DBException if there is an error with the query.
 */
void Stats::last_mutations(Uint32 last_mutations)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(this->db_->db(), "\
UPDATE Stats\n\
SET last_mutations = ?\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_bind_int(stmt, 1, last_mutations);
  sqlite3_bind_int64(stmt, 2, this->id_);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    throw EXCEPTION(DBException, sqlite3_errmsg(this->db_->db()));
  sqlite3_finalize(stmt);
}

}
}
