/**
 * @file tests/db/db_test.cpp
 * Unit test for db::Environment.
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

#define BOOST_TEST_MODULE Unit test for db::Environment
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/ints.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/exception.hpp>
#include <simpleworld/db/db.hpp>
#include <simpleworld/db/environment.hpp>
namespace sw = simpleworld;
namespace db = simpleworld::db;

#include "opendb.hpp"


#define DB_FILE (TESTDATA "db.sw")
#define DB_SAVE (TESTOUTPUT "environment.sw")


/**
 * Get the data.
 */
BOOST_AUTO_TEST_CASE(environment_get)
{
  db::DB sw(DB_FILE);
  db::Environment environment(&sw, 1);

  BOOST_CHECK_EQUAL(environment.id(), 1);
  BOOST_CHECK_EQUAL(environment.time(), 200);
  BOOST_CHECK_EQUAL(environment.size_x(), 16);
  BOOST_CHECK_EQUAL(environment.size_y(), 16);
  BOOST_CHECK_EQUAL(environment.mutations_probability(), 0.001);
  BOOST_CHECK_EQUAL(environment.time_birth(), 32);
  BOOST_CHECK_EQUAL(environment.time_mutate(), 16384);
  BOOST_CHECK_EQUAL(environment.time_laziness(), 1024);
  BOOST_CHECK_EQUAL(environment.energy_laziness(), 16);
  BOOST_CHECK_EQUAL(environment.attack_multiplier(), 2.5);
  BOOST_CHECK_EQUAL(environment.energy_nothing(), 0);
  BOOST_CHECK_EQUAL(environment.energy_myself(), 1);
  BOOST_CHECK_EQUAL(environment.energy_detect(), 1);
  BOOST_CHECK_EQUAL(environment.energy_info(), 1);
  BOOST_CHECK_EQUAL(environment.energy_move(), 2);
  BOOST_CHECK_EQUAL(environment.energy_turn(), 2);
  BOOST_CHECK_EQUAL(environment.energy_attack(), 3);
  BOOST_CHECK_EQUAL(environment.energy_eat(), 3);
  BOOST_CHECK_EQUAL(environment.energy_egg(), 4);
}

db::ID id;

/**
 * Insert the data.
 */
BOOST_AUTO_TEST_CASE(environment_insert)
{
  db::DB sw = open_db(DB_SAVE);
  id = db::Environment::insert(&sw, 100, 16, 16, 0.01, 16, 65536, 2048, 32,
                               3.0, 1, 2, 2, 2, 3, 3, 4, 4, 5);
  db::Environment environment(&sw, id);

  BOOST_CHECK_EQUAL(environment.id(), id);
  BOOST_CHECK_EQUAL(environment.time(), 100);
  BOOST_CHECK_EQUAL(environment.size_x(), 16);
  BOOST_CHECK_EQUAL(environment.size_y(), 16);
  BOOST_CHECK_EQUAL(environment.mutations_probability(), 0.01);
  BOOST_CHECK_EQUAL(environment.time_birth(), 16);
  BOOST_CHECK_EQUAL(environment.time_mutate(), 65536);
  BOOST_CHECK_EQUAL(environment.time_laziness(), 2048);
  BOOST_CHECK_EQUAL(environment.energy_laziness(), 32);
  BOOST_CHECK_EQUAL(environment.attack_multiplier(), 3.0);
  BOOST_CHECK_EQUAL(environment.energy_nothing(), 1);
  BOOST_CHECK_EQUAL(environment.energy_myself(), 2);
  BOOST_CHECK_EQUAL(environment.energy_detect(), 2);
  BOOST_CHECK_EQUAL(environment.energy_info(), 2);
  BOOST_CHECK_EQUAL(environment.energy_move(), 3);
  BOOST_CHECK_EQUAL(environment.energy_turn(), 3);
  BOOST_CHECK_EQUAL(environment.energy_attack(), 4);
  BOOST_CHECK_EQUAL(environment.energy_eat(), 4);
  BOOST_CHECK_EQUAL(environment.energy_egg(), 5);
}


/**
 * Update the data.
 */
BOOST_AUTO_TEST_CASE(environment_update)
{
  db::DB sw = open_db(DB_SAVE);
  db::Environment environment(&sw, id);
  environment.time(101);
  environment.mutations_probability(0.02);
  environment.time_birth(17);
  environment.time_mutate(65537);
  environment.time_laziness(2049);
  environment.energy_laziness(33);
  environment.attack_multiplier(3.1);
  environment.energy_nothing(2);
  environment.energy_myself(3);
  environment.energy_detect(3);
  environment.energy_info(3);
  environment.energy_move(4);
  environment.energy_turn(4);
  environment.energy_attack(5);
  environment.energy_eat(5);
  environment.energy_egg(6);

  BOOST_CHECK_EQUAL(environment.id(), id);
  BOOST_CHECK_EQUAL(environment.time(), 101);
  BOOST_CHECK_EQUAL(environment.size_x(), 16);
  BOOST_CHECK_EQUAL(environment.size_y(), 16);
  BOOST_CHECK_EQUAL(environment.mutations_probability(), 0.02);
  BOOST_CHECK_EQUAL(environment.time_birth(), 17);
  BOOST_CHECK_EQUAL(environment.time_mutate(), 65537);
  BOOST_CHECK_EQUAL(environment.time_laziness(), 2049);
  BOOST_CHECK_EQUAL(environment.energy_laziness(), 33);
  BOOST_CHECK_EQUAL(environment.attack_multiplier(), 3.1);
  BOOST_CHECK_EQUAL(environment.energy_nothing(), 2);
  BOOST_CHECK_EQUAL(environment.energy_myself(), 3);
  BOOST_CHECK_EQUAL(environment.energy_detect(), 3);
  BOOST_CHECK_EQUAL(environment.energy_info(), 3);
  BOOST_CHECK_EQUAL(environment.energy_move(), 4);
  BOOST_CHECK_EQUAL(environment.energy_turn(), 4);
  BOOST_CHECK_EQUAL(environment.energy_attack(), 5);
  BOOST_CHECK_EQUAL(environment.energy_eat(), 5);
  BOOST_CHECK_EQUAL(environment.energy_egg(), 6);
}

/**
 * Delete the data.
 */
BOOST_AUTO_TEST_CASE(environment_delete)
{
  db::DB sw = open_db(DB_SAVE);
  db::Environment::remove(&sw, id);

  BOOST_CHECK_THROW(db::Environment(&sw, id).time(), db::DBException);
}
