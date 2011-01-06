/**
 * @file simpleworld/db/stats.hpp
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

#ifndef SIMPLEWORLD_DB_STATS_HPP
#define SIMPLEWORLD_DB_STATS_HPP

#include <simpleworld/types.hpp>
#include <simpleworld/db/db.hpp>
#include <simpleworld/db/table.hpp>

namespace simpleworld
{
namespace db
{

/**
 * Information about the stats
 */
class Stats: public Table
{
public:
  /**
   * Constructor.
   * It's not checked if the id is in the table, only when accessing the data
   * the id is checked.
   * @param db database.
   * @param id id of the stats.
   */
  Stats(DB* db, ID id);


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
  static ID insert(DB* db, Time time, Uint32 alive, Uint32 eggs, Uint32 food,
                   Uint32 energy, Uint32 mutations, Uint32 age,
                   Uint32 last_births, Uint32 last_sons, Uint32 last_deaths,
                   Uint32 last_kills, Uint32 last_mutations);

  /**
   * Insert the current stats.
   * @param db database.
   * @return the id of the new row.
   * @exception DBException if there is an error with the insertion.
   */
  static ID insert(DB* db);

  /**
   * Delete stats.
   * @param db database.
   * @param id id of the stats.
   * @exception DBException if there is an error with the deletion.
   */
  static void remove(DB* db, ID id);


  /**
   * Get the id of the stats.
   * @return the id.
   * @exception DBException if there is an error with the query.
   */
  ID id() const { return db::Table::id(); }

  /**
   * Set the id of the stats.
   * @param id the new id.
   * @exception DBException if there is an error with the update.
   */
  void id(ID id);


  /**
   * Get the time.
   * @return the time.
   * @exception DBException if there is an error with the query.
   */
  Time time() const;

  /**
   * Set the time.
   * @param time the new time.
   * @exception DBException if there is an error with the query.
   */
  void time(Time time);


  /**
   * Get the number of alive bugs.
   * @return the number of alive bugs.
   * @exception DBException if there is an error with the query.
   */
  Uint32 alive() const;

  /**
   * Set the number of alive bugs.
   * @param alive the number of alive bugs.
   * @exception DBException if there is an error with the query.
   */
  void alive(Uint32 alive);

  /**
   * Get the number of eggs.
   * @return the number of egg.
   * @exception DBException if there is an error with the query.
   */
  Uint32 eggs() const;

  /**
   * Set the number of eggs.
   * @param eggs the number of egg.
   * @exception DBException if there is an error with the query.
   */
  void eggs(Uint32 eggs);

  /**
   * Get the number of food.
   * @return the number of food.
   * @exception DBException if there is an error with the query.
   */
  Uint32 food() const;

  /**
   * set the number of food.
   * @param food the number of food.
   * @exception DBException if there is an error with the query.
   */
  void food(Uint32 food);

  /**
   * Get the energy of the bugs/eggs.
   * @return the energy.
   * @exception DBException if there is an error with the query.
   */
  Uint32 energy() const;

  /**
   * Set the energy of the bugs/eggs.
   * @param energy the energy.
   * @exception DBException if there is an error with the query.
   */
  void energy(Uint32 energy);

  /**
   * Get the number of mutations of the bugs/eggs.
   * @return the number of mutations.
   * @exception DBException if there is an error with the query.
   */
  Uint32 mutations() const;

  /**
   * Set the number of mutations of the bugs/eggs.
   * @param mutations the number of mutations.
   * @exception DBException if there is an error with the query.
   */
  void mutations(Uint32 mutations);

  /**
   * Get the age of the bugs/eggs.
   * @return the age.
   * @exception DBException if there is an error with the query.
   */
  Uint32 age() const;

  /**
   * Set the age of the bugs/eggs.
   * @param age the age.
   * @exception DBException if there is an error with the query.
   */
  void age(Uint32 age);


  /**
   * Get the number of bugs born since the last entry.
   * @return the number of bugs born.
   * @exception DBException if there is an error with the query.
   */
  Uint32 last_births() const;

  /**
   * Set the number of bugs born since the last entry.
   * @param last_births the number of bugs born.
   * @exception DBException if there is an error with the query.
   */
  void last_births(Uint32 last_births);

  /**
   * Get the number of sons born since the last entry.
   * @return the number of sons born.
   * @exception DBException if there is an error with the query.
   */
  Uint32 last_sons() const;

  /**
   * Set the number of sons born since the last entry.
   * @param last_sons the number of sons born.
   * @exception DBException if there is an error with the query.
   */
  void last_sons(Uint32 last_sons);

  /**
   * Get the number of bugs dead since the last entry.
   * @return the number of bugs dead.
   * @exception DBException if there is an error with the query.
   */
  Uint32 last_deaths() const;

  /**
   * Set the number of bugs dead since the last entry.
   * @param last_deaths the number of bugs dead.
   * @exception DBException if there is an error with the query.
   */
  void last_deaths(Uint32 last_deaths);

  /**
   * Get the number of kills since the last entry.
   * @return the number of kills.
   * @exception DBException if there is an error with the query.
   */
  Uint32 last_kills() const;

  /**
   * Set the number of kills since the last entry.
   * @param last_kills the number of kills.
   * @exception DBException if there is an error with the query.
   */
  void last_kills(Uint32 last_kills);

  /**
   * Get the number of mutations since the last entry.
   * @return the number of mutations.
   * @exception DBException if there is an error with the query.
   */
  Uint32 last_mutations() const;

  /**
   * Set the number of mutations since the last entry.
   * @param last_mutations the number of mutations.
   * @exception DBException if there is an error with the query.
   */
  void last_mutations(Uint32 last_mutations);
};

}
}

#endif // SIMPLEWORLD_DB_STATS_HPP
