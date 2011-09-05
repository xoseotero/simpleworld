/**
 * @file simpleworld/mutation.cpp
 * Mutation of bugs.
 *
 *  Copyright (C) 2008-2011  Xosé Otero <xoseotero@gmail.com>
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

#include <list>
#include <utility>
#include <cstdlib>
#include <cstring>

#include <boost/scoped_array.hpp>

#include <simpleworld/cpu/types.hpp>
#include <simpleworld/cpu/word.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/mutation.hpp>

#include "mutation.hpp"

#ifdef DEBUG
#include <iostream>
#include <boost/format.hpp>
#endif // DEBUG

namespace simpleworld
{

/**
 * Types of mutations
 */
enum MutationType {
  Mutation,
  Partial,
  Permutation,
  Addition,
  Duplication,
  Deletion
};

/**
 * A list of mutations
 */
typedef std::list< std::pair<MutationType, cpu::Address> > MutationsList;


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
  const int max = 1 / probability;
  cpu::Address i = 0;
  while (i < *size) {
    if (randint(0, max) == 0) {
      // mutation
      switch (randint(0, 6)) {
      case 0:                           // change the word by a random one
        list->push_back(std::make_pair(Mutation, i));
        i += sizeof(cpu::Word);
        break;

      case 1:                           // mutate part of the word
        list->push_back(std::make_pair(Partial, i));
        i += sizeof(cpu::Word);
        break;

      case 2:                           // permutate the word
        list->push_back(std::make_pair(Permutation, i));
        i += sizeof(cpu::Word);
        break;

      case 3:                           // addition of a random word
        list->push_back(std::make_pair(Addition, i));
        i += sizeof(cpu::Word);
        *size += sizeof(cpu::Word);
        break;

      case 4:                           // duplication of a word
        // if it's the begining of the code do a addition instead
        if (i == 0)
          list->push_back(std::make_pair(Addition, i));
        else
          list->push_back(std::make_pair(Duplication, i));
        i += sizeof(cpu::Word);
        *size += sizeof(cpu::Word);
        break;

      case 5:                           // elimination of a word
        list->push_back(std::make_pair(Deletion, i));
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
* @param bug bug to mutate.
* @param probability probability to happen a mutation.
* @param time the current time.
*/
bool mutate(db::Bug* bug, float probability, Time time)
{
  Uint32 size;
  boost::shared_array<Uint8> original = bug->code().read(&size);

  MutationsList list;
  if (generate(&list, &size, probability)) {
    boost::scoped_array<Uint8> mutated(new Uint8[size]);
    cpu::Address original_i = 0;
    cpu::Address mutated_i = 0;
    MutationsList::const_iterator iter = list.begin();
    while (iter != list.end()) {
      cpu::Address chunk_size = (*iter).second - mutated_i;
      if (chunk_size > 0)
        std::memcpy(mutated.get() + mutated_i, original.get() + original_i,
                    chunk_size);

      switch ((*iter).first) {
      case Mutation:
#ifdef DEBUG
        std::cout << boost::format("Change of a word")
                  << std::endl;
#endif // DEBUG

        {
          cpu::Word old_word =
            *reinterpret_cast<cpu::Word*>(original.get() + chunk_size);
          cpu::Word new_word;
          do {
            new_word = random_word();
          } while (old_word == new_word);
          *reinterpret_cast<cpu::Word*>(mutated.get() + chunk_size) = new_word;
          db::Mutation::insert(bug->db(), bug->id(), time, (*iter).second,
                               old_word, new_word);
          original_i += chunk_size + sizeof(cpu::Word);
          mutated_i += chunk_size + sizeof(cpu::Word);
        }

        break;

      case Partial:
#ifdef DEBUG
        std::cout << boost::format("Partial change of a word")
                  << std::endl;
#endif // DEBUG

        {
          cpu::Word old_word =
            *reinterpret_cast<cpu::Word*>(original.get() + chunk_size);
          cpu::Word new_word;
          do {
            new_word = partial_mutation(old_word);
          } while (old_word == new_word);
          *reinterpret_cast<cpu::Word*>(mutated.get() + chunk_size) = new_word;
          db::Mutation::insert(bug->db(), bug->id(), time, (*iter).second,
                               old_word, new_word);
          original_i += chunk_size + sizeof(cpu::Word);
          mutated_i += chunk_size + sizeof(cpu::Word);
        }

        break;

      case Permutation:
#ifdef DEBUG
        std::cout << boost::format("Permutation of a word")
                  << std::endl;
#endif // DEBUG

        {
          cpu::Word old_word =
            *reinterpret_cast<cpu::Word*>(original.get() + chunk_size);
          cpu::Word new_word;
          do {
            new_word = permutation(old_word);
          } while (old_word == new_word);
          *reinterpret_cast<cpu::Word*>(mutated.get() + chunk_size) = new_word;
          db::Mutation::insert(bug->db(), bug->id(), time, (*iter).second,
                               old_word, new_word);
          original_i += chunk_size + sizeof(cpu::Word);
          mutated_i += chunk_size + sizeof(cpu::Word);
        }

        break;

      case Addition:
#ifdef DEBUG
        std::cout << boost::format("Addition of a random word")
                  << std::endl;
#endif // DEBUG

        {
          cpu::Word new_word = random_word();
          *reinterpret_cast<cpu::Word*>(mutated.get() + chunk_size) = new_word;
          db::Mutation::insert_addition(bug->db(), bug->id(), time,
                                        (*iter).second, new_word);
          original_i += chunk_size;
          mutated_i += chunk_size + sizeof(cpu::Word);
        }

        break;

      case Duplication:
#ifdef DEBUG
        std::cout << boost::format("Duplication of a word")
                  << std::endl;
#endif // DEBUG

        {
          cpu::Word new_word = *reinterpret_cast<cpu::Word*>(mutated.get() +
            chunk_size -sizeof(cpu::Word));
          *reinterpret_cast<cpu::Word*>(mutated.get() + chunk_size) = new_word;
          db::Mutation::insert_addition(bug->db(), bug->id(), time,
                                        (*iter).second, new_word);
          original_i += chunk_size;
          mutated_i += chunk_size + sizeof(cpu::Word);
        }

        break;

      case Deletion:
#ifdef DEBUG
        std::cout << boost::format("Deletion of a word")
                  << std::endl;
#endif // DEBUG

        {
          cpu::Word old_word =
            *reinterpret_cast<cpu::Word*>(original.get() + chunk_size);
          db::Mutation::insert_deletion(bug->db(), bug->id(), time,
                                        (*iter).second, old_word);
          original_i += chunk_size + sizeof(cpu::Word);
          mutated_i += chunk_size;
        }

        break;
      }

      ++iter;
    }

    if (mutated_i != size)
      std::memcpy(mutated.get() + mutated_i, original.get() + original_i,
                  size - mutated_i);

    bug->code().write(mutated.get(), size);

    return true;
  }

  return false;
}

}
