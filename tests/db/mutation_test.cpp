/**
 * @file tests/db/db_test.cpp
 * Unit test for db::Mutation.
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

#define BOOST_TEST_MODULE Unit test for db::Mutation
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/ints.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/exception.hpp>
#include <simpleworld/db/db.hpp>
#include <simpleworld/db/bug.hpp>
#include <simpleworld/db/mutation.hpp>
namespace sw = simpleworld;
namespace db = simpleworld::db;

#include "opendb.hpp"


#define DB_FILE (TESTDATA "db.sw")
#define DB_SAVE (TESTOUTPUT "mutation.sw")


/**
 * Get the data.
 */
BOOST_AUTO_TEST_CASE(mutation_get)
{
  db::DB sw(DB_FILE);
  db::Mutation mutation1(&sw, 1);
  db::Mutation mutation2(&sw, 2);
  db::Mutation mutation3(&sw, 3);

  BOOST_CHECK_EQUAL(mutation1.id(), 1);
  BOOST_CHECK_EQUAL(mutation1.bug_id(), 3);
  BOOST_CHECK_EQUAL(mutation1.time(), 200);
  BOOST_CHECK_EQUAL(mutation1.position(), 0);
  BOOST_CHECK_EQUAL(mutation1.original(), 1);
  BOOST_CHECK_EQUAL(mutation1.mutated(), 2);
  BOOST_CHECK_EQUAL(mutation1.is_null("original"), false);
  BOOST_CHECK_EQUAL(mutation1.is_null("mutated"), false);
  BOOST_CHECK_EQUAL(mutation2.id(), 2);
  BOOST_CHECK_EQUAL(mutation2.bug_id(), 3);
  BOOST_CHECK_EQUAL(mutation2.time(), 200);
  BOOST_CHECK_EQUAL(mutation2.position(), 4);
  BOOST_CHECK_EQUAL(mutation2.original(), 3);
  BOOST_CHECK_EQUAL(mutation2.is_null("original"), false);
  BOOST_CHECK_EQUAL(mutation2.is_null("mutated"), true);
  BOOST_CHECK_EQUAL(mutation3.id(), 3);
  BOOST_CHECK_EQUAL(mutation3.bug_id(), 3);
  BOOST_CHECK_EQUAL(mutation3.time(), 200);
  BOOST_CHECK_EQUAL(mutation3.position(), 4);
  BOOST_CHECK_EQUAL(mutation3.mutated(), 4);
  BOOST_CHECK_EQUAL(mutation3.is_null("original"), true);
  BOOST_CHECK_EQUAL(mutation3.is_null("mutated"), false);
}

db::ID id;
db::ID bug_id;

/**
 * Insert the data.
 */
BOOST_AUTO_TEST_CASE(mutation_insert)
{
  db::DB sw = open_db(DB_SAVE);
  bug_id = db::Bug::insert(&sw, "0123456789abcdef", 16);
  id = db::Mutation::insert(&sw, bug_id, 10, 0, 1, 2);
  db::ID id2 = db::Mutation::insert_addition(&sw, bug_id, 20, 4, 3);
  db::ID id3 = db::Mutation::insert_deletion(&sw, bug_id, 30, 8, 4);

  db::Mutation mutation1(&sw, id);
  db::Mutation mutation2(&sw, id2);
  db::Mutation mutation3(&sw, id3);

  BOOST_CHECK_EQUAL(mutation1.id(), id);
  BOOST_CHECK_EQUAL(mutation1.bug_id(), bug_id);
  BOOST_CHECK_EQUAL(mutation1.time(), 10);
  BOOST_CHECK_EQUAL(mutation1.position(), 0);
  BOOST_CHECK_EQUAL(mutation1.original(), 1);
  BOOST_CHECK_EQUAL(mutation1.mutated(), 2);
  BOOST_CHECK_EQUAL(mutation1.is_null("original"), false);
  BOOST_CHECK_EQUAL(mutation1.is_null("mutated"), false);
  BOOST_CHECK_EQUAL(mutation2.id(), id2);
  BOOST_CHECK_EQUAL(mutation2.bug_id(), bug_id);
  BOOST_CHECK_EQUAL(mutation2.time(), 20);
  BOOST_CHECK_EQUAL(mutation2.position(), 4);
  BOOST_CHECK_EQUAL(mutation2.mutated(), 3);
  BOOST_CHECK_EQUAL(mutation2.is_null("original"), true);
  BOOST_CHECK_EQUAL(mutation2.is_null("mutated"), false);
  BOOST_CHECK_EQUAL(mutation3.id(), id3);
  BOOST_CHECK_EQUAL(mutation3.bug_id(), bug_id);
  BOOST_CHECK_EQUAL(mutation3.time(), 30);
  BOOST_CHECK_EQUAL(mutation3.position(), 8);
  BOOST_CHECK_EQUAL(mutation3.original(), 4);
  BOOST_CHECK_EQUAL(mutation3.is_null("original"), false);
  BOOST_CHECK_EQUAL(mutation3.is_null("mutated"), true);
}


/**
 * Update the data.
 */
BOOST_AUTO_TEST_CASE(mutation_update)
{
  db::DB sw = open_db(DB_SAVE);
  db::Mutation mutation(&sw, id);
  mutation.time(15);
  mutation.position(4);
  mutation.original(3);
  mutation.mutated(4);

  BOOST_CHECK_EQUAL(mutation.id(), id);
  BOOST_CHECK_EQUAL(mutation.bug_id(), bug_id);
  BOOST_CHECK_EQUAL(mutation.time(), 15);
  BOOST_CHECK_EQUAL(mutation.position(), 4);
  BOOST_CHECK_EQUAL(mutation.original(), 3);
  BOOST_CHECK_EQUAL(mutation.mutated(), 4);
  BOOST_CHECK_EQUAL(mutation.is_null("original"), false);
  BOOST_CHECK_EQUAL(mutation.is_null("mutated"), false);
}

/**
 * Delete the data.
 */
BOOST_AUTO_TEST_CASE(mutation_delete)
{
  db::DB sw = open_db(DB_SAVE);
  db::Mutation::remove(&sw, id);

  BOOST_CHECK_THROW(db::Mutation(&sw, id).time(), db::DBException);
}
