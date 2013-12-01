/**
 * @file tests/db/deadbug_test.cpp
 * Unit test for db::DeadBug.
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

#define BOOST_TEST_MODULE Unit test for db::DeadBug
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/ints.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/exception.hpp>
#include <simpleworld/db/db.hpp>
#include <simpleworld/db/transaction.hpp>
#include <simpleworld/db/code.hpp>
#include <simpleworld/db/bug.hpp>
#include <simpleworld/db/world.hpp>
#include <simpleworld/db/egg.hpp>
#include <simpleworld/db/registers.hpp>
#include <simpleworld/db/alivebug.hpp>
#include <simpleworld/db/deadbug.hpp>
namespace sw = simpleworld;
namespace db = simpleworld::db;

#include "opendb.hpp"


#define DB_FILE (TESTDATA "db.sw")
#define DB_SAVE (TESTOUTPUT "deadbug.sw")


/**
 * Get the data.
 */
BOOST_AUTO_TEST_CASE(deadbug_get)
{
  db::DB sw(DB_FILE);
  db::DeadBug deadbug(&sw, 2);

  BOOST_CHECK_EQUAL(deadbug.bug_id(), 2);
  BOOST_CHECK_EQUAL(deadbug.birth(), 100);
  BOOST_CHECK_EQUAL(deadbug.death(), 150);
  BOOST_CHECK_EQUAL(deadbug.is_null("killer_id"), false);
  BOOST_CHECK_EQUAL(deadbug.killer_id(), 1);
}

db::ID id;

/**
 * Insert the data.
 */
BOOST_AUTO_TEST_CASE(deadbug_insert)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);
  db::ID code_id = db::Code::insert(&sw, "code", 4);
  id = db::Bug::insert(&sw, code_id, 0);
  db::DeadBug::insert(&sw, id, 101);
  db::DeadBug deadbug1(&sw, id);

  BOOST_CHECK_EQUAL(deadbug1.bug_id(), id);
  BOOST_CHECK_EQUAL(deadbug1.death(), 101);
  BOOST_CHECK_EQUAL(deadbug1.is_null("birth"), true);
  BOOST_CHECK_EQUAL(deadbug1.is_null("killer_id"), true);

  db::ID id2 = db::Bug::insert(&sw, code_id, 0);
  db::DeadBug::insert(&sw, id2, 21, static_cast<sw::Time>(50));
  db::DeadBug deadbug2(&sw, id2);

  BOOST_CHECK_EQUAL(deadbug2.bug_id(), id2);
  BOOST_CHECK_EQUAL(deadbug2.birth(), 21);
  BOOST_CHECK_EQUAL(deadbug2.is_null("birth"), false);
  BOOST_CHECK_EQUAL(deadbug2.death(), 50);
  BOOST_CHECK_EQUAL(deadbug2.is_null("killer_id"), true);

  db::ID id3 = db::Bug::insert(&sw, code_id, 0);
  db::DeadBug::insert(&sw, id3, 51, id);
  db::DeadBug deadbug3(&sw, id3);

  BOOST_CHECK_EQUAL(deadbug3.bug_id(), id3);
  BOOST_CHECK_EQUAL(deadbug3.death(), 51);
  BOOST_CHECK_EQUAL(deadbug3.is_null("birth"), true);
  BOOST_CHECK_EQUAL(deadbug3.is_null("killer_id"), false);
  BOOST_CHECK_EQUAL(deadbug3.killer_id(), id);

  db::ID id4 = db::Bug::insert(&sw, code_id, 0);
  db::DeadBug::insert(&sw, id4, 0, 100, id);
  db::DeadBug deadbug4(&sw, id4);

  BOOST_CHECK_EQUAL(deadbug4.bug_id(), id4);
  BOOST_CHECK_EQUAL(deadbug4.death(), 100);
  BOOST_CHECK_EQUAL(deadbug4.is_null("birth"), false);
  BOOST_CHECK_EQUAL(deadbug4.birth(), 0);
  BOOST_CHECK_EQUAL(deadbug4.is_null("killer_id"), false);
  BOOST_CHECK_EQUAL(deadbug4.killer_id(), id);

  transaction.commit();
}

/**
 * Insert the data from a egg.
 */
BOOST_AUTO_TEST_CASE(deadbug_insert_egg)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);
  db::ID world_id1 = db::World::insert(&sw, 1, 1, sw::OrientationNorth);
  db::ID code_id = db::Code::insert(&sw, "code", 4);
  db::ID id1 = db::Bug::insert(&sw, code_id, 0);
  db::Egg::insert(&sw, id1, world_id1, 1000, db::Code::insert(&sw, code_id));
  db::Egg egg1(&sw, id1);
  db::DeadBug::insert(&sw, &egg1, 100);
  db::DeadBug deadbug1(&sw, id1);
  db::World::remove(&sw, world_id1);

  BOOST_CHECK_EQUAL(deadbug1.bug_id(), id1);
  BOOST_CHECK_EQUAL(deadbug1.death(), 100);
  BOOST_CHECK_EQUAL(deadbug1.is_null("birth"), true);
  BOOST_CHECK_EQUAL(deadbug1.is_null("killer_id"), true);
  BOOST_CHECK_THROW(db::Egg(&sw, id1).energy(), db::DBException);

  db::ID world_id2 = db::World::insert(&sw, 1, 1, sw::OrientationNorth);
  db::ID id2 = db::Bug::insert(&sw, code_id, 0);
  db::Egg::insert(&sw, id2, world_id2, 1000, db::Code::insert(&sw, code_id));
  db::Egg egg2(&sw, id2);
  db::DeadBug::insert(&sw, &egg2, 100, 1);
  db::DeadBug deadbug2(&sw, id2);
  db::World::remove(&sw, world_id2);

  BOOST_CHECK_EQUAL(deadbug2.bug_id(), id2);
  BOOST_CHECK_EQUAL(deadbug2.death(), 100);
  BOOST_CHECK_EQUAL(deadbug2.is_null("birth"), true);
  BOOST_CHECK_EQUAL(deadbug2.is_null("killer_id"), false);
  BOOST_CHECK_EQUAL(deadbug2.killer_id(), 1);
  BOOST_CHECK_THROW(db::Egg(&sw, id2).energy(), db::DBException);

  transaction.commit();
}

/**
 * Insert the data from a alive bug.
 */
BOOST_AUTO_TEST_CASE(deadbug_insert_alivebug)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);
  db::ID world_id1 = db::World::insert(&sw, 1, 1, sw::OrientationNorth);
  db::ID code_id = db::Code::insert(&sw, "code", 4);
  db::ID id1 = db::Bug::insert(&sw, code_id, 0);
  db::ID registers_id = db::Registers::insert(&sw);
  db::AliveBug::insert(&sw, id1, world_id1, 0, 134, registers_id,
		       db::Code::insert(&sw, code_id));
  db::AliveBug alive1(&sw, id1);
  db::DeadBug::insert(&sw, &alive1, 100);
  db::DeadBug deadbug1(&sw, id1);
  deadbug1.death();
  db::World::remove(&sw, world_id1);

  BOOST_CHECK_EQUAL(deadbug1.bug_id(), id1);
  BOOST_CHECK_EQUAL(deadbug1.death(), 100);
  BOOST_CHECK_EQUAL(deadbug1.is_null("birth"), false);
  BOOST_CHECK_EQUAL(deadbug1.birth(), 0);
  BOOST_CHECK_EQUAL(deadbug1.is_null("killer_id"), true);
  BOOST_CHECK_THROW(db::World(&sw, world_id1).position_x(), db::DBException);
  BOOST_CHECK_THROW(db::AliveBug(&sw, id1).energy(), db::DBException);

  db::ID world_id2 = db::World::insert(&sw, 1, 1, sw::OrientationNorth);
  db::ID id2 = db::Bug::insert(&sw, code_id, 0);
  registers_id = db::Registers::insert(&sw);
  code_id = db::Code::insert(&sw, "code", 4);
  db::AliveBug::insert(&sw, id2, world_id2, 100, 12, registers_id,
		       db::Code::insert(&sw, code_id));
  db::AliveBug alive2(&sw, id2);
  db::DeadBug::insert(&sw, &alive2, 150, 1);
  db::DeadBug deadbug2(&sw, id2);
  db::World::remove(&sw, world_id2);

  BOOST_CHECK_EQUAL(deadbug2.bug_id(), id2);
  BOOST_CHECK_EQUAL(deadbug2.death(), 150);
  BOOST_CHECK_EQUAL(deadbug2.is_null("birth"), false);
  BOOST_CHECK_EQUAL(deadbug2.birth(), 100);
  BOOST_CHECK_EQUAL(deadbug2.is_null("killer_id"), false);
  BOOST_CHECK_EQUAL(deadbug2.killer_id(), 1);
  BOOST_CHECK_THROW(db::World(&sw, world_id1).position_x(), db::DBException);
  BOOST_CHECK_THROW(db::AliveBug(&sw, id).birth(), db::DBException);

  transaction.commit();
}

/**
 * Update the data.
 */
BOOST_AUTO_TEST_CASE(deadbug_update)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);
  db::DeadBug deadbug(&sw, id);
  deadbug.birth(78);
  deadbug.death(108);
  deadbug.killer_id(2);

  BOOST_CHECK_EQUAL(deadbug.bug_id(), id);
  BOOST_CHECK_EQUAL(deadbug.death(), 108);
  BOOST_CHECK_EQUAL(deadbug.is_null("birth"), false);
  BOOST_CHECK_EQUAL(deadbug.birth(), 78);
  BOOST_CHECK_EQUAL(deadbug.is_null("killer_id"), false);
  BOOST_CHECK_EQUAL(deadbug.killer_id(), 2);

  transaction.commit();
}

/**
 * Delete the data.
 */
BOOST_AUTO_TEST_CASE(deadbug_delete)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);

  BOOST_CHECK_NO_THROW(db::DeadBug::remove(&sw, id));
  BOOST_CHECK_THROW(db::DeadBug(&sw, id).death(), db::DBException);

  transaction.commit();
}
