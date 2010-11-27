/**
 * @file simpleworld/db/transaction.cpp
 * Simple World database.
 *
 *  Copyright (C) 2010  Xosé Otero <xoseotero@gmail.com>
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

#include <boost/format.hpp>

#include <sqlite3.h>

#include "exception.hpp"
#include "transaction.hpp"

namespace simpleworld
{
namespace db
{

/**
 * Constructor.
 * @param db DB connection
 */
Transaction::Transaction(DB* db)
  : db_(db), counter_(0), started_(false)
{
}

/**
 * Constructor to start the transaction.
 * @param db DB connection
 * @param type type of the transaction.
 */
Transaction::Transaction(DB* db, Type type)
  : db_(db), counter_(0), started_(false)
{
  this->begin(type);
}

/**
 * Destructor.
 * If the transaction is started it's rollback'ed.
 */
Transaction::~Transaction()
{
  if (this->started_)
    this->rollback();
}


/**
 * Start the transaction.
 * @param type type of the transaction.
 * @exception DBException if the transaction is already started.
 */
void Transaction::begin(Type type)
{
  char* errmsg;
  switch (type) {
  case deferred:
    sqlite3_exec(this->db_->db(), "BEGIN DEFERRED;", NULL, NULL, &errmsg);
    break;
  case immediate:
    sqlite3_exec(this->db_->db(), "BEGIN IMMEDIATE;", NULL, NULL, &errmsg);
    break;
  case exclusive:
    sqlite3_exec(this->db_->db(), "BEGIN EXCLUSIVE;", NULL, NULL, &errmsg);
    break;
  default:
    throw EXCEPTION(DBException,
		    boost::str(boost::format("Unknown transaction type: %1%")
			       % static_cast<int>(type)));
  }

  if (errmsg) {
    std::string error(errmsg);
    sqlite3_free(errmsg);
    throw EXCEPTION(DBException, error);
  }

  this->started_ = true;
}

/**
 * Create a savepoint.
 * @param savepoint name of the savepoint.
 * @exception DBException if the the transaction isn't started.
 */
void Transaction::savepoint(std::string savepoint)
{
  char* errmsg;
  if (sqlite3_exec(this->db_->db(), boost::str(boost::format("SAVEPOINT %1%;")
					% savepoint).c_str(),
		   NULL, NULL, &errmsg)) {
    std::string error(errmsg);
    sqlite3_free(errmsg);
    throw EXCEPTION(DBException, error);
  }
}

/**
 * Create a savepoint.
 * A automatic name is created in the form of SP_x, with x being the value
 * returned.
 * @return the value of the savepoint.
 * @exception DBException if the the transaction isn't started.
 */
Uint16 Transaction::savepoint()
{
  this->counter_++;
  this->savepoint(boost::str(boost::format("SP_%1%") % this->counter_));

  return this->counter_;
}


/**
 * Commit the transaction.
 * @exception DBException if the the transaction isn't started.
 */
void Transaction::commit()
{
  char* errmsg;
  if (sqlite3_exec(this->db_->db(), "COMMIT;", NULL, NULL, &errmsg)) {
    std::string error(errmsg);
    sqlite3_free(errmsg);
    throw EXCEPTION(DBException, error);
  }

  this->started_ = false;
}

/**
 * Release the savepoint.
 * @param savepoint name of the savepoint.
 * @exception DBException if the the transaction isn't started or savepoint
 * doesn't exist.
 */
void Transaction::release(std::string savepoint)
{
  char* errmsg;
  if (sqlite3_exec(this->db_->db(), boost::str(boost::format("RELEASE %1%;")
					% savepoint).c_str(),
		   NULL, NULL, &errmsg)) {
    std::string error(errmsg);
    sqlite3_free(errmsg);
    throw EXCEPTION(DBException, error);
  }
}

/**
 * Release the savepoint.
 * @param savepoint value of the savepoint.
 * @exception DBException if the the transaction isn't started or savepoint
 * doesn't exist.
 */
void Transaction::release(Uint16 savepoint)
{
  this->release(boost::str(boost::format("SP_%1%") % savepoint));
}


/**
 * Rollback the transaction.
 * @exception DBException if the the transaction isn't started.
 */
void Transaction::rollback()
{
  char* errmsg;
  if (sqlite3_exec(this->db_->db(), "ROLLBACK;", NULL, NULL, &errmsg)) {
    std::string error(errmsg);
    sqlite3_free(errmsg);
    throw EXCEPTION(DBException, error);
  }

  this->started_ = false;
}

/**
 * Rollback the savepoint.
 * @param savepoint name of the savepoint.
 * @exception DBException if the the transaction isn't started or savepoint
 * doesn't exist.
 */
void Transaction::rollback(std::string savepoint)
{
  char* errmsg;
  if (sqlite3_exec(this->db_->db(),
		   boost::str(boost::format("ROLLBACK TO %1%;")
			      % savepoint).c_str(),
		   NULL, NULL, &errmsg)) {
    std::string error(errmsg);
    sqlite3_free(errmsg);
    throw EXCEPTION(DBException, error);
  }
}

/**
 * Rollback the savepoint.
 * @param savepoint value of the savepoint.
 * @exception DBException if the the transaction isn't started or savepoint
 * doesn't exist.
 */
void Transaction::rollback(Uint16 savepoint)
{
  this->rollback(boost::str(boost::format("SP_%1%") % savepoint));
}

}
}
