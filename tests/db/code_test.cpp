/**
 * @file tests/db/code_test.cpp
 * Unit test for db::Code.
 *
 *  Copyright (C) 2013  Xosé Otero <xoseotero@gmail.com>
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

#define BOOST_TEST_MODULE Unit test for db::Code
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/ints.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/exception.hpp>
#include <simpleworld/db/db.hpp>
#include <simpleworld/db/transaction.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/code.hpp>
namespace sw = simpleworld;
namespace db = simpleworld::db;

#include "opendb.hpp"


#define DB_FILE (TESTDATA "db.sw")
#define DB_SAVE (TESTOUTPUT "code.sw")


/**
 * Get the data.
 */
BOOST_AUTO_TEST_CASE(code_get)
{
  db::DB sw(DB_FILE);
  db::Code code1(&sw, 1);
  sw::Uint32 size;
  boost::shared_array<sw::Uint8> data = code1.data().read(&size);
  BOOST_CHECK_EQUAL(code1.id(), 1);
  BOOST_CHECK_EQUAL(size, 8);
  BOOST_CHECK_EQUAL(data[0], 0xAB);
  BOOST_CHECK_EQUAL(data[1], 0xAB);
  BOOST_CHECK_EQUAL(data[2], 0xAB);
  BOOST_CHECK_EQUAL(data[3], 0xAB);
  BOOST_CHECK_EQUAL(data[4], 0xAB);
  BOOST_CHECK_EQUAL(data[5], 0xAB);
  BOOST_CHECK_EQUAL(data[6], 0xAB);
  BOOST_CHECK_EQUAL(data[7], 0xAB);

  db::Code code2(&sw, 2);
  data = code2.data().read(&size);
  BOOST_CHECK_EQUAL(code2.id(), 2);
  BOOST_CHECK_EQUAL(size, 8);
  BOOST_CHECK_EQUAL(data[0], 0xBA);
  BOOST_CHECK_EQUAL(data[1], 0xBA);
  BOOST_CHECK_EQUAL(data[2], 0xBA);
  BOOST_CHECK_EQUAL(data[3], 0xBA);
  BOOST_CHECK_EQUAL(data[4], 0xBA);
  BOOST_CHECK_EQUAL(data[5], 0xBA);
  BOOST_CHECK_EQUAL(data[6], 0xBA);
  BOOST_CHECK_EQUAL(data[7], 0xBA);

  db::Code code3(&sw, 3);
  data = code3.data().read(&size);
  BOOST_CHECK_EQUAL(code3.id(), 3);
  BOOST_CHECK_EQUAL(size, 8);
  BOOST_CHECK_EQUAL(data[0], 0x12);
  BOOST_CHECK_EQUAL(data[1], 0x34);
  BOOST_CHECK_EQUAL(data[2], 0x56);
  BOOST_CHECK_EQUAL(data[3], 0x78);
  BOOST_CHECK_EQUAL(data[4], 0x90);
  BOOST_CHECK_EQUAL(data[5], 0xAB);
  BOOST_CHECK_EQUAL(data[6], 0xCD);
  BOOST_CHECK_EQUAL(data[7], 0xEF);

  db::Code code4(&sw, 4);
  data = code4.data().read(&size);
  BOOST_CHECK_EQUAL(code4.id(), 4);
  BOOST_CHECK_EQUAL(size, 8);
  BOOST_CHECK_EQUAL(data[0], 0x1A);
  BOOST_CHECK_EQUAL(data[1], 0x2B);
  BOOST_CHECK_EQUAL(data[2], 0x3C);
  BOOST_CHECK_EQUAL(data[3], 0x4D);
  BOOST_CHECK_EQUAL(data[4], 0x5E);
  BOOST_CHECK_EQUAL(data[5], 0x6F);
  BOOST_CHECK_EQUAL(data[6], 0x78);
  BOOST_CHECK_EQUAL(data[7], 0x90);
}

db::ID id;

/**
 * Insert the data.
 */
BOOST_AUTO_TEST_CASE(code_insert)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);
  id = db::Code::insert(&sw, "ABCD", 4);

  db::Code code(&sw, id);
  sw::Uint32 size;
  boost::shared_array<sw::Uint8> test = code.data().read(&size);
  BOOST_CHECK_EQUAL(size, 4);
  BOOST_CHECK_EQUAL(test[0], 'A');
  BOOST_CHECK_EQUAL(test[1], 'B');
  BOOST_CHECK_EQUAL(test[2], 'C');
  BOOST_CHECK_EQUAL(test[3], 'D');

  transaction.commit();
}


/**
 * Update the data.
 */
BOOST_AUTO_TEST_CASE(code_update)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);
  db::Code code(&sw, id);
  sw::Uint32 size;
  boost::shared_array<sw::Uint8> data = code.data().read(&size);
  data[0] = 'E';
  code.data().write(data.get(), size);

  db::Code test(&sw, id);
  data = test.data().read(&size);

  BOOST_CHECK_EQUAL(test.id(), id);
  BOOST_CHECK_EQUAL(size, 4);
  BOOST_CHECK_EQUAL(data[0], 'E');
  BOOST_CHECK_EQUAL(data[1], 'B');
  BOOST_CHECK_EQUAL(data[2], 'C');
  BOOST_CHECK_EQUAL(data[3], 'D');

  transaction.commit();
}

/**
 * Delete the data.
 */
BOOST_AUTO_TEST_CASE(code_delete)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);

  BOOST_CHECK_NO_THROW(db::Code::remove(&sw, id));
  sw::Uint32 size;
  BOOST_CHECK_THROW(db::Code(&sw, id).data().read(&size), db::DBException);

  transaction.commit();
}
