/**
 * @file simpleworld/db/db.hpp
 * Simple World Database management.
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

#ifndef SIMPLEWORLD_DB_DB_HPP
#define SIMPLEWORLD_DB_DB_HPP

#include <string>
#include <vector>

#include <sqlite3.h>

#include <simpleworld/ints.hpp>
#include <simpleworld/types.hpp>
#include <simpleworld/db/types.hpp>

namespace simpleworld
{
/**
 * Namespace for Simple World Data Base.
 */
namespace db
{

/**
 * Simple World Data Base management.
 */
class DB
{
public:
  /**
   * Constructor.
   * @param filename File name of the database.
   * @exception DBException if the database can't be opened.
   * @exception DBException if there is a error in the database.
   * @exception WrongVersion if the database version is not supported.
   */
  DB(std::string filename);

  /**
   * Destructor.
   */
  ~DB();


  /**
   * Get the sqlite3 database connection handler.
   */
  sqlite3* db() const { return this->db_; }

  /**
   * Version of the data base.
   * @return The version.
   */
  Uint8 version() const { return this->version_; }


  /**
   * List of all the environments (changes), ordered by it's time.
   * @return the list of environments.
   * @exception DBException if there is a error in the database.
   */
  std::vector<ID> environments();

  /**
   * Get the last environment (change).
   * @return the environment.
   * @exception DBException if there is a error in the database.
   * @exception DBException if there isn't any environments.
   */
  ID last_environment();


  /**
   * List of eggs, ordered by its conception time.
   * @return the list of eggs.
   * @exception DBException if there is a error in the database.
   */
  std::vector<ID> eggs();

  /**
   * List of the alive bugs, ordered by its birth.
   * @return the list of bugs.
   * @exception DBException if there is a error in the database.
   */
  std::vector<ID> alive_bugs();

  /**
   * List of the dead bugs, ordered by its death.
   * @return the list of bugs.
   * @exception DBException if there is a error in the database.
   */
  std::vector<ID> dead_bugs();


  /**
   * List of the food.
   * @return the list of food.
   * @exception DBException if there is a error in the database.
   */
  std::vector<ID> food();

private:
  sqlite3* db_;                 /**< Database connection */
  Uint8 version_;               /**< Version of the database */
};

}
}

#endif // SIMPLEWORLD_DB_DB_HPP
