/**
 * @file tests/db/egg_test.cpp
 * Unit test for db::Egg.
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

#define BOOST_TEST_MODULE Unit test for db::Egg
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/ints.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/exception.hpp>
#include <simpleworld/db/db.hpp>
#include <simpleworld/db/transaction.hpp>
#include <simpleworld/db/world.hpp>
#include <simpleworld/db/code.hpp>
#include <simpleworld/db/bug.hpp>
#include <simpleworld/db/egg.hpp>
namespace sw = simpleworld;
namespace db = simpleworld::db;

#include "opendb.hpp"


#define DB_FILE (TESTDATA "db.sw")
#define DB_SAVE (TESTOUTPUT "egg.sw")


/**
 * Get the data.
 */
BOOST_AUTO_TEST_CASE(egg_get)
{
  db::DB sw(DB_FILE);
  db::Egg egg(&sw, 3);

  BOOST_CHECK_EQUAL(egg.bug_id(), 3);
  BOOST_CHECK_EQUAL(egg.world_id(), 1);
  BOOST_CHECK_EQUAL(egg.energy(), 100);
  BOOST_CHECK_EQUAL(egg.memory_id(), 4);
}

db::ID id;
db::ID world_id;

/**
 * Insert the data.
 */
BOOST_AUTO_TEST_CASE(egg_insert)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);
  world_id = db::World::insert(&sw, 4, 3, sw::OrientationNorth);
  db::ID code_id = db::Code::insert(&sw, "code", 4);
  db::ID bug_id = db::Bug::insert(&sw, code_id, 0);
  code_id = db::Code::insert(&sw, code_id);
  id = db::Egg::insert(&sw, bug_id, world_id, 101, code_id);
  db::Egg egg(&sw, id);

  BOOST_CHECK_EQUAL(egg.bug_id(), bug_id);
  BOOST_CHECK_EQUAL(egg.bug_id(), id);
  BOOST_CHECK_EQUAL(egg.world_id(), world_id);
  BOOST_CHECK_EQUAL(egg.energy(), 101);
  BOOST_CHECK_EQUAL(egg.memory_id(), code_id);

  transaction.commit();
}


/**
 * Update the data.
 */
BOOST_AUTO_TEST_CASE(egg_update)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);
  db::Egg egg(&sw, id);
  egg.energy(82);
  db::ID code_id = db::Code::insert(&sw, egg.memory_id());
  egg.memory_id(code_id);

  BOOST_CHECK_EQUAL(egg.bug_id(), id);
  BOOST_CHECK_EQUAL(egg.energy(), 82);
  BOOST_CHECK_EQUAL(egg.memory_id(), code_id);

  transaction.commit();
}

/**
 * Delete the data.
 */
BOOST_AUTO_TEST_CASE(egg_delete)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);

  BOOST_CHECK_NO_THROW(db::Egg::remove(&sw, id));
  BOOST_CHECK_THROW(db::Egg(&sw, id).energy(), db::DBException);

  db::World::remove(&sw, world_id);

  transaction.commit();
}
