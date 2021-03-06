/**
 * @file tests/db/db_test.cpp
 * Unit test for db::Food.
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

#define BOOST_TEST_MODULE Unit test for db::Food
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/ints.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/exception.hpp>
#include <simpleworld/db/db.hpp>
#include <simpleworld/db/transaction.hpp>
#include <simpleworld/db/world.hpp>
#include <simpleworld/db/food.hpp>
namespace sw = simpleworld;
namespace db = simpleworld::db;

#include "opendb.hpp"


#define DB_FILE (TESTDATA "db.sw")
#define DB_SAVE (TESTOUTPUT "food.sw")


/**
 * Get the data.
 */
BOOST_AUTO_TEST_CASE(food_get)
{
  db::DB sw(DB_FILE);
  db::Food food(&sw, 1);

  BOOST_CHECK_EQUAL(food.id(), 1);
  BOOST_CHECK_EQUAL(food.time(), 2);
  BOOST_CHECK_EQUAL(food.world_id(), 3);
  BOOST_CHECK_EQUAL(food.size(), 10);
}

db::ID id;
db::ID world_id;

/**
 * Insert the data.
 */
BOOST_AUTO_TEST_CASE(food_insert)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);
  world_id = db::World::insert(&sw, 10, 8);
  id = db::Food::insert(&sw, 203, world_id, 101);
  db::Food food(&sw, id);

  BOOST_CHECK_EQUAL(food.id(), id);
  BOOST_CHECK_EQUAL(food.time(), 203);
  BOOST_CHECK_EQUAL(food.world_id(), world_id);
  BOOST_CHECK_EQUAL(food.size(), 101);

  transaction.commit();
}


/**
 * Update the data.
 */
BOOST_AUTO_TEST_CASE(food_update)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);
  db::Food food(&sw, id);
  food.time(87);
  food.size(78);

  BOOST_CHECK_EQUAL(food.id(), id);
  BOOST_CHECK_EQUAL(food.time(), 87);
  BOOST_CHECK_EQUAL(food.world_id(), world_id);
  BOOST_CHECK_EQUAL(food.size(), 78);

  transaction.commit();
}

/**
 * Delete the data.
 */
BOOST_AUTO_TEST_CASE(food_delete)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);
  db::Food::remove(&sw, id);
  db::World::remove(&sw, world_id);

  BOOST_CHECK_THROW(db::Food(&sw, id).size(), db::DBException);

  transaction.commit();
}
