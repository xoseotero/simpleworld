/**
 * @file tests/db/spawn_test.cpp
 * Unit test for db::Spawn.
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

#define BOOST_TEST_MODULE Unit test for db::Spawn
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/ints.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/exception.hpp>
#include <simpleworld/db/db.hpp>
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
  sw::Uint32 size1;
  boost::shared_array<sw::Uint8> code1 = spawn1.code().read(&size1);
  db::Spawn spawn2(&sw, 2);
  sw::Uint32 size2;
  boost::shared_array<sw::Uint8> code2 = spawn2.code().read(&size2);

  BOOST_CHECK_EQUAL(spawn1.id(), 1);
  BOOST_CHECK_EQUAL(spawn1.frequency(), 1024);
  BOOST_CHECK_EQUAL(spawn1.max(), 12);
  BOOST_CHECK_EQUAL(spawn1.start_x(), 1);
  BOOST_CHECK_EQUAL(spawn1.start_y(), 1);
  BOOST_CHECK_EQUAL(spawn1.end_x(), 5);
  BOOST_CHECK_EQUAL(spawn1.end_y(), 6);
  BOOST_CHECK_EQUAL(spawn1.energy(), 512);
  BOOST_CHECK_EQUAL(size1, 8);
  BOOST_CHECK_EQUAL(code1[0], 0xAB);
  BOOST_CHECK_EQUAL(code1[1], 0xAB);
  BOOST_CHECK_EQUAL(code1[2], 0xAB);
  BOOST_CHECK_EQUAL(code1[3], 0xAB);
  BOOST_CHECK_EQUAL(code1[4], 0xAB);
  BOOST_CHECK_EQUAL(code1[5], 0xAB);
  BOOST_CHECK_EQUAL(code1[6], 0xAB);
  BOOST_CHECK_EQUAL(code1[7], 0xAB);
  BOOST_CHECK_EQUAL(spawn2.id(), 2);
  BOOST_CHECK_EQUAL(spawn2.frequency(), 4096);
  BOOST_CHECK_EQUAL(spawn2.max(), 16);
  BOOST_CHECK_EQUAL(spawn2.start_x(), 4);
  BOOST_CHECK_EQUAL(spawn2.start_y(), 5);
  BOOST_CHECK_EQUAL(spawn2.end_x(), 8);
  BOOST_CHECK_EQUAL(spawn2.end_y(), 9);
  BOOST_CHECK_EQUAL(spawn2.energy(), 1024);
  BOOST_CHECK_EQUAL(size2, 8);
  BOOST_CHECK_EQUAL(code2[0], 0xBA);
  BOOST_CHECK_EQUAL(code2[1], 0xBA);
  BOOST_CHECK_EQUAL(code2[2], 0xBA);
  BOOST_CHECK_EQUAL(code2[3], 0xBA);
  BOOST_CHECK_EQUAL(code2[4], 0xBA);
  BOOST_CHECK_EQUAL(code2[5], 0xBA);
  BOOST_CHECK_EQUAL(code2[6], 0xBA);
  BOOST_CHECK_EQUAL(code2[7], 0xBA);
}

db::ID id;

/**
 * Insert the data.
 */
BOOST_AUTO_TEST_CASE(spawn_insert)
{
  db::DB sw = open_db(DB_SAVE);
  id = db::Spawn::insert(&sw, 1024, 8, 0, 0, 4, 4, 512, "1234", 4);
  db::Spawn spawn(&sw, id);
  sw::Uint32 size;
  boost::shared_array<sw::Uint8> code = spawn.code().read(&size);

  BOOST_CHECK_EQUAL(spawn.id(), id);
  BOOST_CHECK_EQUAL(spawn.frequency(), 1024);
  BOOST_CHECK_EQUAL(spawn.max(), 8);
  BOOST_CHECK_EQUAL(spawn.start_x(), 0);
  BOOST_CHECK_EQUAL(spawn.start_y(), 0);
  BOOST_CHECK_EQUAL(spawn.end_x(), 4);
  BOOST_CHECK_EQUAL(spawn.end_y(), 4);
  BOOST_CHECK_EQUAL(spawn.energy(), 512);
  BOOST_CHECK_EQUAL(size, 4);
  BOOST_CHECK_EQUAL(code[0], '1');
  BOOST_CHECK_EQUAL(code[1], '2');
  BOOST_CHECK_EQUAL(code[2], '3');
  BOOST_CHECK_EQUAL(code[3], '4');
}


/**
 * Update the data.
 */
BOOST_AUTO_TEST_CASE(spawn_update)
{
  db::DB sw = open_db(DB_SAVE);
  db::Spawn spawn(&sw, id);
  spawn.frequency(2048);
  spawn.max(16);
  spawn.end_x(6);
  spawn.end_y(7);
  spawn.start_x(1);
  spawn.start_y(2);
  spawn.energy(1024);
  spawn.code().write("0101", 4);
  sw::Uint32 size;
  boost::shared_array<sw::Uint8> code = spawn.code().read(&size);

  BOOST_CHECK_EQUAL(spawn.id(), id);
  BOOST_CHECK_EQUAL(spawn.frequency(), 2048);
  BOOST_CHECK_EQUAL(spawn.max(), 16);
  BOOST_CHECK_EQUAL(spawn.start_x(), 1);
  BOOST_CHECK_EQUAL(spawn.start_y(), 2);
  BOOST_CHECK_EQUAL(spawn.end_x(), 6);
  BOOST_CHECK_EQUAL(spawn.end_y(), 7);
  BOOST_CHECK_EQUAL(spawn.energy(), 1024);
  BOOST_CHECK_EQUAL(size, 4);
  BOOST_CHECK_EQUAL(code[0], '0');
  BOOST_CHECK_EQUAL(code[1], '1');
  BOOST_CHECK_EQUAL(code[2], '0');
  BOOST_CHECK_EQUAL(code[3], '1');
}

/**
 * Delete the data.
 */
BOOST_AUTO_TEST_CASE(spawn_delete)
{
  db::DB sw = open_db(DB_SAVE);
  db::Spawn::remove(&sw, id);

  BOOST_CHECK_THROW(db::Spawn(&sw, id).frequency(), db::DBException);
}
