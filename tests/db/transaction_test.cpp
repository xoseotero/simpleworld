/**
 * @file tests/db/db_test.cpp
 * Unit test for db::Transaction.
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

#define BOOST_TEST_MODULE Unit test for db::Transaction
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/ints.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/exception.hpp>
#include <simpleworld/db/db.hpp>
#include <simpleworld/db/world.hpp>
#include <simpleworld/db/transaction.hpp>
namespace sw = simpleworld;
namespace db = simpleworld::db;

#include "opendb.hpp"


#define DB_SAVE (TESTOUTPUT "transaction.sw")


/**
 * Check rollback.
 */
BOOST_AUTO_TEST_CASE(db_rollback)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);
  db::ID id = db::World::insert(&sw, 1, 2);
  transaction.rollback();

  BOOST_CHECK_THROW(db::World(&sw, id).position_x(), db::DBException);
}

/**
 * Check rollback in destructor.
 */
BOOST_AUTO_TEST_CASE(db_rollback_block)
{
  db::DB sw = open_db(DB_SAVE);
  db::ID id;
  {
    db::Transaction transaction(&sw, db::Transaction::deferred);
    id = db::World::insert(&sw, 1, 2);
  }

  BOOST_CHECK_THROW(db::World(&sw, id).position_x(), db::DBException);
}

/**
 * Check commit.
 */
BOOST_AUTO_TEST_CASE(db_commit)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);
  db::ID id = db::World::insert(&sw, 1, 2);
  transaction.commit();

  BOOST_CHECK_NO_THROW(db::World(&sw, id).position_x());
  db::World::remove(&sw, id);
}

/**
 * Check rollback with savepoints.
 */
BOOST_AUTO_TEST_CASE(db_savepoint_rollback)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);
  sw::Uint16 savepoint = transaction.savepoint();
  db::ID id = db::World::insert(&sw, 1, 2);
  transaction.rollback(savepoint);
  transaction.commit();

  BOOST_CHECK_THROW(db::World(&sw, id).position_x(), db::DBException);
}

/**
 * Check release with savepoints.
 */
BOOST_AUTO_TEST_CASE(db_savepoint_release)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);
  sw::Uint16 savepoint = transaction.savepoint();
  db::ID id = db::World::insert(&sw, 1, 2);
  transaction.release(savepoint);
  transaction.commit();

  BOOST_CHECK_NO_THROW(db::World(&sw, id).position_x());
  db::World::remove(&sw, id);
}

/**
 * Check release with savepoints but without commit.
 */
BOOST_AUTO_TEST_CASE(db_savepoint_release_rollback)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);
  sw::Uint16 savepoint = transaction.savepoint();
  db::ID id = db::World::insert(&sw, 1, 2);
  transaction.release(savepoint);
  transaction.rollback();

  BOOST_CHECK_THROW(db::World(&sw, id).position_x(), db::DBException);
}
