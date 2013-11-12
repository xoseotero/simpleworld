/**
 * @file tests/db/spawn_test.cpp
 * Unit test for db::Spawn.
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

#define BOOST_TEST_MODULE Unit test for db::Spawn
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/ints.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/exception.hpp>
#include <simpleworld/db/db.hpp>
#include <simpleworld/db/code.hpp>
#include <simpleworld/db/spawn.hpp>
namespace sw = simpleworld;
namespace db = simpleworld::db;

#include "opendb.hpp"


#define DB_FILE (TESTDATA "db.sw")
#define DB_SAVE (TESTOUTPUT "spawn.sw")


/**
 * Get the data.
 */
BOOST_AUTO_TEST_CASE(spawn_get)
{
  db::DB sw(DB_FILE);

  db::Spawn spawn1(&sw, 1);
  BOOST_CHECK_EQUAL(spawn1.id(), 1);
  BOOST_CHECK_EQUAL(spawn1.code_id(), 1);
  BOOST_CHECK_EQUAL(spawn1.frequency(), 1024);
  BOOST_CHECK_EQUAL(spawn1.max(), 12);
  BOOST_CHECK_EQUAL(spawn1.start_x(), 1);
  BOOST_CHECK_EQUAL(spawn1.start_y(), 1);
  BOOST_CHECK_EQUAL(spawn1.end_x(), 5);
  BOOST_CHECK_EQUAL(spawn1.end_y(), 6);
  BOOST_CHECK_EQUAL(spawn1.energy(), 512);

  db::Spawn spawn2(&sw, 2);  
  BOOST_CHECK_EQUAL(spawn2.id(), 2);
  BOOST_CHECK_EQUAL(spawn2.code_id(), 2);
  BOOST_CHECK_EQUAL(spawn2.frequency(), 4096);
  BOOST_CHECK_EQUAL(spawn2.max(), 16);
  BOOST_CHECK_EQUAL(spawn2.start_x(), 4);
  BOOST_CHECK_EQUAL(spawn2.start_y(), 5);
  BOOST_CHECK_EQUAL(spawn2.end_x(), 8);
  BOOST_CHECK_EQUAL(spawn2.end_y(), 9);
  BOOST_CHECK_EQUAL(spawn2.energy(), 1024);
}

db::ID id;

/**
 * Insert the data.
 */
BOOST_AUTO_TEST_CASE(spawn_insert)
{
  db::DB sw = open_db(DB_SAVE);
  db::ID code_id = db::Code::insert(&sw, "1234", 4);
  id = db::Spawn::insert(&sw, code_id, 1024, 8, 0, 0, 4, 4, 512);

  db::Spawn spawn(&sw, id);
  BOOST_CHECK_EQUAL(spawn.id(), id);
  BOOST_CHECK_EQUAL(spawn.code_id(), code_id);
  BOOST_CHECK_EQUAL(spawn.frequency(), 1024);
  BOOST_CHECK_EQUAL(spawn.max(), 8);
  BOOST_CHECK_EQUAL(spawn.start_x(), 0);
  BOOST_CHECK_EQUAL(spawn.start_y(), 0);
  BOOST_CHECK_EQUAL(spawn.end_x(), 4);
  BOOST_CHECK_EQUAL(spawn.end_y(), 4);
  BOOST_CHECK_EQUAL(spawn.energy(), 512);
}


/**
 * Update the data.
 */
BOOST_AUTO_TEST_CASE(spawn_update)
{
  db::DB sw = open_db(DB_SAVE);
  db::ID code_id = db::Code::insert(&sw, "1234", 4);
  db::Spawn spawn(&sw, id);
  spawn.code_id(code_id);
  spawn.frequency(2048);
  spawn.max(16);
  spawn.end_x(6);
  spawn.end_y(7);
  spawn.start_x(1);
  spawn.start_y(2);
  spawn.energy(1024);
  
  db::Spawn test(&sw, id);
  BOOST_CHECK_EQUAL(test.id(), id);
  BOOST_CHECK_EQUAL(test.code_id(), code_id);
  BOOST_CHECK_EQUAL(test.frequency(), 2048);
  BOOST_CHECK_EQUAL(test.max(), 16);
  BOOST_CHECK_EQUAL(test.start_x(), 1);
  BOOST_CHECK_EQUAL(test.start_y(), 2);
  BOOST_CHECK_EQUAL(test.end_x(), 6);
  BOOST_CHECK_EQUAL(test.end_y(), 7);
  BOOST_CHECK_EQUAL(test.energy(), 1024);
}

/**
 * Delete the data.
 */
BOOST_AUTO_TEST_CASE(spawn_delete)
{
  db::DB sw = open_db(DB_SAVE);

  BOOST_CHECK_NO_THROW(db::Spawn::remove(&sw, id));
  BOOST_CHECK_THROW(db::Spawn(&sw, id).frequency(), db::DBException);
}
