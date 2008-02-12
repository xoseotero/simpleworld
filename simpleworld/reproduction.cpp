/**
 * @file simpleworld/reproduction.cpp
 * Reproduction of bugs.
 *
 * begin:     Sun, 27 Jan 2008 05:57:39 +0100
 * last:      $Date$
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

#include <cstdlib>

#include <simpleworld/cpu/types.hpp>
#include <simpleworld/cpu/word.hpp>
#include <simpleworld/db/code.hpp>
#include <simpleworld/db/mutation.hpp>

#include "reproduction.hpp"

#ifdef DEBUG
#include <iostream>
#include <boost/format.hpp>
#endif // DEBUG

namespace SimpleWorld
{

/**
 * Get a random value in the range min <= n < max.
 * @param min the minimun value.
 * @param max the maximun value.
 * @return the random value.
 */
static int randint(int min, int max)
{
  return min + (((float) max - min) * (std::rand() / (RAND_MAX + 1.0)));
}

/**
 * Create a random word.
 * @return the random word.
 */
static ::SimpleWorld::CPU::Word random_word()
{
  ::SimpleWorld::CPU::Word word;
  unsigned int i;
  for (i = 0; i < sizeof(::SimpleWorld::CPU::Word); i++)
    ::SimpleWorld::CPU::set_byte(&word, i, randint(0, 256));

  return word;
}

/**
 * Add a random word to the copy.
 * @param copy where to add the word.
 * @param copy_pos position of the new word.
 * @param code original code.
 */
static void add_word(::SimpleWorld::DB::Code* copy,
                     ::SimpleWorld::CPU::Address copy_pos,
                     const ::SimpleWorld::DB::Code& code)
{
#ifdef DEBUG
  std::cout << boost::format("Addition of a random word")
    << std::endl;
#endif // DEBUG

  copy->code.resize(copy->code.size() + sizeof(::SimpleWorld::CPU::Word));
  copy->code.set_word(copy_pos, random_word());

  ::SimpleWorld::DB::Mutation mutation(code.db());
  mutation.position = copy_pos;
  mutation.type = ::SimpleWorld::DB::Mutation::addition;
  mutation.mutated = copy->code[copy_pos];
  mutation.add_null("original");
  copy->mutations.push_back(mutation);
}

/**
 * Eliminate a word from the copy.
 * @param copy where to add the word.
 * @param copy_pos position of the new word.
 * @param code original code.
 * @param code_pos position of the original word.
 */
static void eliminate_word(::SimpleWorld::DB::Code* copy,
                           ::SimpleWorld::CPU::Address copy_pos,
                           const ::SimpleWorld::DB::Code& code,
                           ::SimpleWorld::CPU::Address code_pos)
{
#ifdef DEBUG
  std::cout << boost::format("Deletion of a word")
    << std::endl;
#endif // DEBUG

  copy->code.resize(copy->code.size() - sizeof(::SimpleWorld::CPU::Word));

  ::SimpleWorld::DB::Mutation mutation(code.db());
  mutation.position = copy_pos;
  mutation.type = ::SimpleWorld::DB::Mutation::deletion;
  mutation.original = code.code[code_pos];
  mutation.add_null("mutated");
  copy->mutations.push_back(mutation);
}

/**
 * Change a word by a random one.
 * @param copy where to add the word.
 * @param copy_pos position of the new word.
 * @param code original code.
 * @param code_pos position of the original word.
 */
static void change_word(::SimpleWorld::DB::Code* copy,
                        ::SimpleWorld::CPU::Address copy_pos,
                        const ::SimpleWorld::DB::Code& code,
                        ::SimpleWorld::CPU::Address code_pos)
{
#ifdef DEBUG
  std::cout << boost::format("Change of a word")
    << std::endl;
#endif // DEBUG

  copy->code.set_word(copy_pos, random_word());

  // check if the new code is a mutation
  // when the code is changed, the changed code can be the same as the
  // original code
  if (copy->code[copy_pos] != code.code[code_pos]) {
    // this is a mutation
    ::SimpleWorld::DB::Mutation mutation(code.db());
    mutation.position = copy_pos;
    mutation.type = ::SimpleWorld::DB::Mutation::mutation;
    mutation.original = code.code[code_pos];
    mutation.mutated = copy->code[copy_pos];
    copy->mutations.push_back(mutation);
  }
}


/**
 * Get a copy of the code but with occasional mutations.
 * @param code the original code.
 * @return the copy of the code.
 */
::SimpleWorld::DB::Code copy_code(const ::SimpleWorld::DB::Code& code,
                                  float mutations_probability)
{
  ::SimpleWorld::DB::Code copy(code.db());
  copy.code.resize(code.code.size());

  ::SimpleWorld::CPU::Address code_pos = 0, copy_pos = 0;
  while (code_pos < code.code.size()) {
    // check if the word will be mutated
    if (randint(0, 1 / mutations_probability) == 0) {
      // mutation
      switch (randint(0, 3)) {
      case 0:
        // addition of a random word
        add_word(&copy, copy_pos, code);
        copy_pos += sizeof(::SimpleWorld::CPU::Word);

        break;

      case 1:
        // elimination of a word
        eliminate_word(&copy, copy_pos, code, code_pos);
        code_pos += sizeof(::SimpleWorld::CPU::Word);

        break;

      case 2:
        // change the word by a random one
        change_word(&copy, copy_pos, code, code_pos);
        code_pos += sizeof(::SimpleWorld::CPU::Word);
        copy_pos += sizeof(::SimpleWorld::CPU::Word);

        break;
      }
    } else {
      // same as original
      copy.code.set_word(copy_pos, code.code[code_pos]);

      code_pos += sizeof(::SimpleWorld::CPU::Word);
      copy_pos += sizeof(::SimpleWorld::CPU::Word);
    }
  }

  // addition of a random word at the end of the code
  if (randint(0, 1 / mutations_probability) == 0)
    add_word(&copy, copy_pos, code);

  copy.size = copy.code.size();

  return copy;
}

}
