/**
 * @file simpleworld/db/db.hpp
 * Simple World Database management.
 *
 * begin:     Wed, 24 Jan 2007 17:13:02 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2007  Xosé Otero <xoseotero@users.sourceforge.net>
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

#ifndef __SIMPLEWORLD_DB_DB_HPP__
#define __SIMPLEWORLD_DB_DB_HPP__

#include <vector>

#include <sqlite3x.hpp>

#include <simpleworld/ints.hpp>
#include <simpleworld/types.hpp>
#include <simpleworld/db/types.hpp>

namespace SimpleWorld
{
/**
 * Namespace for Simple World Data Base.
 */
namespace DB
{

/**
 * Simple World Data Base management.
 */
class DB: public sqlite3x::sqlite3_connection
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
   * Version of the data base.
   * @return The version.
   */
  Uint8 version() const { return this->version_; }


  /**
   * List of all the environments (changes), ordered by it's time.
   * @return the list of environments.
   * @exception DBException if there is a error in the database.
   */
  std::vector<Time> environments();

  /**
   * Get the last environment (change).
   * @return the environment.
   * @exception DBException if there is a error in the database.
   * @exception DBException if there is any environments.
   */
  Time last_environment();


  /**
   * List of all the bugs, ordered by its birth.
   * @return the list of bugs.
   * @exception DBException if there is a error in the database.
   */
  std::vector<ID> bugs();

  /**
   * List of the alive bugs, ordered by its birth.
   * @return the list of bugs.
   * @exception DBException if there is a error in the database.
   */
  std::vector<ID> alive_bugs();

  /**
   * List of eggs, ordered by its birth.
   * @return the list of eggs.
   * @exception DBException if there is a error in the database.
   */
  std::vector<ID> eggs();


  /**
   * List of the food.
   * @return the list of food.
   * @exception DBException if there is a error in the database.
   */
  std::vector<ID> food();

protected:
  /**
   * This function is called when open() succeeds. Subclasses
   * which wish to do custom db initialization or sanity checks
   * may do them here.
   * @exception WrongVersion if the database version is not supported.
   */
  void on_open();

  /**
   * Create the tables.
   * @exception DBException The tables can't be created.
   */
  void create_tables();

private:
  Uint8 version_;
};

}
}

#endif // __SIMPLEWORLD_DB_DB_HPP__
