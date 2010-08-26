/**
 * @file tests/db/db_test.cpp
 * Unit test for DB::DB.
 *
 *  Copyright (C) 2007  Xosé Otero <xoseotero@gmail.com>
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
  db::DB sw(DB_SAVE);
  sw::Uint8 version = sw.version();

  BOOST_CHECK_EQUAL(version, db::DB(DB_SAVE).version());
}
