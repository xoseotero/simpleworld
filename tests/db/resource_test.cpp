/**
 * @file tests/db/resource_test.cpp
 * Unit test for db::Resource.
 *
 *  Copyright (C) 2010-2013  Xosé Otero <xoseotero@gmail.com>
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

#define BOOST_TEST_MODULE Unit test for db::Resource
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/ints.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/exception.hpp>
#include <simpleworld/db/db.hpp>
#include <simpleworld/db/transaction.hpp>
#include <simpleworld/db/resource.hpp>
namespace sw = simpleworld;
namespace db = simpleworld::db;

#include "opendb.hpp"


#define DB_FILE (TESTDATA "db.sw")
#define DB_SAVE (TESTOUTPUT "resource.sw")


/**
 * Get the data.
 */
BOOST_AUTO_TEST_CASE(resource_get)
{
  db::DB sw(DB_FILE);
  db::Resource resource(&sw, 1);

  BOOST_CHECK_EQUAL(resource.id(), 1);
  BOOST_CHECK_EQUAL(resource.frequency(), 512);
  BOOST_CHECK_EQUAL(resource.max(), 10);
  BOOST_CHECK_EQUAL(resource.start_x(), 0);
  BOOST_CHECK_EQUAL(resource.start_y(), 0);
  BOOST_CHECK_EQUAL(resource.end_x(), 16);
  BOOST_CHECK_EQUAL(resource.end_y(), 16);
  BOOST_CHECK_EQUAL(resource.size(), 64);
}

db::ID id;

/**
 * Insert the data.
 */
BOOST_AUTO_TEST_CASE(resource_insert)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);
  id = db::Resource::insert(&sw, 1024, 8, 0, 0, 4, 4, 128);
  db::Resource resource(&sw, id);

  BOOST_CHECK_EQUAL(resource.id(), id);
  BOOST_CHECK_EQUAL(resource.frequency(), 1024);
  BOOST_CHECK_EQUAL(resource.max(), 8);
  BOOST_CHECK_EQUAL(resource.start_x(), 0);
  BOOST_CHECK_EQUAL(resource.start_y(), 0);
  BOOST_CHECK_EQUAL(resource.end_x(), 4);
  BOOST_CHECK_EQUAL(resource.end_y(), 4);
  BOOST_CHECK_EQUAL(resource.size(), 128);

  transaction.commit();
}


/**
 * Update the data.
 */
BOOST_AUTO_TEST_CASE(resource_update)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);
  db::Resource resource(&sw, id);
  resource.frequency(2048);
  resource.max(16);
  resource.end_x(6);
  resource.end_y(7);
  resource.start_x(1);
  resource.start_y(2);
  resource.size(100);

  BOOST_CHECK_EQUAL(resource.id(), id);
  BOOST_CHECK_EQUAL(resource.frequency(), 2048);
  BOOST_CHECK_EQUAL(resource.max(), 16);
  BOOST_CHECK_EQUAL(resource.start_x(), 1);
  BOOST_CHECK_EQUAL(resource.start_y(), 2);
  BOOST_CHECK_EQUAL(resource.end_x(), 6);
  BOOST_CHECK_EQUAL(resource.end_y(), 7);
  BOOST_CHECK_EQUAL(resource.size(), 100);

  transaction.commit();
}

/**
 * Delete the data.
 */
BOOST_AUTO_TEST_CASE(resource_delete)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);
  db::Resource::remove(&sw, id);

  BOOST_CHECK_THROW(db::Resource(&sw, id).frequency(), db::DBException);

  transaction.commit();
}
