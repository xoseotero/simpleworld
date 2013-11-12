/**
 * @file tests/db/db_test.cpp
 * Unit test for db::AliveBug.
 *
 *  Copyright (C) 2010-2011  Xosé Otero <xoseotero@gmail.com>
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
#include <simpleworld/cpu/isa.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/exception.hpp>
#include <simpleworld/db/db.hpp>
#include <simpleworld/db/world.hpp>
#include <simpleworld/db/code.hpp>
#include <simpleworld/db/bug.hpp>
#include <simpleworld/db/egg.hpp>
#include <simpleworld/db/registers.hpp>
#include <simpleworld/db/alivebug.hpp>
namespace sw = simpleworld;
namespace db = simpleworld::db;
namespace cpu = simpleworld::cpu;

#include "opendb.hpp"

#define DB_FILE (TESTDATA "db.sw")
#define DB_SAVE (TESTOUTPUT "alivebug.sw")

#define REGISTERS_SIZE TOTAL_REGISTERS * sizeof(cpu::Word)


/**
 * Get the data.
 */
BOOST_AUTO_TEST_CASE(alivebug_get)
{
  db::DB sw(DB_FILE);
  db::AliveBug alivebug(&sw, 1);

  BOOST_CHECK_EQUAL(alivebug.bug_id(), 1);
  BOOST_CHECK_EQUAL(alivebug.world_id(), 2);
  BOOST_CHECK_EQUAL(alivebug.birth(), 75);
  BOOST_CHECK_EQUAL(alivebug.energy(), 126);
  BOOST_CHECK_EQUAL(alivebug.time_last_action(), 189);
  BOOST_CHECK_EQUAL(alivebug.is_null("action_time"), true);
  BOOST_CHECK_EQUAL(alivebug.registers_id(), 1);
  BOOST_CHECK_EQUAL(alivebug.memory_id(), 3);
}

db::ID id1;
db::ID bug_id1;
db::ID world_id1;

/**
 * Insert the data.
 */
BOOST_AUTO_TEST_CASE(alivebug_insert)
{
  db::DB sw = open_db(DB_SAVE);
  world_id1 = db::World::insert(&sw, 1, 12);
  db::ID code_id = db::Code::insert(&sw, "code", 4);
  bug_id1 = db::Bug::insert(&sw, code_id, 0);
  db::ID registers_id = db::Registers::insert(&sw);
  id1 = db::AliveBug::insert(&sw, bug_id1, world_id1, 15, 215,
			    registers_id, code_id);
  db::AliveBug alivebug(&sw, id1);

  BOOST_CHECK_EQUAL(alivebug.bug_id(), id1);
  BOOST_CHECK_EQUAL(alivebug.world_id(), world_id1);
  BOOST_CHECK_EQUAL(alivebug.birth(), 15);
  BOOST_CHECK_EQUAL(alivebug.energy(), 215);
  BOOST_CHECK_EQUAL(alivebug.is_null("time_last_action"), true);
  BOOST_CHECK_EQUAL(alivebug.is_null("action_time"), true);
  BOOST_CHECK_EQUAL(alivebug.registers_id(), registers_id);
  BOOST_CHECK_EQUAL(alivebug.memory_id(), code_id);
}

db::ID id2;
db::ID bug_id2;
db::ID world_id2;

/**
 * Insert the data from a egg.
 */
BOOST_AUTO_TEST_CASE(alivebug_insert_egg)
{
  db::DB sw = open_db(DB_SAVE);
  world_id2 = db::World::insert(&sw, 13, 2);
  db::ID code_id = db::Code::insert(&sw, "code", 4);
  bug_id2 = db::Bug::insert(&sw, code_id, 0);
  db::ID egg_id = db::Egg::insert(&sw, bug_id2, world_id2, 74,
				  db::Code::insert(&sw, code_id));
  db::Egg egg(&sw, egg_id);
  id2 = db::AliveBug::insert(&sw, &egg, 25);
  db::AliveBug alivebug(&sw, id2);

  BOOST_CHECK_THROW(db::Egg(&sw, egg_id).energy(), db::DBException);
  BOOST_CHECK_EQUAL(alivebug.bug_id(), id2);
  BOOST_CHECK_EQUAL(alivebug.world_id(), world_id2);
  BOOST_CHECK_EQUAL(alivebug.birth(), 25);
  BOOST_CHECK_EQUAL(alivebug.energy(), 74);
  BOOST_CHECK_EQUAL(alivebug.is_null("time_last_action"), true);
  BOOST_CHECK_EQUAL(alivebug.is_null("action_time"), true);
}

/**
 * Update the data.
 */
BOOST_AUTO_TEST_CASE(alivebug_update)
{
  db::DB sw = open_db(DB_SAVE);
  db::AliveBug alivebug(&sw, id1);
  alivebug.birth(21);
  alivebug.energy(52);
  alivebug.time_last_action(0);
  alivebug.action_time(45);
  db::ID registers_id = db::Registers::insert(&sw);
  alivebug.registers_id(registers_id);
  db::ID memory_id = db::Code::insert(&sw, "code", 4);
  alivebug.memory_id(memory_id);

  BOOST_CHECK_EQUAL(alivebug.id(), id1);
  BOOST_CHECK_EQUAL(alivebug.world_id(), world_id1);
  BOOST_CHECK_EQUAL(alivebug.birth(), 21);
  BOOST_CHECK_EQUAL(alivebug.energy(), 52);
  BOOST_CHECK_EQUAL(alivebug.is_null("time_last_action"), false);
  BOOST_CHECK_EQUAL(alivebug.time_last_action(), 0);
  BOOST_CHECK_EQUAL(alivebug.is_null("action_time"), false);
  BOOST_CHECK_EQUAL(alivebug.action_time(), 45);
  BOOST_CHECK_EQUAL(alivebug.registers_id(), registers_id);
  BOOST_CHECK_EQUAL(alivebug.memory_id(), memory_id);
}

/**
 * Delete the data.
 */
BOOST_AUTO_TEST_CASE(alivebug_delete)
{
  db::DB sw = open_db(DB_SAVE);
  BOOST_CHECK_NO_THROW(db::AliveBug::remove(&sw, id1));
  db::World::remove(&sw, world_id1);
  db::Bug::remove(&sw, id1);

  BOOST_CHECK_NO_THROW(db::AliveBug::remove(&sw, id2));
  db::World::remove(&sw, world_id2);
  db::Bug::remove(&sw, id2);

  BOOST_CHECK_THROW(db::AliveBug(&sw, id1).birth(), db::DBException);
  BOOST_CHECK_THROW(db::AliveBug(&sw, id2).energy(), db::DBException);
}
