/**
 * @file tests/db/db_test.cpp
 * Unit test for db::World.
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

#define BOOST_TEST_MODULE Unit test for db::World
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/concept_check.hpp>

#include <simpleworld/ints.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/exception.hpp>
#include <simpleworld/db/db.hpp>
#include <simpleworld/db/transaction.hpp>
#include <simpleworld/db/world.hpp>
namespace sw = simpleworld;
namespace db = simpleworld::db;

#include "opendb.hpp"


#define DB_FILE (TESTDATA "db.sw")
#define DB_SAVE (TESTOUTPUT "world.sw")


/**
 * Get the data.
 */
BOOST_AUTO_TEST_CASE(world_get)
{
  db::DB sw(DB_FILE);
  db::World world1(&sw, 1);
  db::World world2(&sw, 2);
  db::World world3(&sw, 3);

  BOOST_CHECK_EQUAL(world1.id(), 1);
  BOOST_CHECK_EQUAL(world1.position_x(), 2);
  BOOST_CHECK_EQUAL(world1.position_y(), 3);
  BOOST_CHECK_EQUAL(world1.orientation(), sw::OrientationNorth);
  BOOST_CHECK_EQUAL(world2.id(), 2);
  BOOST_CHECK_EQUAL(world2.position_x(), 11);
  BOOST_CHECK_EQUAL(world2.position_y(), 11);
  BOOST_CHECK_EQUAL(world2.orientation(), sw::OrientationSouth);
  BOOST_CHECK_EQUAL(world3.id(), 3);
  BOOST_CHECK_EQUAL(world3.position_x(), 1);
  BOOST_CHECK_EQUAL(world3.position_y(), 7);
  BOOST_CHECK_EQUAL(world3.is_null("orientation"), true);
}

db::ID id;

/**
 * Insert the data.
 */
BOOST_AUTO_TEST_CASE(world_insert)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);
  id = db::World::insert(&sw, 5, 3, sw::OrientationSouth);
  db::World world(&sw, id);

  BOOST_CHECK_EQUAL(world.id(), id);
  BOOST_CHECK_EQUAL(world.position_x(), 5);
  BOOST_CHECK_EQUAL(world.position_y(), 3);
  BOOST_CHECK_EQUAL(world.orientation(), sw::OrientationSouth);

  transaction.commit();
}


/**
 * Update the data.
 */
BOOST_AUTO_TEST_CASE(world_update)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);
  db::World world(&sw, id);
  world.position_x(8);
  world.position_y(0);
  world.set_null("orientation");

  BOOST_CHECK_EQUAL(world.id(), id);
  BOOST_CHECK_EQUAL(world.position_x(), 8);
  BOOST_CHECK_EQUAL(world.position_y(), 0);
  BOOST_CHECK_EQUAL(world.is_null("orientation"), true);

  transaction.commit();
}

/**
 * Delete the data.
 */
BOOST_AUTO_TEST_CASE(world_delete)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);

  BOOST_CHECK_NO_THROW(db::World::remove(&sw, id));
  BOOST_CHECK_THROW(db::World(&sw, id).position_x(), db::DBException);

  transaction.commit();
}
