/**
 * @file src/info.cpp
 * Command info of Simple World.
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

#include <iostream>
#include <algorithm>
#include <iterator>
#include <deque>
#include <cstdio>
#include <cstdlib>

#include <getopt.h>

#include <boost/format.hpp>

#include <simpleworld/ints.hpp>
#include <simpleworld/types.hpp>
#include <simpleworld/element.hpp>
#include <simpleworld/simpleworld.hpp>
#include <simpleworld/cpu/memory.hpp>
#include <simpleworld/cpu/memory_file.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/exception.hpp>
#include <simpleworld/db/environment.hpp>
#include <simpleworld/db/food.hpp>
#include <simpleworld/db/egg.hpp>
#include <simpleworld/db/bug.hpp>
#include <simpleworld/db/deadbug.hpp>
#include <simpleworld/db/mutation.hpp>
namespace sw = simpleworld;
namespace cpu = simpleworld::cpu;
namespace db = simpleworld::db;

#include "simpleworld.hpp"
#include "table.hpp"


/**
 * Show the usage of the command.
 * @param error a text to show as error.
 */
static void usage(std::string error)
{
  std::cerr << boost::format(\
"%1% info: %2%\n\
Try `%1% info --help' for more information.")
    % program_short_name
    % error
    << std::endl;

  std::exit(1);
}

/**
 * Show the help of the command.
 */
static void help()
{
  std::cout << boost::format(\
"Usage: %1% info [OPTION]... [DATABASE]\n\
Show information about the World, by default the environment.\n\
\n\
Mandatory arguments to long options are mandatory for short options too.\n\
      --spawns               show the spawns\n\
      --bugs                 show the alive bugs\n\
      --eggs                 show the eggs\n\
      --foods                show the food\n\
\n\
      --sortenergy           show all the bugs sorted by energy\n\
      --sortage              show all the bugs sorted by age\n\
      --sortsons             show all the bugs sorted by number of sons\n\
      --sortkills            show all the bugs sorted by number of kills\n\
      --sortmutations        show all the bugs sorted by number of mutations\n\
\n\
      --bug=ID               information of a bug\n\
      --code=ID              get the code of a bug (saved as ID.swo)\n\
      --hierarchy=ID         hierarchy of a bug\n\
      --mutations=ID         mutations of a bug\n\
\n\
      --food=ID              information of a food\n\
\n\
      --version              show the version of the database\n\
\n\
  -h, --help                 display this help and exit\n\
\n\
Exit status is 0 if OK, 1 if minor problems, 2 if serious trouble.\n\
\n\
Report bugs to <%2%>.")
    % program_short_name
    % program_mailbugs
    << std::endl;
  std::exit(EXIT_SUCCESS);
}


// information from the command line
static std::string database_path;

static bool spawns_flag = false;
static bool bugs_flag = false;
static bool eggs_flag = false;
static bool foods_flag = false;

static bool sortenergy_flag = false;
static bool sortage_flag = false;
static bool sortsons_flag = false;
static bool sortkills_flag = false;
static bool sortmutations_flag = false;

static db::ID food_id = 0;
static bool bug_flag = false;
static db::ID bug_id = 0;
static bool code_flag = false;
static db::ID code_id = 0;
static bool hierarchy_flag = false;
static bool mutations_flag = false;

static bool food_flag = false;

static bool version_flag = false;

/**
 * Parse the command line.
 * @param argc number of parameters.
 * @param argv parameters.
 */
static void parse_cmd(int argc, char* argv[])
{
  struct option long_options[] = {
    {"spawns", no_argument, NULL, 'P'},
    {"bugs", no_argument, NULL, 'B'},
    {"eggs", no_argument, NULL, 'E'},
    {"foods", no_argument, NULL, 'F'},

    {"sortenergy", no_argument, NULL, 'N'},
    {"sortage", no_argument, NULL, 'G'},
    {"sortsons", no_argument, NULL, 'S'},
    {"sortkills", no_argument, NULL, 'K'},
    {"sortmutations", no_argument, NULL, 'M'},

    {"bug", required_argument, NULL, 'b'},
    {"code", required_argument, NULL, 'c'},
    {"hierarchy", required_argument, NULL, 'i'},
    {"mutations", required_argument, NULL, 'm'},

    {"food", required_argument, NULL, 'f'},

    {"version", no_argument, NULL, 'v'},

    {"help", no_argument, NULL, 'h'},

    {NULL, 0, NULL, 0}
  };

  // start the scan from the begining
  optind = 0;
  // avoid that getopt prints any message
  opterr = 0;
  while (true) {
    /* getopt_long stores the option index here. */
    int option_index = 0;
    int c = getopt_long(argc, argv, "h", long_options, &option_index);
    /* Detect the end of the options. */
    if (c == -1)
      break;
    switch (c)
    {
    case 'P': // spawns
      spawns_flag = true;
      break;
    case 'B': // bugs
      bugs_flag = true;
      break;
    case 'E': // eggs
      eggs_flag = true;
      break;
    case 'F': // foods
      foods_flag = true;
      break;

    case 'N': // sort by energy
      sortenergy_flag = true;
      break;
    case 'G': // sort by age
      sortage_flag = true;
      break;
    case 'S': // sort by number of sons
      sortsons_flag = true;
      break;
    case 'K': // sort by number of kills
      sortkills_flag = true;
      break;
    case 'M': // sort by number of mutations
      sortmutations_flag = true;
      break;

    case 'b': // bug
      bug_flag = true;
      {
        char* endptr;
        bug_id = std::strtoul(optarg, &endptr, 10);
        if (*endptr != '\0')
          usage(boost::str(boost::format("invalid argument `%1%'")
                           % optarg));
      }
      break;
    case 'c': // code
      code_flag = true;
      {
        char* endptr;
        code_id = std::strtoul(optarg, &endptr, 10);
        if (*endptr != '\0')
          usage(boost::str(boost::format("invalid argument `%1%'")
                           % optarg));
      }
      break;
    case 'i': // hierarchy
      hierarchy_flag = true;
      {
        char* endptr;
        bug_id = std::strtoul(optarg, &endptr, 10);
        if (*endptr != '\0')
          usage(boost::str(boost::format("invalid argument `%1%'")
                           % optarg));
      }
      break;
    case 'm': // mutations
      mutations_flag = true;
      {
        char* endptr;
        bug_id = std::strtoul(optarg, &endptr, 10);
        if (*endptr != '\0')
          usage(boost::str(boost::format("invalid argument `%1%'")
                           % optarg));
      }
      break;

    case 'v': // version
      version_flag = true;
      break;

    case 'h':
      help();
      break;

    case '?':
      if (optind <= 1)
        optind++;
      usage(boost::str(boost::format("unrecognized option `%1%'")
                       % argv[optind - 1]));
      break;

    default:
      abort();
    }
  }

  if (argc == optind)
    usage("a database file is needed");
  else if ((optind + 1) < argc)
    usage("too many database files");

  database_path = argv[optind];
}


/**
 * Show the environment of the World.
 * @param sw database.
 */
static void show_env(sw::SimpleWorld& sw)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(sw.db(), "\
SELECT time, size_x, size_y,\n\
       mutations_probability, time_birth, time_mutate, time_laziness,\n\
       energy_laziness, attack_multiplier,\n\
       energy_nothing, energy_myself, energy_detect, energy_info,\n\
       energy_move, energy_turn, energy_attack, energy_eat, energy_egg\n\
FROM Environment\n\
WHERE id = (SELECT max(id)\n\
            FROM Environment)\n\
ORDER BY id;", -1, &stmt, NULL))
    throw EXCEPTION(db::DBException, sqlite3_errmsg(sw.db()));
  show_query_line(true, "NULL", stmt);
  sqlite3_finalize(stmt);
}

/**
 * Show the spawns of the World.
 * @param sw database.
 */
static void show_spawns(sw::SimpleWorld& sw)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(sw.db(), "\
SELECT id, frequency, max, start_x, start_y, end_x, end_y\n\
FROM Spawn;", -1, &stmt, NULL))
    throw EXCEPTION(db::DBException, sqlite3_errmsg(sw.db()));
  show_query_column(true, 10, "NULL", stmt);
  sqlite3_finalize(stmt);
}

/**
 * Show the alive bugs of the World.
 * @param sw database.
 */
static void show_bugs(sw::SimpleWorld& sw)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(sw.db(), "\
SELECT *\n\
FROM AliveBug\n\
ORDER BY bug_id;", -1, &stmt, NULL))
    throw EXCEPTION(db::DBException, sqlite3_errmsg(sw.db()));
  show_query_column(true, 10, "NULL", stmt);
  sqlite3_finalize(stmt);
}

/**
 * Show the eggs of the World.
 * @param sw database.
 */
static void show_eggs(sw::SimpleWorld& sw)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(sw.db(), "\
SELECT *\n\
FROM Egg\n\
ORDER BY bug_id;", -1, &stmt, NULL))
    throw EXCEPTION(db::DBException, sqlite3_errmsg(sw.db()));
  show_query_column(true, 10, "NULL", stmt);
  sqlite3_finalize(stmt);
}

/**
 * Show all the food of the World.
 * @param sw database.
 */
static void show_foods(sw::SimpleWorld& sw)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(sw.db(), "\
SELECT *\n\
FROM Food\n\
ORDER BY id;", -1, &stmt, NULL))
    throw EXCEPTION(db::DBException, sqlite3_errmsg(sw.db()));
  show_query_column(true, 10, "NULL", stmt);
  sqlite3_finalize(stmt);
}

/**
 * Show all the bugs sorted by energy of the World.
 * @param sw database.
 */
static void show_sortenergy(sw::SimpleWorld& sw)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(sw.db(), "\
SELECT bug_id, energy\n\
FROM AliveBug\n\
ORDER BY energy DESC, bug_id;", -1, &stmt, NULL))
    throw EXCEPTION(db::DBException, sqlite3_errmsg(sw.db()));
  show_query_column(true, 10, "NULL", stmt);
  sqlite3_finalize(stmt);
}

/**
 * Show all the bugs sorted by age of the World.
 * @param sw database.
 */
static void show_sortage(sw::SimpleWorld& sw)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(sw.db(), "\
SELECT bug_id, (SELECT max(time) FROM Environment) - birth AS age\n\
FROM AliveBug\n\
UNION\n\
SELECT bug_id, dead - birth AS age\n\
FROM DeadBug\n\
ORDER BY age DESC, id;", -1, &stmt, NULL))
    throw EXCEPTION(db::DBException, sqlite3_errmsg(sw.db()));
  show_query_column(true, 10, "NULL", stmt);
  sqlite3_finalize(stmt);
}

/**
 * Show all the bugs sorted by the number of sons of the World.
 * @param sw database.
 */
static void show_sortsons(sw::SimpleWorld& sw)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(sw.db(), "\
SELECT father_id, count(father_id) AS sons\n\
FROM Bug\n\
WHERE father_id IS NOT NULL\n\
GROUP BY father_id\n\
ORDER BY sons DESC;", -1, &stmt, NULL))
    throw EXCEPTION(db::DBException, sqlite3_errmsg(sw.db()));
  show_query_column(true, 10, "NULL", stmt);
  sqlite3_finalize(stmt);
}

/**
 * Show all the bugs sorted by the number of kills of the World.
 * @param sw database.
 */
static void show_sortkills(sw::SimpleWorld& sw)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(sw.db(), "\
SELECT killer_id, count(killer_id) AS kills\n\
FROM DeadBug\n\
WHERE killer_id IS NOT NULL\n\
GROUP BY killer_id\n\
ORDER BY kills DESC, id;", -1, &stmt, NULL))
    throw EXCEPTION(db::DBException, sqlite3_errmsg(sw.db()));
  show_query_column(true, 10, "NULL", stmt);
  sqlite3_finalize(stmt);
}

/**
 * Show all the alive bugs sorted by the number of mutations of the World.
 * @param sw database.
 */
static void show_sortmutations(sw::SimpleWorld& sw)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(sw.db(), "\
SELECT bug_id, count(position) AS mutations\n\
FROM Mutation\n\
GROUP BY bug_id\n\
ORDER BY mutations DESC, id;", -1, &stmt, NULL))
    throw EXCEPTION(db::DBException, sqlite3_errmsg(sw.db()));
  show_query_column(true, 10, "NULL", stmt);
  sqlite3_finalize(stmt);
}

/**
 * Show information about a food of the World.
 * @param sw database.
 */
static void show_food(sw::SimpleWorld& sw)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(sw.db(), "\
SELECT *\n\
FROM Food\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(db::DBException, sqlite3_errmsg(sw.db()));
  sqlite3_bind_int64(stmt, 1, food_id);
  show_query_line(true, "NULL", stmt);
  sqlite3_finalize(stmt);
}

/**
 * Show information about a bug of the World.
 * @param sw database.
 */
static void show_bug(sw::SimpleWorld& sw)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(sw.db(), "\
SELECT *\n\
FROM AliveBug\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(db::DBException, sqlite3_errmsg(sw.db()));
  sqlite3_bind_int64(stmt, 1, bug_id);
  show_query_line(true, "NULL", stmt);
  sqlite3_finalize(stmt);
}

/**
 * Get the code of a bug of the World.
 * @param sw database.
 */
static void extract_code(sw::SimpleWorld& sw)
{
  try {
    db::Bug bug(&sw, code_id);
    sw::Uint32 size;
    boost::shared_array<sw::Uint8> data = bug.code().read(&size);
    cpu::MemoryFile code(cpu::Memory(data.get(), size));
    code.save_file(boost::str(boost::format("%1%.swo") % code_id));
  } catch (const db::DBException& e) {
    std::cerr << boost::format("\
Bug[%1%] not found")
      % code_id
      << std::endl;
  }
}

/**
 * Get the father of a bug.
 * @param sw database.
 * @param bug the bug.
 * @param father where to set the father id.
 * @return if the bug has father.
 */
static bool bug_father(sw::SimpleWorld& sw, db::ID bug, db::ID* father)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(sw.db(), "\
SSELECT father_id\n\
FROM Bug\n\
WHERE id = ?;", -1, &stmt, NULL))
    throw EXCEPTION(db::DBException, sqlite3_errmsg(sw.db()));
  sqlite3_bind_int64(stmt, 1, bug);
  switch(sqlite3_step(stmt)) {
  case SQLITE_DONE:
    sqlite3_finalize(stmt);
    return false;
  case SQLITE_ROW:
    *father = sqlite3_column_int64(stmt, 0);
    sqlite3_finalize(stmt);
    return true;
  default:
    sqlite3_finalize(stmt);
    throw EXCEPTION(db::DBException, sqlite3_errmsg(sw.db()));
  }
}

/**
 * Get the hierarchy of a bug.
 * @param sw database.
 * @param bug the bug.
 * @return the hierarchy.
 */
static std::deque<db::ID> bug_hierarchy(sw::SimpleWorld& sw, db::ID bug)
{
  std::deque<db::ID> hierarchy;
  db::ID father_id;
  while (bug_father(sw, bug, &father_id)) {
    if (father_id != 0)
      hierarchy.push_front(father_id);
    bug = father_id;
  }

  return hierarchy;
}

/**
 * Show the hierarchy of a bug of the World.
 * @param sw database.
 */
static void show_hierarchy(sw::SimpleWorld& sw)
{
  std::deque<db::ID> hierarchy = bug_hierarchy(sw, bug_id);
  std::copy(hierarchy.begin(), hierarchy.end(),
            std::ostream_iterator<int>(std::cout, "\n"));
}

/**
 * Show a mutation.
 * @param mutation the mutation.
 */
static void show_mutation(const db::Mutation& mutation)
{
  if (mutation.is_null("original"))
    std::cout << boost::format("Position %d(%d):\tNULL\t->\t0x%08x")
      % mutation.position()
      % mutation.time()
      % mutation.mutated()
      << std::endl;
  else if (mutation.is_null("mutated"))
    std::cout << boost::format("Position %d(%d):\t0x%08x\t->\tNULL")
      % mutation.position()
      % mutation.time()
      % mutation.original()
      << std::endl;
  else
    std::cout << boost::format("Position %d(%d):\t0x%08x\t->\t0x%08x")
      % mutation.position()
      % mutation.time()
      % mutation.original()
      % mutation.mutated()
      << std::endl;
}

/**
 * Show the mutations of a bug of the World.
 * @param sw database.
 */
static void show_mutations(sw::SimpleWorld& sw)
{
  std::deque<db::ID> hierarchy = bug_hierarchy(sw, bug_id);
  std::deque<db::ID>::const_iterator bug = hierarchy.begin();
  while (bug != hierarchy.end()) {
    std::vector<db::ID> mutations = db::Bug(&sw, *bug).mutations();
    std::vector<db::ID>::const_iterator mutation = 
      mutations.begin();
    while (mutation != mutations.end()) {
      show_mutation(db::Mutation(&sw, *mutation));

      ++mutation;
    }

    ++bug;
  }
}

/**
 * Show the version of the World.
 * @param sw database.
 */
static void show_version(sw::SimpleWorld& sw)
{
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(sw.db(), "\
PRAGMA user_version;", -1, &stmt, NULL))
    throw EXCEPTION(db::DBException, sqlite3_errmsg(sw.db()));
  show_query_line(true, "NULL", stmt);
  sqlite3_finalize(stmt);
}

/**
 * Simple World info command.
 * @param argc number of parameters.
 * @param argv array of parameters.
 */
void sw_info(int argc, char* argv[])
{
  parse_cmd(argc, argv);
  sw::SimpleWorld simpleworld(database_path);

  if (spawns_flag)
    show_spawns(simpleworld);
  else if (bugs_flag)
    show_bugs(simpleworld);
  else if (eggs_flag)
    show_eggs(simpleworld);
  else if (foods_flag)
    show_foods(simpleworld);
  else if (sortenergy_flag)
    show_sortenergy(simpleworld);
  else if (sortage_flag)
    show_sortage(simpleworld);
  else if (sortsons_flag)
    show_sortsons(simpleworld);
  else if (sortkills_flag)
    show_sortkills(simpleworld);
  else if (sortmutations_flag)
    show_sortmutations(simpleworld);
  else if (bug_flag)
    show_bug(simpleworld);
  else if (code_flag)
    extract_code(simpleworld);
  else if (hierarchy_flag)
    show_hierarchy(simpleworld);
  else if (mutations_flag)
    show_mutations(simpleworld);
  else if (food_flag)
    show_food(simpleworld);
  else if (version_flag)
    show_version(simpleworld);
  else
    show_env(simpleworld);
}
