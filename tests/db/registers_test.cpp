/**
 * @file tests/db/registers_test.cpp
 * Unit test for db::Registers.
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

#define BOOST_TEST_MODULE Unit test for db::Registers
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/ints.hpp>
#include <simpleworld/cpu/isa.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/exception.hpp>
#include <simpleworld/db/db.hpp>
#include <simpleworld/db/transaction.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/registers.hpp>
namespace sw = simpleworld;
namespace db = simpleworld::db;
namespace cpu = simpleworld::cpu;

#include "opendb.hpp"


#define DB_FILE (TESTDATA "db.sw")
#define DB_SAVE (TESTOUTPUT "registers.sw")

#define REGISTERS_SIZE TOTAL_REGISTERS * sizeof(cpu::Word)


/**
 * Get the data.
 */
BOOST_AUTO_TEST_CASE(registers_get)
{
  db::DB sw(DB_FILE);
  db::Registers registers(&sw, 1);
  sw::Uint32 size;
  boost::shared_array<sw::Uint8> data = registers.data().read(&size);
  sw::Uint8 test[] = {
    0x00, 0x00, 0x00, 0x00, 0x11, 0x11, 0x11, 0x11,
    0x22, 0x22, 0x22, 0x22, 0x33, 0x33, 0x33, 0x33,
    0x44, 0x44, 0x44, 0x44, 0x55, 0x55, 0x55, 0x55,
    0x66, 0x66, 0x66, 0x66, 0x77, 0x77, 0x77, 0x77,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05,
    0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05,
    0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05,
    0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05,
    0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05,
    0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05,
    0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05,
    0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05,
    0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05,
    0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05,
    0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05,
    0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05,
    0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05,
    0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05,
    0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05,
    0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05,
    0x00, 0x00, 0x00, 0x06, 0x00, 0x0, 0x00, 0x007
  };
  BOOST_CHECK_EQUAL(registers.id(), 1);
  BOOST_CHECK_EQUAL(size, REGISTERS_SIZE);
  for (int i = 0; i < REGISTERS_SIZE; i++)
    BOOST_CHECK_EQUAL(data[i], test[i]);
}

db::ID ids[2];

/**
 * Insert the data.
 */
BOOST_AUTO_TEST_CASE(registers_insert)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);
  sw::Uint8 data[REGISTERS_SIZE];
  for (int i = 0; i < REGISTERS_SIZE; i++)
    data[i] = static_cast<sw::Uint8>(i);
  ids[0] = db::Registers::insert(&sw, data, REGISTERS_SIZE);

  db::Registers reg0(&sw, ids[0]);
  sw::Uint32 size;
  boost::shared_array<sw::Uint8> test = reg0.data().read(&size);
  BOOST_CHECK_EQUAL(size, REGISTERS_SIZE);
  for (int i = 0; i < REGISTERS_SIZE; i++)
    BOOST_CHECK_EQUAL(test[i], static_cast<sw::Uint8>(i));

  ids[1] = db::Registers::insert(&sw);
  db::Registers reg1(&sw, ids[1]);
  test = reg1.data().read(&size);
  BOOST_CHECK_EQUAL(size, REGISTERS_SIZE);
  for (int i = 0; i < REGISTERS_SIZE; i++)
    BOOST_CHECK_EQUAL(test[i], 0);

  transaction.commit();
}


/**
 * Update the data.
 */
BOOST_AUTO_TEST_CASE(registers_update)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);
  db::Registers registers(&sw, ids[1]);
  sw::Uint32 size;
  boost::shared_array<sw::Uint8> data = registers.data().read(&size);
  data[0] = 0xFF;
  registers.data().write(data.get(), size);

  db::Registers test(&sw, ids[1]);
  data = test.data().read(&size);

  BOOST_CHECK_EQUAL(test.id(), ids[1]);
  BOOST_CHECK_EQUAL(size, REGISTERS_SIZE);
  BOOST_CHECK_EQUAL(data[0], 0xFF);
  for (int i = 1; i < REGISTERS_SIZE - 1; i++)
    BOOST_CHECK_EQUAL(data[i], data[i]);

  transaction.commit();
}

/**
 * Delete the data.
 */
BOOST_AUTO_TEST_CASE(registers_delete)
{
  db::DB sw = open_db(DB_SAVE);
  db::Transaction transaction(&sw, db::Transaction::deferred);

  BOOST_CHECK_NO_THROW(db::Registers::remove(&sw, ids[0]));
  BOOST_CHECK_NO_THROW(db::Registers::remove(&sw, ids[1]));
  sw::Uint32 size;
  BOOST_CHECK_THROW(db::Registers(&sw, ids[0]).data().read(&size),
                    db::DBException);
  BOOST_CHECK_THROW(db::Registers(&sw, ids[1]).data().read(&size),
                    db::DBException);

  transaction.commit();
}
