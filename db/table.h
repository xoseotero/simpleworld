/**
 * @file db/table.h
 * Base clase for the tables.
 *
 * begin:     Tue, 13 Feb 2007 11:11:22 +0100
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2007, Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __DB_TABLE_H__
#define __DB_TABLE_H__

#include <vector>
#include <string>
#include <algorithm>

#include <boost/noncopyable.hpp>

#include <db/types.h>
#include <db/exception.h>
#include <db/db.h>

namespace SimpleWorld
{
namespace DB
{

/**
 * Base clase for the tables.
 */
class Table: public boost::noncopyable
{
public:
  /**
   * Constructor.
   * @param db database.
   * @param id id of the row.
   * @exception DBError if there is a error in the database.
   * @exception IDNotFound if the ID is not found in the table.
   */
  Table(DB* db, ID id) throw (DBError, IDNotFound);

  /**
   * Destructor.
  */
  virtual ~Table() throw () = 0;


  /**
   * The database of the table.
   * @return the database object.
   */
  DB* db() const throw () { return this->db_; }

  /**
   * The id of the table.
   * @return the ID.
   */
  ID id() const throw () { return this->id_; }


  /**
   * Cols with NULL value.
   * This can be used directly of via *null() functions.
   */
  std::vector<std::string> null;

  /**
   * Check if colname is NULL.
   * @param colname name of the column.
   * @return true if colname is NULL, else false.
   */
  bool is_null(std::string colname) const throw ()
  {
    return std::find(this->null.begin(),
		     this->null.end(),
		     colname) != this->null.end();
  }

  /**
   * Add colname as NULL.
   * @param colname name of the column.
   */
  void add_null(std::string colname) throw ()
  {
    this->null.push_back(colname);
  }

  /**
   * Remove colname as NULL.
   * @param colname name of the column.
   */
  void remove_null(std::string colname) throw ()
  {
    this->null.erase(std::find(this->null.begin(),
			       this->null.end(),
			       colname));
  }


  /**
   * Update the data of the class with the database.
   * @exception DBError if there is a error in the database.
   * @execption IDNotFound if the ID is not found in the table.
   */
  virtual void update() throw (DBError, IDNotFound) = 0;

  /**
   * Update the database with the data of the class.
   * @exception DBError if there is a error in the database.
   */
  virtual void update_db() throw (DBError) = 0;

protected:
  DB* db_;
  ID id_;
};

}
}

#endif // __DB_TABLE_H__
