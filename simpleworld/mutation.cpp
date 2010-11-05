/**
 * @file simpleworld/mutation.cpp
 * Mutation of bugs.
 *
 *  Copyright (C) 2008-2010  Xosé Otero <xoseotero@gmail.com>
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

#include <sqlite3x.hpp>

#include <simpleworld/cpu/types.hpp>
#include <simpleworld/cpu/word.hpp>
#include <simpleworld/cpu/memory.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/exception.hpp>
#include <simpleworld/db/mutation.hpp>

#include "dbmemory.hpp"
#include "mutation.hpp"

#ifdef DEBUG
#include <iostream>
#include <boost/format.hpp>
#endif // DEBUG
#include <boost/concept_check.hpp>

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
 * @param bug bug to mutate.
 * @param mutated where to add the word.
 * @param mutated_pos position of the new word.
 * @param time the current time.
 */
static void add_word(db::Bug* bug, cpu::Memory& mutated,
                     cpu::Address mutated_pos, Time time)
{
#ifdef DEBUG
  std::cout << boost::format("Addition of a random word")
    << std::endl;
#endif // DEBUG

  mutated.resize(mutated.size() + sizeof(cpu::Word));
  cpu::Word newword = random_word();
  mutated.set_word(mutated_pos, newword, false);

  db::Mutation::insert_addition(bug->db(), bug->id(), time, mutated_pos,
                                newword);
}

/**
 * Eliminate a word from the copy.
 * @param bug bug to mutate.
 * @param mutated where to add the word.
 * @param mutated_pos position of the new word.
 * @param original original code.
 * @param original_pos position of the original word.
 * @param time the current time.
 */
static void eliminate_word(db::Bug* bug, cpu::Memory& mutated,
                           cpu::Address mutated_pos,
                           const cpu::Memory& original,
                           cpu::Address original_pos, Time time)
{
#ifdef DEBUG
  std::cout << boost::format("Deletion of a word")
    << std::endl;
#endif // DEBUG

  mutated.resize(mutated.size() - sizeof(cpu::Word));

  db::Mutation::insert_deletion(bug->db(), bug->id(), time, mutated_pos,
                                original.get_word(original_pos, false));
}

/**
 * Change a word by a random one.
 * @param bug bug to mutate.
 * @param mutated where to add the word.
 * @param mutated_pos position of the new word.
 * @param original original code.
 * @param original_pos position of the original word.
 * @param time the current time.
 * @return if the word was changed
 */
static bool change_word(db::Bug* bug, cpu::Memory& mutated,
                        cpu::Address mutated_pos, const cpu::Memory& original,
                        cpu::Address original_pos, Time time)
{
#ifdef DEBUG
  std::cout << boost::format("Change of a word")
    << std::endl;
#endif // DEBUG

  cpu::Word newword = random_word();
  mutated.set_word(mutated_pos, newword, false);

  // check if the new code is a mutation
  // when the code is changed, the changed code can be the same as the
  // original code
  if (original.get_word(original_pos, false) != newword) {
    db::Mutation::insert(bug->db(), bug->id(), time, mutated_pos,
                         original.get_word(original_pos, false), newword);

    return true;
  } else
    return false;
}


/**
 * Get a copy of the code but with occasional mutations.
 * @param bug bug to mutate.
 * @param probability probability to happen a mutation.
 * @param time the current time.
 */
bool mutate(db::Bug* bug, float probability, Time time)
{
  bool ret = false;

  // savepoint
  sqlite3x::sqlite3_command sql(*bug->db());

  try {
    sql.prepare("SAVEPOINT mutation;");

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(db::DBException, std::string(e.what()) +
                    " (" + bug->db()->errormsg() + ")");
  }

  DBMemory original(bug->code());
  cpu::Memory mutated(original.size());

  cpu::Address original_pos = 0, mutated_pos = 0;
  while (original_pos < original.size()) {
    // check if the word will be mutated
    if (randint(0, 1 / probability) == 0) {
      // mutation
      switch (randint(0, 3)) {
      case 0:
        // addition of a random word
        add_word(bug, mutated, mutated_pos, time);
        mutated_pos += sizeof(cpu::Word);

        break;

      case 1:
        // elimination of a word
        eliminate_word(bug, mutated, mutated_pos, original, original_pos,
                       time);
        original_pos += sizeof(cpu::Word);

        break;

      case 2:
        // change the word by a random one
        while (not change_word(bug, mutated, mutated_pos, original, original_pos,
                        time))
          ;
        original_pos += sizeof(cpu::Word);
        mutated_pos += sizeof(cpu::Word);

        break;
      }

      ret = true;
    } else {
      // same as original
      mutated.set_word(mutated_pos, original.get_word(original_pos, false),
                       false);

      original_pos += sizeof(cpu::Word);
      mutated_pos += sizeof(cpu::Word);
    }
  }

  // addition of a random word at the end of the code
  if (randint(0, 1 / probability) == 0) {
    add_word(bug, mutated, mutated_pos, time);
    ret = true;
  }

  original.assign(mutated);

  // Release savepoint
  try {
    sql.prepare("RELEASE mutation;");

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw EXCEPTION(db::DBException, std::string(e.what()) +
                    " (" + bug->db()->errormsg() + ")");
  }

  return ret;
}

}
