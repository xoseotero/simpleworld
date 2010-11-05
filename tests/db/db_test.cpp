/**
 * @file tests/db/db_test.cpp
 * Unit test for db::DB.
 *
 *  Copyright (C) 2007-2010  Xosé Otero <xoseotero@gmail.com>
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

#define BOOST_TEST_MODULE Unit test for db::DB
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/ints.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/db.hpp>
namespace sw = simpleworld;
namespace db = simpleworld::db;


#define DB_FILE (TESTDATA "db.sw")
#define DB_SAVE (TESTOUTPUT "temp.sw")


/**
 * Get the version.
 */
BOOST_AUTO_TEST_CASE(db_version)
{
  db::DB sw(DB_FILE);

  BOOST_CHECK_EQUAL(sw.version(), db::DB(DB_SAVE).version());
}

/**
 * Get the environments.
 */
BOOST_AUTO_TEST_CASE(db_environments)
{
  db::DB sw(DB_FILE);
  std::vector<db::ID> envs = sw.environments();

  BOOST_CHECK_EQUAL(envs.size(), 1);
  BOOST_CHECK_EQUAL(envs[0], 1);
}

/**
 * Get the last environment.
 */
BOOST_AUTO_TEST_CASE(db_last_environment)
{
  db::DB sw(DB_FILE);
  db::ID env = sw.last_environment();

  BOOST_CHECK_EQUAL(env, 1);
}

/**
 * Get the eggs.
 */
BOOST_AUTO_TEST_CASE(db_eggs)
{
  db::DB sw(DB_FILE);
  std::vector<db::ID> eggs = sw.eggs();

  BOOST_CHECK_EQUAL(eggs.size(), 1);
  BOOST_CHECK_EQUAL(eggs[0], 3);
}

/**
 * Get the alive bugs.
 */
BOOST_AUTO_TEST_CASE(db_alive_bugs)
{
  db::DB sw(DB_FILE);
  std::vector<db::ID> alivebugs = sw.alive_bugs();

  BOOST_CHECK_EQUAL(alivebugs.size(), 1);
  BOOST_CHECK_EQUAL(alivebugs[0], 1);
}

/**
 * Get the dead bugs.
 */
BOOST_AUTO_TEST_CASE(db_dead_bugs)
{
  db::DB sw(DB_FILE);
  std::vector<db::ID> deadbugs = sw.dead_bugs();

  BOOST_CHECK_EQUAL(deadbugs.size(), 1);
  BOOST_CHECK_EQUAL(deadbugs[0], 2);
}

/**
 * Get the food.
 */
BOOST_AUTO_TEST_CASE(db_food)
{
  db::DB sw(DB_FILE);
  std::vector<db::ID> food = sw.food();

  BOOST_CHECK_EQUAL(food.size(), 1);
  BOOST_CHECK_EQUAL(food[0], 1);
}

