/**
 * @file tests/db/db_test.cpp
 * Unit test for db::AliveBug.
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

#define BOOST_TEST_MODULE Unit test for db::AliveBug
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/ints.hpp>
#include <simpleworld/cpu/types.hpp>
#include <simpleworld/cpu/cpu.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/exception.hpp>
#include <simpleworld/db/db.hpp>
#include <simpleworld/db/world.hpp>
#include <simpleworld/db/bug.hpp>
#include <simpleworld/db/egg.hpp>
#include <simpleworld/db/alivebug.hpp>
namespace sw = simpleworld;
namespace db = simpleworld::db;
namespace cpu = simpleworld::cpu;


#define DB_FILE (TESTDATA "db.sw")
#define DB_SAVE (TESTOUTPUT "alivebug.sw")


/**
 * Get the data.
 */
BOOST_AUTO_TEST_CASE(alivebug_get)
{
  db::DB sw(DB_FILE);
  db::AliveBug alivebug(&sw, 1);
  sw::Uint32 size;
  boost::shared_array<sw::Uint8> registers = alivebug.registers().read(&size);

  BOOST_CHECK_EQUAL(alivebug.bug_id(), 1);
  BOOST_CHECK_EQUAL(alivebug.world_id(), 2);
  BOOST_CHECK_EQUAL(alivebug.birth(), 75);
  BOOST_CHECK_EQUAL(alivebug.energy(), 126);
  BOOST_CHECK_EQUAL(alivebug.time_last_action(), 189);
  BOOST_CHECK_EQUAL(alivebug.is_null("action_time"), true);
  BOOST_CHECK_EQUAL(size, 16);
  BOOST_CHECK_EQUAL(registers[0], 0x0);
  BOOST_CHECK_EQUAL(registers[1], 0x1);
  BOOST_CHECK_EQUAL(registers[2], 0x2);
  BOOST_CHECK_EQUAL(registers[3], 0x3);
  BOOST_CHECK_EQUAL(registers[4], 0x4);
  BOOST_CHECK_EQUAL(registers[5], 0x5);
  BOOST_CHECK_EQUAL(registers[6], 0x6);
  BOOST_CHECK_EQUAL(registers[7], 0x7);
  BOOST_CHECK_EQUAL(registers[8], 0x8);
  BOOST_CHECK_EQUAL(registers[9], 0x9);
  BOOST_CHECK_EQUAL(registers[10], 0xA);
  BOOST_CHECK_EQUAL(registers[11], 0xB);
  BOOST_CHECK_EQUAL(registers[12], 0xC);
  BOOST_CHECK_EQUAL(registers[13], 0xD);
  BOOST_CHECK_EQUAL(registers[14], 0xE);
  BOOST_CHECK_EQUAL(registers[15], 0xF);
}

db::ID id1;
db::ID bug_id1;
db::ID world_id1;

/**
 * Insert the data.
 */
BOOST_AUTO_TEST_CASE(alivebug_insert)
{
  db::DB sw(DB_SAVE);
  world_id1 = db::World::insert(&sw, 1, 12);
  bug_id1 = db::Bug::insert(&sw, "code", 4);
  id1 = db::AliveBug::insert(&sw, bug_id1, world_id1, 15, 215, "register", 8);
  db::AliveBug alivebug(&sw, id1);
  sw::Uint32 size;
  boost::shared_array<sw::Uint8> registers = alivebug.registers().read(&size);

  BOOST_CHECK_EQUAL(alivebug.bug_id(), id1);
  BOOST_CHECK_EQUAL(alivebug.world_id(), world_id1);
  BOOST_CHECK_EQUAL(alivebug.birth(), 15);
  BOOST_CHECK_EQUAL(alivebug.energy(), 215);
  BOOST_CHECK_EQUAL(alivebug.is_null("time_last_action"), true);
  BOOST_CHECK_EQUAL(alivebug.is_null("action_time"), true);
  BOOST_CHECK_EQUAL(size, 8);
  BOOST_CHECK_EQUAL(registers[0], 'r');
  BOOST_CHECK_EQUAL(registers[1], 'e');
  BOOST_CHECK_EQUAL(registers[2], 'g');
  BOOST_CHECK_EQUAL(registers[3], 'i');
  BOOST_CHECK_EQUAL(registers[4], 's');
  BOOST_CHECK_EQUAL(registers[5], 't');
  BOOST_CHECK_EQUAL(registers[6], 'e');
  BOOST_CHECK_EQUAL(registers[7], 'r');
}

db::ID id2;
db::ID bug_id2;
db::ID world_id2;

/**
 * Insert the data from a egg.
 */
BOOST_AUTO_TEST_CASE(alivebug_insert_egg)
{
  db::DB sw(DB_SAVE);
  world_id2 = db::World::insert(&sw, 13, 2);
  bug_id2 = db::Bug::insert(&sw, "code", 4);
  db::ID egg_id = db::Egg::insert(&sw, bug_id2, world_id2, 74, 0);
  db::Egg egg(&sw, egg_id);
  id2 = db::AliveBug::insert(&sw, &egg, 25);
  db::AliveBug alivebug(&sw, id2);
  sw::Uint32 size;
  boost::shared_array<sw::Uint8> registers = alivebug.registers().read(&size);

  BOOST_CHECK_THROW(db::Egg(&sw, egg_id).conception(), db::DBException);
  BOOST_CHECK_EQUAL(alivebug.bug_id(), id2);
  BOOST_CHECK_EQUAL(alivebug.world_id(), world_id2);
  BOOST_CHECK_EQUAL(alivebug.birth(), 25);
  BOOST_CHECK_EQUAL(alivebug.energy(), 74);
  BOOST_CHECK_EQUAL(alivebug.is_null("time_last_action"), true);
  BOOST_CHECK_EQUAL(alivebug.is_null("action_time"), true);
  BOOST_CHECK_EQUAL(size, TOTAL_REGISTERS * sizeof(cpu::Word));
  for (int i = 0; i < TOTAL_REGISTERS * sizeof(cpu::Word); i++)
    BOOST_CHECK_EQUAL(registers[i], 0x0);
}

/**
 * Update the data.
 */
BOOST_AUTO_TEST_CASE(alivebug_update)
{
  db::DB sw(DB_SAVE);
  db::AliveBug alivebug(&sw, id1);
  alivebug.birth(21);
  alivebug.energy(52);
  alivebug.time_last_action(40);
  alivebug.action_time(45);
  alivebug.registers().write("void", 4);
  sw::Uint32 size;
  boost::shared_array<sw::Uint8> registers = alivebug.registers().read(&size);

  BOOST_CHECK_EQUAL(alivebug.id(), id1);
  BOOST_CHECK_EQUAL(alivebug.world_id(), world_id1);
  BOOST_CHECK_EQUAL(alivebug.birth(), 21);
  BOOST_CHECK_EQUAL(alivebug.energy(), 52);
  BOOST_CHECK_EQUAL(alivebug.time_last_action(), 40);
  BOOST_CHECK_EQUAL(alivebug.action_time(), 45);
  BOOST_CHECK_EQUAL(size, 4);
  BOOST_CHECK_EQUAL(registers[0], 'v');
  BOOST_CHECK_EQUAL(registers[1], 'o');
  BOOST_CHECK_EQUAL(registers[2], 'i');
  BOOST_CHECK_EQUAL(registers[3], 'd');
}

/**
 * Delete the data.
 */
BOOST_AUTO_TEST_CASE(alivebug_delete)
{
  db::DB sw(DB_SAVE);
  db::AliveBug::remove(&sw, id1);
  db::World::remove(&sw, world_id1);
  db::Bug::remove(&sw, id1);
  db::AliveBug::remove(&sw, id2);
  db::World::remove(&sw, world_id2);
  db::Bug::remove(&sw, id2);

  BOOST_CHECK_THROW(db::AliveBug(&sw, id1).birth(), db::DBException);
  BOOST_CHECK_THROW(db::AliveBug(&sw, id2).energy(), db::DBException);
}
