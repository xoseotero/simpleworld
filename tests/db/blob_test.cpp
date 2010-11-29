/**
 * @file tests/db/db_test.cpp
 * Unit test for db::Blob.
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

#define BOOST_TEST_MODULE Unit test for db::Blob
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/ints.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/db.hpp>
#include <simpleworld/db/bug.hpp>
#include <simpleworld/db/blob.hpp>
namespace sw = simpleworld;
namespace db = simpleworld::db;

#include "opendb.hpp"


#define DB_FILE (TESTDATA "db.sw")
#define DB_SAVE (TESTOUTPUT "blob.sw")


/**
 * Get the data.
 */
BOOST_AUTO_TEST_CASE(blob_get)
{
  db::DB sw(DB_FILE);
  db::Blob blob(db::Bug(&sw, 1).code());

  BOOST_CHECK_EQUAL(blob.size(), 8);
  sw::Uint32 size;
  boost::shared_array<sw::Uint8> data = blob.read(&size);
  BOOST_CHECK_EQUAL(size, 8);
  BOOST_CHECK_EQUAL(data[0], 0x12);
  BOOST_CHECK_EQUAL(data[1], 0x34);
  BOOST_CHECK_EQUAL(data[2], 0x56);
  BOOST_CHECK_EQUAL(data[3], 0x78);
  BOOST_CHECK_EQUAL(data[4], 0x90);
  BOOST_CHECK_EQUAL(data[5], 0xAB);
  BOOST_CHECK_EQUAL(data[6], 0xCD);
  BOOST_CHECK_EQUAL(data[7], 0xEF);
}

/**
 * Get a part of the data.
 */
BOOST_AUTO_TEST_CASE(blob_get_part)
{
  db::DB sw(DB_FILE);
  db::Blob blob(db::Bug(&sw, 2).code());

  boost::shared_array<sw::Uint8> data = blob.read(sizeof(sw::Uint8) * 4,
                                                  sizeof(sw::Uint8) * 3);

  BOOST_CHECK_EQUAL(data[0], 0x4D);
  BOOST_CHECK_EQUAL(data[1], 0x5E);
  BOOST_CHECK_EQUAL(data[2], 0x6F);
  BOOST_CHECK_EQUAL(data[3], 0x78);
}

/**
 * Update the data.
 */
BOOST_AUTO_TEST_CASE(blob_update)
{
  db::DB sw = open_db(DB_SAVE);
  sw::Uint8 code[8];
  code[0] = 0xAA;
  code[1] = 0xBB;
  code[2] = 0xCC;
  code[3] = 0xDD;
  code[4] = 0xEE;
  code[5] = 0xFF;
  code[6] = 0x00;
  code[7] = 0x11;
  db::ID id = db::Bug::insert(&sw, code, sizeof(code));

  db::Blob blob(db::Bug(&sw, id).code());
  sw::Uint8 newcode[4] = {0x55, 0x66, 0x77, 0x88};
  blob.write(&newcode, sizeof(newcode));

  sw::Uint32 size;
  boost::shared_array<sw::Uint8> data = blob.read(&size);

  BOOST_CHECK_EQUAL(blob.size(), sizeof(newcode));
  BOOST_CHECK_EQUAL(data[0], 0x55);
  BOOST_CHECK_EQUAL(data[1], 0x66);
  BOOST_CHECK_EQUAL(data[2], 0x77);
  BOOST_CHECK_EQUAL(data[3], 0x88);
}

/**
 * Update a part of the data.
 */
BOOST_AUTO_TEST_CASE(blob_update_part)
{
  db::DB sw = open_db(DB_SAVE);
  sw::Uint8 code[4];
  code[0] = 0xAA;
  code[1] = 0xBB;
  code[2] = 0xCC;
  code[3] = 0xDD;
  db::ID id = db::Bug::insert(&sw, code, sizeof(code));

  db::Blob blob(db::Bug(&sw, id).code());
  sw::Uint8 newcode = 0x88;
  blob.write(&newcode, sizeof(newcode), sizeof(sw::Uint8) * 1);

  sw::Uint32 size;
  boost::shared_array<sw::Uint8> data = blob.read(&size);

  BOOST_CHECK_EQUAL(blob.size(), 4);
  BOOST_CHECK_EQUAL(data[1], 0x88);
}
