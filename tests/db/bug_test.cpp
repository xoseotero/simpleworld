/**
 * @file tests/db/db_test.cpp
 * Unit test for db::Bug.
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

#define BOOST_TEST_MODULE Unit test for db::Bug
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/ints.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/exception.hpp>
#include <simpleworld/db/db.hpp>
#include <simpleworld/db/bug.hpp>
namespace sw = simpleworld;
namespace db = simpleworld::db;


#define DB_FILE (TESTDATA "db.sw")
#define DB_SAVE (TESTOUTPUT "bug.sw")


/**
 * Get the data.
 */
BOOST_AUTO_TEST_CASE(bug_get)
{
  db::DB sw(DB_FILE);
  db::Bug bug1(&sw, 1);
  sw::Uint32 size1;
  boost::shared_array<sw::Uint8> code1 = bug1.code().read(&size1);
  db::Bug bug2(&sw, 2);
  sw::Uint32 size2;
  boost::shared_array<sw::Uint8> code2 = bug2.code().read(&size2);
  db::Bug bug3(&sw, 3);
  sw::Uint32 size3;
  boost::shared_array<sw::Uint8> code3 = bug3.code().read(&size3);

  BOOST_CHECK_EQUAL(bug1.id(), 1);
  BOOST_CHECK_EQUAL(bug1.is_null("father_id"), true);
  BOOST_CHECK_EQUAL(size1, 8);
  BOOST_CHECK_EQUAL(code1[0], 0x12);
  BOOST_CHECK_EQUAL(code1[1], 0x34);
  BOOST_CHECK_EQUAL(code1[2], 0x56);
  BOOST_CHECK_EQUAL(code1[3], 0x78);
  BOOST_CHECK_EQUAL(code1[4], 0x90);
  BOOST_CHECK_EQUAL(code1[5], 0xAB);
  BOOST_CHECK_EQUAL(code1[6], 0xCD);
  BOOST_CHECK_EQUAL(code1[7], 0xEF);
  BOOST_CHECK_EQUAL(bug2.id(), 2);
  BOOST_CHECK_EQUAL(bug2.is_null("father_id"), true);
  BOOST_CHECK_EQUAL(size2, 8);
  BOOST_CHECK_EQUAL(code2[0], 0x1A);
  BOOST_CHECK_EQUAL(code2[1], 0x2B);
  BOOST_CHECK_EQUAL(code2[2], 0x3C);
  BOOST_CHECK_EQUAL(code2[3], 0x4D);
  BOOST_CHECK_EQUAL(code2[4], 0x5E);
  BOOST_CHECK_EQUAL(code2[5], 0x6F);
  BOOST_CHECK_EQUAL(code2[6], 0x78);
  BOOST_CHECK_EQUAL(code2[7], 0x90);
  BOOST_CHECK_EQUAL(bug3.id(), 3);
  BOOST_CHECK_EQUAL(bug3.father_id(), 1);
  BOOST_CHECK_EQUAL(bug3.is_null("father_id"), false);
  BOOST_CHECK_EQUAL(size3, 8);
  BOOST_CHECK_EQUAL(code3[0], 0xAB);
  BOOST_CHECK_EQUAL(code3[1], 0xCD);
  BOOST_CHECK_EQUAL(code3[2], 0xEF);
  BOOST_CHECK_EQUAL(code3[3], 0x12);
  BOOST_CHECK_EQUAL(code3[4], 0x34);
  BOOST_CHECK_EQUAL(code3[5], 0x56);
  BOOST_CHECK_EQUAL(code3[6], 0x78);
  BOOST_CHECK_EQUAL(code3[7], 0x90);
}

db::ID id;

/**
 * Insert the data.
 */
BOOST_AUTO_TEST_CASE(bug_insert)
{
  db::DB sw(DB_SAVE);
  db::ID father_id = db::Bug::insert(&sw, "1234", 4);
  id = db::Bug::insert(&sw, father_id, "4321", 4);
  db::Bug father(&sw, father_id);
  sw::Uint32 father_size;
  boost::shared_array<sw::Uint8> father_code =
    father.code().read(&father_size);
  db::Bug son(&sw, id);
  sw::Uint32 son_size;
  boost::shared_array<sw::Uint8> son_code = son.code().read(&son_size);

  BOOST_CHECK_EQUAL(father.id(), father_id);
  BOOST_CHECK_EQUAL(father.is_null("father_id"), true);
  BOOST_CHECK_EQUAL(father_size, 4);
  BOOST_CHECK_EQUAL(father_code[0], '1');
  BOOST_CHECK_EQUAL(father_code[1], '2');
  BOOST_CHECK_EQUAL(father_code[2], '3');
  BOOST_CHECK_EQUAL(father_code[3], '4');
  BOOST_CHECK_EQUAL(son.id(), id);
  BOOST_CHECK_EQUAL(son.is_null("father_id"), false);
  BOOST_CHECK_EQUAL(son.father_id(), father_id);
  BOOST_CHECK_EQUAL(son_size, 4);
  BOOST_CHECK_EQUAL(son_code[0], '4');
  BOOST_CHECK_EQUAL(son_code[1], '3');
  BOOST_CHECK_EQUAL(son_code[2], '2');
  BOOST_CHECK_EQUAL(son_code[3], '1');
}


/**
 * Update the data.
 */
BOOST_AUTO_TEST_CASE(bug_update)
{
  db::DB sw(DB_SAVE);
  db::ID father_id = db::Bug::insert(&sw, "1234", 4);
  db::Bug bug(&sw, id);
  bug.father_id(father_id);
  bug.code().write("0101", 4);
  sw::Uint32 size;
  boost::shared_array<sw::Uint8> code = bug.code().read(&size);

  BOOST_CHECK_EQUAL(bug.id(), id);
  BOOST_CHECK_EQUAL(bug.is_null("father_id"), false);
  BOOST_CHECK_EQUAL(bug.father_id(), father_id);
  BOOST_CHECK_EQUAL(size, 4);
  BOOST_CHECK_EQUAL(code[0], '0');
  BOOST_CHECK_EQUAL(code[1], '1');
  BOOST_CHECK_EQUAL(code[2], '0');
  BOOST_CHECK_EQUAL(code[3], '1');
}

/**
 * Delete the data.
 */
BOOST_AUTO_TEST_CASE(bug_delete)
{
  db::DB sw(DB_SAVE);
  db::Bug::remove(&sw, id);

  BOOST_CHECK_THROW(db::Bug(&sw, id).father_id(), db::DBException);
}
