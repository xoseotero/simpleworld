/**
 * @file src/simpleworld/create.cpp
 * Command create of Simple World.
 *
 * begin:     Mon, 09 Jun 2008 22:51:03 +0200
 * last:      $Date$
 *
 *  Copyright (C) 2008  Xosé Otero <xoseotero@users.sourceforge.net>
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
#include <cstdio>

#include <getopt.h>

#include <boost/format.hpp>

#include <simpleworld/types.hpp>
#include <simpleworld/simpleworld.hpp>
#include <simpleworld/db/environment.hpp>
namespace sw = simpleworld;
namespace db = simpleworld::db;

#include "simpleworld.hpp"


// Default values
#define DEFAULT_SIZE {16, 16}
#define DEFAULT_MUTATIONS 0.01
#define DEFAULT_BIRTH 32
#define DEFAULT_OLD 16 * 1024
#define DEFAULT_LAZINESS 1024
#define DEFAULT_ELAZINESS 64
#define DEFAULT_MULTIPLIER 2.5
#define DEFAULT_NOTHING 0
#define DEFAULT_MYSELF 1
#define DEFAULT_DETECT 1
#define DEFAULT_INFO 1
#define DEFAULT_MOVE 2
#define DEFAULT_TURN 1
#define DEFAULT_ATTACK 3
#define DEFAULT_EAT 3
#define DEFAULT_EGG 4


/**
 * Show the usage of the command.
 * @param error a text to show as error.
 */
static void usage(std::string error)
{
  std::cerr << boost::format(\
"%1% create: %2%\n\
Try `%1% create --help' for more information.")
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
"Usage: %1% create [OPTION]... [DATABASE]\n\
Create a void World.\n\
The parameters not set are filled with the default values.\n\
\n\
Mandatory arguments to long options are mandatory for short options too.\n\
      --size=X,Y             size of the World\n\
\n\
      --mutations=PROB       probability for a mutation\n\
\n\
      --birth=CYCLES         cycles needed for a egg to get born\n\
      --old=CYCLES           cycles needed to consider a bug old and mutate\n\
      --laziness=CYCLES      cycles needed to consider a bug lazy\n\
      --elaziness=ENERGY     energy substracted to a lazy bug\n\
\n\
      --multiplier=MULT      multiplier to the attack\n\
\n\
      --nothing=ENERGY       energy used for doing action NOTHING\n\
      --myself=ENERGY        energy used for doing action MYSELF\n\
      --detect=ENERGY        energy used for doing action DETECT\n\
      --info=ENERGY          energy used for doing action INFO\n\
      --move=ENERGY          energy used for doing action MOVE\n\
      --turn=ENERGY          energy used for doing action TURN\n\
      --attack=ENERGY        energy used for doing action ATTACK\n\
      --eat=ENERGY           energy used for doing action EAT\n\
      --egg=ENERGY           energy used for doing action EGG\n\
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

static sw::Position size = DEFAULT_SIZE;
static double mutations = DEFAULT_MUTATIONS;
static sw::Time birth = DEFAULT_BIRTH;
static sw::Time old = DEFAULT_OLD;
static sw::Time laziness = DEFAULT_LAZINESS;
static sw::Energy elaziness = DEFAULT_ELAZINESS;
static double multiplier = DEFAULT_MULTIPLIER;
static sw::Energy nothing = DEFAULT_NOTHING;
static sw::Energy myself = DEFAULT_MYSELF;
static sw::Energy detect = DEFAULT_DETECT;
static sw::Energy info = DEFAULT_INFO;
static sw::Energy move = DEFAULT_MOVE;
static sw::Energy turn = DEFAULT_TURN;
static sw::Energy attack = DEFAULT_ATTACK;
static sw::Energy eat = DEFAULT_EAT;
static sw::Energy egg = DEFAULT_EGG;

/**
 * Parse the command line.
 * @param argc number of parameters.
 * @param argv parameters.
 */
static void parse_cmd(int argc, char* argv[])
{
  struct option long_options[] = {
    {"size", required_argument, NULL, 's'},

    {"mutations", required_argument, NULL, 'm'},

    {"birth", required_argument, NULL, 'b'},
    {"old", required_argument, NULL, 'o'},
    {"laziness", required_argument, NULL, 'l'},
    {"elaziness", required_argument, NULL, 'e'},

    {"multiplier", required_argument, NULL, 'u'},

    {"nothing", required_argument, NULL, 'N'},
    {"myself", required_argument, NULL, 'M'},
    {"detect", required_argument, NULL, 'D'},
    {"info", required_argument, NULL, 'I'},
    {"move", required_argument, NULL, 'V'},
    {"turn", required_argument, NULL, 'T'},
    {"attack", required_argument, NULL, 'A'},
    {"eat", required_argument, NULL, 'E'},
    {"egg", required_argument, NULL, 'G'},

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
    case 's': // size
      if (sscanf(optarg, "%u,%u", &size.x, &size.y) != 2)
        usage(boost::str(boost::format("Invalid value for --size (%1%)")
                         % optarg));
      break;

    case 'm': // mutations
      if (sscanf(optarg, "%lf", &mutations) != 1)
        usage(boost::str(boost::format("Invalid value for --mutations (%1%)")
                         % optarg));
      break;

    case 'b': // birth
      if (sscanf(optarg, "%d", &birth) != 1)
        usage(boost::str(boost::format("Invalid value for --birth (%1%)")
                         % optarg));
      break;
    case 'o': // old
      if (sscanf(optarg, "%d", &old) != 1)
        usage(boost::str(boost::format("Invalid value for --old (%1%)")
                         % optarg));
      break;
    case 'l': // laziness
      if (sscanf(optarg, "%d", &laziness) != 1)
        usage(boost::str(boost::format("Invalid value for --laziness (%1%)")
                         % optarg));
      break;
    case 'e': // elaziness
      if (sscanf(optarg, "%d", &elaziness) != 1)
        usage(boost::str(boost::format("Invalid value for --elaziness (%1%)")
                         % optarg));
      break;

    case 'u': // multiplier
      if (sscanf(optarg, "%lf", &multiplier) != 1)
        usage(boost::str(boost::format("Invalid value for --multiplier (%1%)")
                         % optarg));
      break;

    case 'N': // nothing
      if (sscanf(optarg, "%d", &nothing) != 1)
        usage(boost::str(boost::format("Invalid value for --nothing (%1%)")
                         % optarg));
      break;
    case 'M': // myself
      if (sscanf(optarg, "%d", &myself) != 1)
        usage(boost::str(boost::format("Invalid value for --myself (%1%)")
                         % optarg));
      break;
    case 'D': // detect
      if (sscanf(optarg, "%d", &detect) != 1)
        usage(boost::str(boost::format("Invalid value for --detect (%1%)")
                         % optarg));
      break;
    case 'I': // info
      if (sscanf(optarg, "%d", &info) != 1)
        usage(boost::str(boost::format("Invalid value for --info (%1%)")
                         % optarg));
      break;
    case 'V': // move
      if (sscanf(optarg, "%d", &move) != 1)
        usage(boost::str(boost::format("Invalid value for --move (%1%)")
                         % optarg));
      break;
    case 'T': // turn
      if (sscanf(optarg, "%d", &turn) != 1)
        usage(boost::str(boost::format("Invalid value for --turn (%1%)")
                         % optarg));
      break;
    case 'A': // attack
      if (sscanf(optarg, "%d", &attack) != 1)
        usage(boost::str(boost::format("Invalid value for --attack (%1%)")
                         % optarg));
      break;
    case 'E': // eat
      if (sscanf(optarg, "%d", &eat) != 1)
        usage(boost::str(boost::format("Invalid value for --eat (%1%)")
                         % optarg));
      break;
    case 'G': // egg
      if (sscanf(optarg, "%d", &egg) != 1)
        usage(boost::str(boost::format("Invalid value for --egg (%1%)")
                         % optarg));
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
 * Simple World create command.
 * @param argc number of parameters.
 * @param argv array of parameters.
 */
void sw_create(int argc, char* argv[])
{
  parse_cmd(argc, argv);

  sw::SimpleWorld simpleworld(database_path);
  db::Environment env(&simpleworld);
  env.time = 0;
  env.size = size;
  env.mutations_probability = mutations;
  env.time_birth = birth;
  env.time_mutate = old;
  env.time_laziness = laziness;
  env.energy_laziness = elaziness;
  env.attack_multiplier = multiplier;
  env.energy_nothing = nothing;
  env.energy_myself = myself;
  env.energy_detect = detect;
  env.energy_info = info;
  env.energy_move = move;
  env.energy_turn = turn;
  env.energy_attack = attack;
  env.energy_eat = eat;
  env.energy_egg = egg;
  env.insert();
}