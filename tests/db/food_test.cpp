/**
 * @file tests/db/db_test.cpp
 * Unit test for db::Food.
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

#define BOOST_TEST_MODULE Unit test for db::Food
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/ints.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/exception.hpp>
#include <simpleworld/db/db.hpp>
#include <simpleworld/db/world.hpp>
#include <simpleworld/db/food.hpp>
namespace sw = simpleworld;
namespace db = simpleworld::db;


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
  db::DB sw(DB_SAVE);
  world_id = db::World::insert(&sw, 10, 8);
  id = db::Food::insert(&sw, world_id, 101);
  db::Food food(&sw, id);

  BOOST_CHECK_EQUAL(food.id(), id);
  BOOST_CHECK_EQUAL(food.world_id(), world_id);
  BOOST_CHECK_EQUAL(food.size(), 101);
}


/**
 * Update the data.
 */
BOOST_AUTO_TEST_CASE(food_update)
{
  db::DB sw(DB_SAVE);
  db::Food food(&sw, id);
  food.size(78);

  BOOST_CHECK_EQUAL(food.id(), id);
  BOOST_CHECK_EQUAL(food.world_id(), world_id);
  BOOST_CHECK_EQUAL(food.size(), 78);
}

/**
 * Delete the data.
 */
BOOST_AUTO_TEST_CASE(food_delete)
{
  db::DB sw(DB_SAVE);
  db::Food::remove(&sw, id);
  db::World::remove(&sw, world_id);

  BOOST_CHECK_THROW(db::Food(&sw, id).size(), db::DBException);
}