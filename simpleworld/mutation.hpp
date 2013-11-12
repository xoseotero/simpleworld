/**
 * @file simpleworld/mutation.hpp
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

#ifndef SIMPLEWORLD_MUTATION_HPP
#define SIMPLEWORLD_MUTATION_HPP

#include <list>

#include <simpleworld/types.hpp>
#include <simpleworld/cpu/types.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/mutation.hpp>

namespace simpleworld
{

/**
 * Description of a mutation
 */
struct Mutation
{
  db::Mutation::Type type;
  cpu::Address address;
  cpu::Word old_value;
  cpu::Word new_value;
};

/**
 * A list of mutations
 */
typedef std::list<Mutation> MutationsList;


/**
 * Mutate the code of a bug.
 * @param list pointer to the list of mutations.
 * @param db pointer to the database.
 * @param code_id id of the code.
 * @param probability probability to happen a mutation.
 * @return if the code was mutated.
 */
bool mutate(MutationsList* list, db::DB* db, db::ID code_id, float probability);

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
            float probability);

/**
 * Insert the mutations in the Mutation table of the database.
 * @param list pointer to the list of mutations.
 * @param db pointer to the database.
 * @param bug_id id of the bug.
 * @param time the current time.
 */
void update_mutations(MutationsList* list, db::DB* db, db::ID bug_id,
                      Time time);

}

#endif // SIMPLEWORLD_MUTATION_HPP
