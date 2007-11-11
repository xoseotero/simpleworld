/**
 * @file db/table.hpp
 * Base clase for the tables.
 *
 * begin:     Tue, 13 Feb 2007 11:11:22 +0100
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

#ifndef __DB_TABLE_HPP__
#define __DB_TABLE_HPP__

#include <vector>
#include <string>
#include <algorithm>

#include <db/types.hpp>
#include <db/exception.hpp>
#include <db/db.hpp>

namespace SimpleWorld
{
namespace DB
{

/**
 * Base clase for the tables.
 */
class Table
{
public:
  /**
   * Constructor.
   * @param db database.
   * @param id id of the row.
   * @exception DBError if there is a error in the database.
   * @exception IDNotFound if the ID is not found in the table.
   */
  Table(DB* db, ID id);

  /**
   * Destructor.
  */
  virtual ~Table();


  /**
   * The database of the table.
   * @return the database object.
   */
  DB* db() const { return this->db_; }

  /**
   * The id of the table.
   * @return the ID.
   */
  ID id() const { return this->id_; }


  /**
   * Update the data of the class with the database.
   * changed is set to false.
   * @exception DBError if there is a error in the database.
   * @exception IDNotFound if the ID is not found in the table.
   */
  virtual void update();

  /**
   * Update the database with the data of the class in changed or force are
   * true.
   * changed is set to false.
   * @param force force the update of the database.
   * @exception DBError if there is a error in the database.
   */
  virtual void update_db(bool force = false);


  /**
   * Check if colname is NULL.
   * @param colname name of the column.
   * @return true if colname is NULL, else false.
   */
  bool is_null(std::string colname) const
  {
    return std::find(this->null.begin(),
		     this->null.end(),
		     colname) != this->null.end();
  }

  /**
   * Add colname as NULL.
   * @param colname name of the column.
   */
  void add_null(std::string colname)
  {
    this->null.push_back(colname);
  }

  /**
   * Remove colname as NULL.
   * @param colname name of the column.
   */
  void remove_null(std::string colname)
  {
    this->null.erase(std::find(this->null.begin(),
			       this->null.end(),
			       colname));
  }


  /**
   * Cols with NULL value.
   * This can be used directly of via *null() functions.
   */
  std::vector<std::string> null;

  /**
   * If the table has changed.
   */
  bool changed;

protected:
  DB* db_;				/**< DB connection */
  ID id_;				/**< ID of the table */
};

}
}

#endif // __DB_TABLE_HPP__
