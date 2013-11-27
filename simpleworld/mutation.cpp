/**
 * @file simpleworld/mutation.cpp
 * Mutation of bugs.
 *
 *  Copyright (C) 2008-2013  Xosé Otero <xoseotero@gmail.com>
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
#include <cstring>

#include <boost/shared_array.hpp>

#include <simpleworld/cpu/types.hpp>
#include <simpleworld/cpu/word.hpp>
#include <simpleworld/db/code.hpp>

#include "mutation.hpp"

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
* Partially mutate the word.
* @param word the original word.
* @return the mutated word.
*/
static cpu::Word partial_mutation(cpu::Word word)
{
  cpu::Word newword = word;
  unsigned int max = randint(1, 4);
  for (unsigned int i = 0; i < max; i++)
    cpu::set_byte(&newword, randint(0, 4), randint(0, 256));

  return newword;
}

/**
 * Permutate the word.
 * @param word the original word.
 * @return the permutated word.
 */
static cpu::Word permutation(cpu::Word word)
{
  cpu::Word newword;
  for (unsigned int i = 0; i < sizeof(cpu::Word); i++)
    cpu::set_byte(&newword, i, cpu::get_byte(word, randint(0, 4)));

  return newword;
}

/**
 * Generate mutations.
 * @param list pointer to a list of mutations.
 * @param size pointer to the size of the code.
 * @param probability probability to happen a mutation.
 * @return if the code is mutated.
 */
static bool generate(MutationsList* list, cpu::Address* size, float probability)
{
  // if the probability is 0, there is nothing to do
  if (probability == 0.0)
    return false;

  const int max = 1 / probability;
  cpu::Address i = 0;
  Mutation mutation;
  while (i < *size) {
    if (randint(0, max) == 0) {
      // mutation
      switch (randint(0, 6)) {
      case 0:                           // change the word by a random one
        mutation.type = db::Mutation::Total;
        mutation.address = i;
        list->push_back(mutation);
        i += sizeof(cpu::Word);
        break;

      case 1:                           // mutate part of the word
        mutation.type = db::Mutation::Partial;
        mutation.address = i;
        list->push_back(mutation);
        i += sizeof(cpu::Word);
        break;

      case 2:                           // permutate the word
        mutation.type = db::Mutation::Permutation;
        mutation.address = i;
        list->push_back(mutation);
        i += sizeof(cpu::Word);
        break;

      case 3:                           // addition of a random word
        mutation.type = db::Mutation::Addition;
        mutation.address = i;
        list->push_back(mutation);
        i += sizeof(cpu::Word);
        *size += sizeof(cpu::Word);
        break;

      case 4:                           // duplication of a word
        // if it's the begining of the code do a addition instead
        if (i == 0)
          mutation.type = db::Mutation::Addition;
        else
          mutation.type = db::Mutation::Duplication;
        mutation.address = i;
        list->push_back(mutation);
        i += sizeof(cpu::Word);
        *size += sizeof(cpu::Word);
        break;

      case 5:                           // elimination of a word
        mutation.type = db::Mutation::Deletion;
        mutation.address = i;
        list->push_back(mutation);
        *size -= sizeof(cpu::Word);
        break;
      }
    } else
      // no mutation
      i += sizeof(cpu::Word);
  }

  return not list->empty();
}


/**
 * Get a copy of the code but with occasional mutations.
 * @param list pointer to a list of mutations.
 * @param original the original code.
 * @param size the size of the code.
 * @return the code mutated.
 */
static boost::shared_array<Uint8> mutate(MutationsList* list,
                                         boost::shared_array<Uint8> original,
                                         Uint32 size)
{
  boost::shared_array<Uint8> mutated(new Uint8[size]);
  cpu::Address original_i = 0;
  cpu::Address mutated_i = 0;
  for (MutationsList::iterator iter = list->begin();
       iter != list->end();
       ++iter) {
    cpu::Address chunk_size = (*iter).address - mutated_i;
    if (chunk_size > 0)
      std::memcpy(mutated.get() + mutated_i, original.get() + original_i,
                  chunk_size);

    switch ((*iter).type) {
    case db::Mutation::Total:
#ifdef DEBUG
      std::cout << boost::format("Mutation of a word")
                << std::endl;
#endif // DEBUG

      {
        cpu::Word old_word = 
          *reinterpret_cast<cpu::Word*>(original.get() + chunk_size);
        cpu::Word new_word = random_word();
        if (old_word != new_word) {
          *reinterpret_cast<cpu::Word*>(mutated.get() + chunk_size) = new_word;
          (*iter).old_value = old_word;
          (*iter).new_value = new_word;
          original_i += chunk_size + sizeof(cpu::Word);
          mutated_i += chunk_size + sizeof(cpu::Word);
        }
      }

      break;

    case db::Mutation::Partial:
#ifdef DEBUG
      std::cout << boost::format("Partial mutation of a word")
                << std::endl;
#endif // DEBUG

      {
        cpu::Word old_word =
          *reinterpret_cast<cpu::Word*>(original.get() + chunk_size);
        cpu::Word new_word = partial_mutation(old_word);
        if (old_word != new_word) {
          *reinterpret_cast<cpu::Word*>(mutated.get() + chunk_size) = new_word;
          (*iter).old_value = old_word;
          (*iter).new_value = new_word;
          original_i += chunk_size + sizeof(cpu::Word);
          mutated_i += chunk_size + sizeof(cpu::Word);
        }
      }

      break;

    case db::Mutation::Permutation:
#ifdef DEBUG
      std::cout << boost::format("Permutation of a word")
                << std::endl;
#endif // DEBUG

      {
        cpu::Word old_word =
          *reinterpret_cast<cpu::Word*>(original.get() + chunk_size);
        cpu::Word new_word = permutation(old_word);
        if (old_word != new_word) {
          *reinterpret_cast<cpu::Word*>(mutated.get() + chunk_size) = new_word;
          (*iter).old_value = old_word;
          (*iter).new_value = new_word;
          original_i += chunk_size + sizeof(cpu::Word);
          mutated_i += chunk_size + sizeof(cpu::Word);
        }
      }

      break;

    case db::Mutation::Addition:
#ifdef DEBUG
      std::cout << boost::format("Addition of a random word")
                << std::endl;
#endif // DEBUG

      {
        cpu::Word new_word = random_word();
        *reinterpret_cast<cpu::Word*>(mutated.get() + chunk_size) = new_word;
        (*iter).new_value = new_word;
        original_i += chunk_size;
        mutated_i += chunk_size + sizeof(cpu::Word);
      }

      break;

    case db::Mutation::Duplication:
#ifdef DEBUG
      std::cout << boost::format("Duplication of a word")
                << std::endl;
#endif // DEBUG

      {
        cpu::Word new_word =
          *reinterpret_cast<cpu::Word*>(mutated.get() + chunk_size -
                                        sizeof(cpu::Word));
        *reinterpret_cast<cpu::Word*>(mutated.get() + chunk_size) = new_word;
        (*iter).new_value = new_word;
        original_i += chunk_size;
        mutated_i += chunk_size + sizeof(cpu::Word);
      }

      break;

    case db::Mutation::Deletion:
#ifdef DEBUG
      std::cout << boost::format("Deletion of a word")
                << std::endl;
#endif // DEBUG

      {
        cpu::Word old_word =
          *reinterpret_cast<cpu::Word*>(original.get() + chunk_size);
        (*iter).old_value = old_word;
        original_i += chunk_size + sizeof(cpu::Word);
        mutated_i += chunk_size;
      }

      break;
    }
  }

  if (mutated_i != size)
    std::memcpy(mutated.get() + mutated_i, original.get() + original_i,
                size - mutated_i);

  return mutated;
}


/**
 * Mutate the code of a bug.
 * @param list pointer to the list of mutations.
 * @param db pointer to the database.
 * @param code_id id of the code.
 * @param probability probability to happen a mutation.
 * @return if the code was mutated.
 */
bool mutate(MutationsList* list, db::DB* db, db::ID code_id, float probability)
{
  cpu::Address size = db::Code(db, code_id).data().size();
  if (generate(list, &size, probability)) {
    boost::shared_array<Uint8> mutated =
      mutate(list, db::Code(db, code_id).data().read(&size), size);
    db::Code(db, code_id).data().write(mutated.get(), size);

    return true;
  }

  return false;
}

/**
 * Create a copy of the code of a bug with mutations.
 * @param list pointer to the list of mutations.
 * @param db pointer to the database.
 * @param new_code id of the new code.
 * @param code_id id of the original code.
 * @param probability probability to happen a mutation.
 * @return if the code was mutated.
 */
bool mutate(MutationsList* list, db::DB* db, db::ID* new_code, db::ID code_id, 
            float probability)
{
  cpu::Address size = db::Code(db, code_id).data().size();
  if (generate(list, &size, probability)) {
    boost::shared_array<Uint8> mutated =
      mutate(list, db::Code(db, code_id).data().read(&size), size);
    *new_code = db::Code::insert(db, mutated.get(), size);

    return true;
  }

  return false;
}


/**
 * Insert the mutations in the Mutation table of the database.
 * @param list pointer to the list of mutations.
 * @param db pointer to the database.
 * @param bug_id id of the bug.
 * @param time the current time.
 */
void update_mutations(MutationsList* list, db::DB* db, db::ID bug_id, Time time)
{
  for (MutationsList::const_iterator iter = list->begin();
       iter != list->end();
       ++iter)
    switch ((*iter).type) {
      case db::Mutation::Total:
        db::Mutation::insert_mutation(db, bug_id, time, (*iter).address,
                                      (*iter).old_value, (*iter).new_value);

        break;

      case db::Mutation::Partial:
        db::Mutation::insert_partial(db, bug_id, time, (*iter).address,
                                     (*iter).old_value, (*iter).new_value);

        break;

      case db::Mutation::Permutation:
        db::Mutation::insert_permutation(db, bug_id, time, (*iter).address,
                                         (*iter).old_value, (*iter).new_value);

        break;

      case db::Mutation::Addition:
        db::Mutation::insert_addition(db, bug_id, time, (*iter).address,
                                      (*iter).new_value);

        break;

      case db::Mutation::Duplication:
        db::Mutation::insert_duplication(db, bug_id, time, (*iter).address,
                                         (*iter).new_value);

        break;

      case db::Mutation::Deletion:
        db::Mutation::insert_deletion(db, bug_id, time, (*iter).address,
                                      (*iter).old_value);

      break;
    }
}

}
