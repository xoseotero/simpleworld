/**
 * @file tests/db/db_test.cpp
 * Unit test for db::Bug.
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

#define BOOST_TEST_MODULE Unit test for db::Bug
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/ints.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/exception.hpp>
#include <simpleworld/db/db.hpp>
#include <simpleworld/db/transaction.hpp>
#include <simpleworld/db/code.hpp>
#include <simpleworld/db/bug.hpp>
namespace sw = simpleworld;
namespace db = simpleworld::db;

#include "opendb.hpp"


#define DB_FILE (TESTDATA "db.sw")
#define DB_SAVE (TESTOUTPUT "bug.sw")

#define REGISTERS_SIZE TOTAL_REGISTERS * sizeof(cpu::Word)

/**
 * Get the data.
 */
BOOST_AUTO_TEST_CASE(bug_get)
{
  db::DB sw(DB_FILE);

  db::Bug bug1(&sw, 1);
  BOOST_CHECK_EQUAL(bug1.id(), 1);
  BOOST_CHECK_EQUAL(bug1.creation(), 1);
  BOOST_CHECK_EQUAL(bug1.is_null("father_id"), true);

  db::Bug bug2(&sw, 2);
  BOOST_CHECK_EQUAL(bug2.id(), 2);
  BOOST_CHECK_EQUAL(bug2.creation(), 101);
  BOOST_CHECK_EQUAL(bug2.is_null("father_id"), true);

  db::Bug bug3(&sw, 3);
  BOOST_CHECK_EQUAL(bug3.id(), 3);
  BOOST_CHECK_EQUAL(bug3.creation(), 200);
  BOOST_CHECK_EQUAL(bug3.father_id(), 1);
  BOOST_CHECK_EQUAL(bug3.is_null("father_id"), false);
}

db::ID id;

/**
 * Insert the data.
 */
BOOST_AUTO_TEST_CASE(bug_insert)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);
  db::ID code_id = db::Code::insert(&sw, "code", 4);
  db::ID father_id = db::Bug::insert(&sw, code_id, 0);
  id = db::Bug::insert(&sw, code_id, 0, father_id);
  db::Bug father(&sw, father_id);
  db::Bug son(&sw, id);

  BOOST_CHECK_EQUAL(father.id(), father_id);
  BOOST_CHECK_EQUAL(father.creation(), 0);
  BOOST_CHECK_EQUAL(father.is_null("father_id"), true);
  BOOST_CHECK_EQUAL(son.id(), id);
  BOOST_CHECK_EQUAL(son.creation(), 0);
  BOOST_CHECK_EQUAL(son.is_null("father_id"), false);
  BOOST_CHECK_EQUAL(son.father_id(), father_id);

  transaction.commit();
}


/**
 * Update the data.
 */
BOOST_AUTO_TEST_CASE(bug_update)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);
  db::ID code_id = db::Code::insert(&sw, "new code", 4);
  db::ID father_id = db::Bug::insert(&sw, code_id, 0);
  db::Bug bug(&sw, id);
  bug.father_id(father_id);

  BOOST_CHECK_EQUAL(bug.id(), id);
  BOOST_CHECK_EQUAL(bug.is_null("father_id"), false);
  BOOST_CHECK_EQUAL(bug.father_id(), father_id);

  transaction.commit();
}

/**
 * Delete the data.
 */
BOOST_AUTO_TEST_CASE(bug_delete)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);

  BOOST_CHECK_NO_THROW(db::Bug::remove(&sw, id));
  BOOST_CHECK_THROW(db::Bug(&sw, id).father_id(), db::DBException);

  transaction.commit();
}
