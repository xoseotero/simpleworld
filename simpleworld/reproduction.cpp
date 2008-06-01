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

namespace simpleworld
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
static cpu::Word random_word()
{
  cpu::Word word;
  for (unsigned int i = 0; i < sizeof(cpu::Word); i++)
    cpu::set_byte(&word, i, randint(0, 256));

  return word;
}

/**
 * Add a random word to the copy.
 * @param copy where to add the word.
 * @param copy_pos position of the new word.
 * @param code original code.
 * @param time the current time.
 */
static void add_word(db::Code* copy, cpu::Address copy_pos,
                     const db::Code& code,
                     Time time)
{
#ifdef DEBUG
  std::cout << boost::format("Addition of a random word")
    << std::endl;
#endif // DEBUG

  copy->code.resize(copy->code.size() + sizeof(cpu::Word));
  copy->code.set_word(copy_pos, random_word());

  db::Mutation mutation(code.db());
  mutation.time = time;
  mutation.position = copy_pos;
  mutation.type = db::Mutation::addition;
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
 * @param time the current time.
 */
static void eliminate_word(db::Code* copy, cpu::Address copy_pos,
                           const db::Code& code, cpu::Address code_pos,
                           Time time)
{
#ifdef DEBUG
  std::cout << boost::format("Deletion of a word")
    << std::endl;
#endif // DEBUG

  copy->code.resize(copy->code.size() - sizeof(cpu::Word));

  db::Mutation mutation(code.db());
  mutation.time = time;
  mutation.position = copy_pos;
  mutation.type = db::Mutation::deletion;
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
 * @param time the current time.
 */
static void change_word(db::Code* copy, cpu::Address copy_pos,
                        const db::Code& code, cpu::Address code_pos,
                        Time time)
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
    db::Mutation mutation(code.db());
    mutation.time = time;
    mutation.position = copy_pos;
    mutation.type = db::Mutation::mutation;
    mutation.original = code.code[code_pos];
    mutation.mutated = copy->code[copy_pos];
    copy->mutations.push_back(mutation);
  }
}


/**
 * Get a copy of the code but with occasional mutations.
 * @param code the original code.
 * @param probability probability to happen a mutation.
 * @param egg true if it's a egg, false if it's a bug.
 * @param time the current time.
 * @return the copy of the code.
 */
db::Code copy_code(const db::Code& code, float probability,
                   bool egg, Time time)
{
  db::Code copy(code.db(), code.id());
  if (egg)
    copy.mutations.clear();

  cpu::Address code_pos = 0, copy_pos = 0;
  while (code_pos < code.code.size()) {
    // check if the word will be mutated
    if (randint(0, 1 / probability) == 0) {
      // mutation
      switch (randint(0, 3)) {
      case 0:
        // addition of a random word
        add_word(&copy, copy_pos, code, time);
        copy_pos += sizeof(cpu::Word);

        break;

      case 1:
        // elimination of a word
        eliminate_word(&copy, copy_pos, code, code_pos, time);
        code_pos += sizeof(cpu::Word);

        break;

      case 2:
        // change the word by a random one
        change_word(&copy, copy_pos, code, code_pos, time);
        code_pos += sizeof(cpu::Word);
        copy_pos += sizeof(cpu::Word);

        break;
      }
    } else {
      // same as original
      copy.code.set_word(copy_pos, code.code[code_pos]);

      code_pos += sizeof(cpu::Word);
      copy_pos += sizeof(cpu::Word);
    }
  }

  // addition of a random word at the end of the code
  if (randint(0, 1 / probability) == 0)
    add_word(&copy, copy_pos, code, time);

  copy.size = copy.code.size();

  return copy;
}

}
