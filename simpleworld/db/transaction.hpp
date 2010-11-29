/**
 * @file simpleworld/db/transaction.hpp
 * Transactions management.
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

#ifndef SIMPLEWORLD_DB_TRANSACTION_HPP
#define SIMPLEWORLD_DB_TRANSACTION_HPP

#include <simpleworld/ints.hpp>
#include <simpleworld/db/db.hpp>

namespace simpleworld
{
namespace db
{

/**
 * Transactions management.
 */
class Transaction
{
public:
  /**
   * Types of transactions.
   * deferred:  write locks are adquired when the first insert/update/drop
   *            command is executed.
   * immediate: write locks are adquired when the transaction starts.
   * exclusive: write/read locks are adquired when the transaction starts.
   */
  enum Type {deferred, immediate, exclusive};


  /**
   * Constructor.
   * @param db DB connection
   */
  Transaction(DB* db);

  /**
   * Constructor to start the transaction.
   * @param db DB connection
   * @param type type of the transaction.
   */
  Transaction(DB* db, Type type);

  /**
   * Destructor.
   * If the transaction is started it's rollback'ed.
   */
  ~Transaction();


  /**
   * Start the transaction.
   * @param type type of the transaction.
   * @exception DBException if the transaction is already started.
   */
  void begin(Type type = deferred);

  /**
   * Create a savepoint.
   * @param savepoint name of the savepoint.
   * @exception DBException if the the transaction isn't started.
   */
  void savepoint(std::string savepoint);

  /**
   * Create a savepoint.
   * A automatic name is created in the form of SP_x, with x being the value
   * returned.
   * @return the value of the savepoint.
   * @exception DBException if the the transaction isn't started.
   */
  Uint16 savepoint();


  /**
   * Commit the transaction.
   * @exception DBException if the the transaction isn't started.
   */
  void commit();

  /**
   * Release the savepoint.
   * @param savepoint name of the savepoint.
   * @exception DBException if the the transaction isn't started or savepoint
   * doesn't exist.
   */
  void release(std::string savepoint);

  /**
   * Release the savepoint.
   * @param savepoint value of the savepoint.
   * @exception DBException if the the transaction isn't started or savepoint
   * doesn't exist.
   */
  void release(Uint16 savepoint);


  /**
   * Rollback the transaction.
   * @exception DBException if the the transaction isn't started.
   */
  void rollback();

  /**
   * Rollback the savepoint.
   * @param savepoint name of the savepoint.
   * @exception DBException if the the transaction isn't started or savepoint
   * doesn't exist.
   */
  void rollback(std::string savepoint);

  /**
   * Rollback the savepoint.
   * @param savepoint value of the savepoint.
   * @exception DBException if the the transaction isn't started or savepoint
   * doesn't exist.
   */
  void rollback(Uint16 savepoint);

private:
  DB* db_;                      /**< DB connection */
  Uint16 counter_;              /**< Values of the automatic savepoints */
  bool started_;                /**< If a transaction is started */
};

}
}

#endif // SIMPLEWORLD_DB_TRANSACTION_HPP
