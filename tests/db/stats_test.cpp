/**
 * @file tests/db/db_test.cpp
 * Unit test for db::Stats.
 *
 *  Copyright (C) 2011  Xosé Otero <xoseotero@gmail.com>
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

#define BOOST_TEST_MODULE Unit test for db::Stats
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/ints.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/exception.hpp>
#include <simpleworld/db/db.hpp>
#include <simpleworld/db/world.hpp>
#include <simpleworld/db/stats.hpp>
namespace sw = simpleworld;
namespace db = simpleworld::db;

#include "opendb.hpp"


#define DB_FILE (TESTDATA "db.sw")
#define DB_SAVE (TESTOUTPUT "stats.sw")


/**
 * Get the data.
 */
BOOST_AUTO_TEST_CASE(stats_get)
{
  db::DB sw(DB_FILE);
  db::Stats stats(&sw, 1);

  BOOST_CHECK_EQUAL(stats.id(), 1);
  BOOST_CHECK_EQUAL(stats.time(), 1024);
  BOOST_CHECK_EQUAL(stats.families(), 90);
  BOOST_CHECK_EQUAL(stats.alive(), 100);
  BOOST_CHECK_EQUAL(stats.eggs(), 40);
  BOOST_CHECK_EQUAL(stats.food(), 32);
  BOOST_CHECK_EQUAL(stats.energy(), 10000);
  BOOST_CHECK_EQUAL(stats.mutations(), 5);
  BOOST_CHECK_EQUAL(stats.age(), 10240);
  BOOST_CHECK_EQUAL(stats.last_births(), 100);
  BOOST_CHECK_EQUAL(stats.last_sons(), 10);
  BOOST_CHECK_EQUAL(stats.last_deaths(), 105);
  BOOST_CHECK_EQUAL(stats.last_kills(), 99);
  BOOST_CHECK_EQUAL(stats.last_mutations(), 20);
}

db::ID id;

/**
 * Insert the data.
 */
BOOST_AUTO_TEST_CASE(stats_insert)
{
  db::DB sw = open_db(DB_SAVE);
  id = db::Stats::insert(&sw, 2048, 98, 101, 41, 33, 20000, 10, 10241,
                         1, 2, 3, 4, 5);
  db::Stats stats(&sw, id);

  BOOST_CHECK_EQUAL(stats.id(), id);
  BOOST_CHECK_EQUAL(stats.time(), 2048);
  BOOST_CHECK_EQUAL(stats.families(), 98);
  BOOST_CHECK_EQUAL(stats.alive(), 101);
  BOOST_CHECK_EQUAL(stats.eggs(), 41);
  BOOST_CHECK_EQUAL(stats.food(), 33);
  BOOST_CHECK_EQUAL(stats.energy(), 20000);
  BOOST_CHECK_EQUAL(stats.mutations(), 10);
  BOOST_CHECK_EQUAL(stats.age(), 10241);
  BOOST_CHECK_EQUAL(stats.last_births(), 1);
  BOOST_CHECK_EQUAL(stats.last_sons(), 2);
  BOOST_CHECK_EQUAL(stats.last_deaths(), 3);
  BOOST_CHECK_EQUAL(stats.last_kills(), 4);
  BOOST_CHECK_EQUAL(stats.last_mutations(), 5);
}


/**
 * Update the data.
 */
BOOST_AUTO_TEST_CASE(stats_update)
{
  db::DB sw = open_db(DB_SAVE);
  db::Stats stats(&sw, id);
  stats.time(4096);
  stats.families(96);
  stats.alive(99);
  stats.eggs(39);
  stats.food(31);
  stats.energy(20000);
  stats.mutations(4);
  stats.age(2024);
  stats.last_births(2);
  stats.last_sons(4);
  stats.last_deaths(6);
  stats.last_kills(8);
  stats.last_mutations(10);

  BOOST_CHECK_EQUAL(stats.id(), id);
  BOOST_CHECK_EQUAL(stats.time(), 4096);
  BOOST_CHECK_EQUAL(stats.families(), 96);
  BOOST_CHECK_EQUAL(stats.alive(), 99);
  BOOST_CHECK_EQUAL(stats.eggs(), 39);
  BOOST_CHECK_EQUAL(stats.food(), 31);
  BOOST_CHECK_EQUAL(stats.energy(), 20000);
  BOOST_CHECK_EQUAL(stats.mutations(), 4);
  BOOST_CHECK_EQUAL(stats.age(), 2024);
  BOOST_CHECK_EQUAL(stats.last_births(), 2);
  BOOST_CHECK_EQUAL(stats.last_sons(), 4);
  BOOST_CHECK_EQUAL(stats.last_deaths(), 6);
  BOOST_CHECK_EQUAL(stats.last_kills(), 8);
  BOOST_CHECK_EQUAL(stats.last_mutations(), 10);
}

/**
 * Delete the data.
 */
BOOST_AUTO_TEST_CASE(stats_delete)
{
  db::DB sw = open_db(DB_SAVE);
  db::Stats::remove(&sw, id);

  BOOST_CHECK_THROW(db::Stats(&sw, id).time(), db::DBException);
}
