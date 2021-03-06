/**
 * @file src/create.cpp
 * Command create of Simple World.
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

#include <iostream>
#include <cstdio>

#include <getopt.h>

#include <boost/format.hpp>

#include <simpleworld/types.hpp>
#include <simpleworld/db/db.hpp>
#include <simpleworld/db/default.hpp>
namespace sw = simpleworld;
namespace db = simpleworld::db;

#include "simpleworld.hpp"


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
      --rot=CYCLES           time needed to rot the food\n\
      --srot=SIZE            size that is substracted to the food\n\
\n\
      --mutations=PROB       probability for a mutation\n\
\n\
      --birth=CYCLES         time needed for a egg to get born\n\
      --old=CYCLES           time needed to consider a bug old and mutate\n\
      --laziness=CYCLES      time needed to consider a bug lazy\n\
      --elaziness=ENERGY     energy substracted to a lazy bug\n\
\n\
      --multiplier=MULT      multiplier to the attack\n\
\n\
      --tnothing=ENERGY      time needed for doing action NOTHING\n\
      --tmyself=ENERGY       time needed for doing action MYSELF\n\
      --tdetect=ENERGY       time needed for doing action DETECT\n\
      --tinfo=ENERGY         time needed for doing action INFO\n\
      --tmove=ENERGY         time needed for doing action MOVE\n\
      --tturn=ENERGY         time needed for doing action TURN\n\
      --tattack=ENERGY       time needed for doing action ATTACK\n\
      --teat=ENERGY          time needed for doing action EAT\n\
      --tegg=ENERGY          time needed for doing action EGG\n\
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

/**
 * Parse the command line.
 * @param argc number of parameters.
 * @param argv parameters.
 */
static void parse_cmd(int argc, char* argv[])
{
  struct option long_options[] = {
    {"size", required_argument, NULL, 's'},

    {"rot", required_argument, NULL, 'r'},
    {"srot", required_argument, NULL, 't'},

    {"mutations", required_argument, NULL, 'm'},

    {"birth", required_argument, NULL, 'b'},
    {"old", required_argument, NULL, 'o'},
    {"laziness", required_argument, NULL, 'l'},
    {"elaziness", required_argument, NULL, 'e'},

    {"multiplier", required_argument, NULL, 'u'},

    {"tnothing", required_argument, NULL, '0'},
    {"tmyself", required_argument, NULL, '1'},
    {"tdetect", required_argument, NULL, '2'},
    {"tinfo", required_argument, NULL, '3'},
    {"tmove", required_argument, NULL, '4'},
    {"tturn", required_argument, NULL, '5'},
    {"tattack", required_argument, NULL, '6'},
    {"teat", required_argument, NULL, '7'},
    {"tegg", required_argument, NULL, '8'},
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
      if (sscanf(optarg, "%hu,%hu", &db::default_environment.size_x,
                 &db::default_environment.size_y) != 2)
        usage(boost::str(boost::format("Invalid value for --size (%1%)")
                         % optarg));
      break;

    case 'r': // rot
      if (sscanf(optarg, "%d", &db::default_environment.time_rot) != 1)
        usage(boost::str(boost::format("Invalid value for --rot (%1%)")
        % optarg));
      break;
    case 't': // srot
      if (sscanf(optarg, "%d", &db::default_environment.size_rot) != 1)
        usage(boost::str(boost::format("Invalid value for --srot (%1%)")
        % optarg));
      break;

    case 'm': // mutations
      if (sscanf(optarg, "%lf",
          &db::default_environment.mutations_probability) != 1)
        usage(boost::str(boost::format("Invalid value for --mutations (%1%)")
                         % optarg));
      break;

    case 'b': // birth
      if (sscanf(optarg, "%d", &db::default_environment.time_birth) != 1)
        usage(boost::str(boost::format("Invalid value for --birth (%1%)")
                         % optarg));
      break;
    case 'o': // old
      if (sscanf(optarg, "%d", &db::default_environment.time_mutate) != 1)
        usage(boost::str(boost::format("Invalid value for --old (%1%)")
                         % optarg));
      break;
    case 'l': // laziness
      if (sscanf(optarg, "%d", &db::default_environment.time_laziness) != 1)
        usage(boost::str(boost::format("Invalid value for --laziness (%1%)")
                         % optarg));
      break;
    case 'e': // elaziness
      if (sscanf(optarg, "%d", &db::default_environment.energy_laziness) != 1)
        usage(boost::str(boost::format("Invalid value for --elaziness (%1%)")
                         % optarg));
      break;

    case 'u': // multiplier
      if (sscanf(optarg, "%lf",
                 &db::default_environment.attack_multiplier) != 1)
        usage(boost::str(boost::format("Invalid value for --multiplier (%1%)")
                         % optarg));
      break;

    case '0': // tnothing
      if (sscanf(optarg, "%d", &db::default_environment.time_nothing) != 1)
        usage(boost::str(boost::format("Invalid value for --tnothing (%1%)")
        % optarg));
      break;
    case '1': // tmyself
      if (sscanf(optarg, "%d", &db::default_environment.time_myself) != 1)
        usage(boost::str(boost::format("Invalid value for --tmyself (%1%)")
        % optarg));
      break;
    case '2': // tdetect
      if (sscanf(optarg, "%d", &db::default_environment.time_detect) != 1)
        usage(boost::str(boost::format("Invalid value for --tdetect (%1%)")
        % optarg));
      break;
    case '3': // tinfo
      if (sscanf(optarg, "%d", &db::default_environment.time_info) != 1)
        usage(boost::str(boost::format("Invalid value for --tinfo (%1%)")
        % optarg));
      break;
    case '4': // tmove
      if (sscanf(optarg, "%d", &db::default_environment.time_move) != 1)
        usage(boost::str(boost::format("Invalid value for --tmove (%1%)")
        % optarg));
      break;
    case '5': // tturn
      if (sscanf(optarg, "%d", &db::default_environment.time_turn) != 1)
        usage(boost::str(boost::format("Invalid value for --tturn (%1%)")
        % optarg));
      break;
    case '6': // tattack
      if (sscanf(optarg, "%d", &db::default_environment.time_attack) != 1)
        usage(boost::str(boost::format("Invalid value for --tattack (%1%)")
        % optarg));
      break;
    case '7': // teat
      if (sscanf(optarg, "%d", &db::default_environment.time_eat) != 1)
        usage(boost::str(boost::format("Invalid value for --teat (%1%)")
        % optarg));
      break;
    case '8': // tegg
      if (sscanf(optarg, "%d", &db::default_environment.time_egg) != 1)
        usage(boost::str(boost::format("Invalid value for --tegg (%1%)")
        % optarg));
      break;
    case 'N': // nothing
      if (sscanf(optarg, "%d", &db::default_environment.energy_nothing) != 1)
        usage(boost::str(boost::format("Invalid value for --nothing (%1%)")
                         % optarg));
      break;
    case 'M': // myself
      if (sscanf(optarg, "%d", &db::default_environment.energy_myself) != 1)
        usage(boost::str(boost::format("Invalid value for --myself (%1%)")
                         % optarg));
      break;
    case 'D': // detect
      if (sscanf(optarg, "%d", &db::default_environment.energy_detect) != 1)
        usage(boost::str(boost::format("Invalid value for --detect (%1%)")
                         % optarg));
      break;
    case 'I': // info
      if (sscanf(optarg, "%d", &db::default_environment.energy_info) != 1)
        usage(boost::str(boost::format("Invalid value for --info (%1%)")
                         % optarg));
      break;
    case 'V': // move
      if (sscanf(optarg, "%d", &db::default_environment.energy_move) != 1)
        usage(boost::str(boost::format("Invalid value for --move (%1%)")
                         % optarg));
      break;
    case 'T': // turn
      if (sscanf(optarg, "%d", &db::default_environment.energy_turn) != 1)
        usage(boost::str(boost::format("Invalid value for --turn (%1%)")
                         % optarg));
      break;
    case 'A': // attack
      if (sscanf(optarg, "%d", &db::default_environment.energy_attack) != 1)
        usage(boost::str(boost::format("Invalid value for --attack (%1%)")
                         % optarg));
      break;
    case 'E': // eat
      if (sscanf(optarg, "%d", &db::default_environment.energy_eat) != 1)
        usage(boost::str(boost::format("Invalid value for --eat (%1%)")
                         % optarg));
      break;
    case 'G': // egg
      if (sscanf(optarg, "%d", &db::default_environment.energy_egg) != 1)
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

  db::DB::create(database_path);
}
