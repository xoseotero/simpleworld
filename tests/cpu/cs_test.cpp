/**
 * @file tests/cpu/cs_test.cpp
 * Unit test for CPU::CS.
 *
 *  Copyright (C) 2008  Xosé Otero <xoseotero@gmail.com>
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

#define BOOST_TEST_MODULE Unit test for cpu::CS
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <simpleworld/config.hpp>
#include <simpleworld/ints.hpp>
#include <simpleworld/cpu/cs.hpp>
namespace sw = simpleworld;
namespace cpu = simpleworld::cpu;


/**
 * Encode a cs register.
 */
BOOST_AUTO_TEST_CASE(cs_encode)
{
  cpu::CS cs;
  cs.itp = 0xABCD;
  cs.cw = 0xA;
  cs.enable = true;
  cs.interrupt = false;
  cs.max_interrupts = 0x5;

#if defined(IS_BIG_ENDIAN)
  BOOST_CHECK_EQUAL(cs.encode(), 0xABCD0A05 | ENABLE_FLAG);
#elif defined(IS_LITTLE_ENDIAN)
  BOOST_CHECK_EQUAL(cs.encode(), 0x050ACDAB | ENABLE_FLAG);
#endif


  cs.itp = 0x0110;
  cs.cw = 0xF;
  cs.enable = false;
  cs.interrupt = true;
  cs.max_interrupts = 0xF;

#if defined(IS_BIG_ENDIAN)
  BOOST_CHECK_EQUAL(cs.encode(), 0x01100F0F | INTERRUPT_FLAG);
#elif defined(IS_LITTLE_ENDIAN)
  BOOST_CHECK_EQUAL(cs.encode(), 0x0F0F1001 | INTERRUPT_FLAG);
#endif
}

/**
 * Decode a cs register.
 */
BOOST_AUTO_TEST_CASE(cs_decode)
{
#if defined(IS_BIG_ENDIAN)
  sw::Uint32 cs_coded = 0xABCD0A05 | ENABLE_FLAG;
#elif defined(IS_LITTLE_ENDIAN)
  sw::Uint32 cs_coded = 0x050ACDAB | ENABLE_FLAG;
#endif
  cpu::CS cs(cs_coded);

  BOOST_CHECK_EQUAL(cs.itp, 0xABCD);
  BOOST_CHECK_EQUAL(cs.cw, 0xA);
  BOOST_CHECK_EQUAL(cs.enable, true);
  BOOST_CHECK_EQUAL(cs.interrupt, false);
  BOOST_CHECK_EQUAL(cs.max_interrupts, 0x5);


#if defined(IS_BIG_ENDIAN)
  cs_coded = 0x01100000 | INTERRUPT_FLAG | 0x00000F0F;
#elif defined(IS_LITTLE_ENDIAN)
  cs_coded = 0x00001001 | INTERRUPT_FLAG | 0x0F0F0000;
#endif
  cs.decode(cs_coded);

  BOOST_CHECK_EQUAL(cs.itp, 0x0110);
  BOOST_CHECK_EQUAL(cs.cw, 0xF);
  BOOST_CHECK_EQUAL(cs.enable, false);
  BOOST_CHECK_EQUAL(cs.interrupt, true);
  BOOST_CHECK_EQUAL(cs.max_interrupts, 0xf);
}
