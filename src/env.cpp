/**
 * @file src/env.cpp
 * Command env of Simple World.
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


/**
 * Show the usage of the command.
 * @param error a text to show as error.
 */
static void usage(std::string error)
{
  std::cerr << boost::format(\
"%1% env: %2%\n\
Try `%1% env --help' for more information.")
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
"Usage: %1% env [OPTION]... [DATABASE]\n\
Set the environment of the World.\n\
The parameters not set are filled with the values of the last environment.\n\
The size of the World can change.\n\
\n\
Mandatory arguments to long options are mandatory for short options too.\n\
      --rot=CYCLES           cycles needed to rot the food\n\
      --srot=SIZE            size that is substracted to the food\n\
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

static sw::Time rot;
static sw::Energy srot;
static double mutations;
static sw::Time birth;
static sw::Time old;
static sw::Time laziness;
static sw::Energy elaziness;
static double multiplier;
static sw::Energy tnothing;
static sw::Energy tmyself;
static sw::Energy tdetect;
static sw::Energy tinfo;
static sw::Energy tmove;
static sw::Energy tturn;
static sw::Energy tattack;
static sw::Energy teat;
static sw::Energy tegg;
static sw::Energy nothing;
static sw::Energy myself;
static sw::Energy detect;
static sw::Energy info;
static sw::Energy move;
static sw::Energy turn;
static sw::Energy attack;
static sw::Energy eat;
static sw::Energy egg;

/**
 * Parse the command line.
 * @param argc number of parameters.
 * @param argv parameters.
 */
static void parse_cmd(int argc, char* argv[])
{
  struct option long_options[] = {
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
      case 'r': // rot
        if (sscanf(optarg, "%d", &rot) != 1)
          usage(boost::str(boost::format("Invalid value for --rot (%1%)")
          % optarg));
        break;
      case 't': // srot
        if (sscanf(optarg, "%d", &srot) != 1)
          usage(boost::str(boost::format("Invalid value for --srot (%1%)")
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

    case '0': // tnothing
      if (sscanf(optarg, "%d", &tnothing) != 1)
        usage(boost::str(boost::format("Invalid value for --tnothing (%1%)")
        % optarg));
      break;
    case '1': // tmyself
      if (sscanf(optarg, "%d", &tmyself) != 1)
        usage(boost::str(boost::format("Invalid value for --tmyself (%1%)")
        % optarg));
      break;
    case '2': // tdetect
      if (sscanf(optarg, "%d", &tdetect) != 1)
        usage(boost::str(boost::format("Invalid value for --tdetect (%1%)")
        % optarg));
      break;
    case '3': // tinfo
      if (sscanf(optarg, "%d", &tinfo) != 1)
        usage(boost::str(boost::format("Invalid value for --tinfo (%1%)")
        % optarg));
      break;
    case '4': // tmove
      if (sscanf(optarg, "%d", &tmove) != 1)
        usage(boost::str(boost::format("Invalid value for --tmove (%1%)")
        % optarg));
      break;
    case '5': // tturn
      if (sscanf(optarg, "%d", &tturn) != 1)
        usage(boost::str(boost::format("Invalid value for --tturn (%1%)")
        % optarg));
      break;
    case '6': // tattack
      if (sscanf(optarg, "%d", &tattack) != 1)
        usage(boost::str(boost::format("Invalid value for --tattack (%1%)")
        % optarg));
      break;
    case '7': // teat
      if (sscanf(optarg, "%d", &teat) != 1)
        usage(boost::str(boost::format("Invalid value for --teat (%1%)")
        % optarg));
      break;
    case '8': // tegg
      if (sscanf(optarg, "%d", &tegg) != 1)
        usage(boost::str(boost::format("Invalid value for --tegg (%1%)")
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
 * Simple World env command.
 * @param argc number of parameters.
 * @param argv array of parameters.
 */
void sw_env(int argc, char* argv[])
{
  // take the actual environment
  parse_cmd(argc, argv);

  sw::SimpleWorld simpleworld(database_path);
  rot = simpleworld.env().time_rot();
  srot = simpleworld.env().size_rot();
  mutations = simpleworld.env().mutations_probability();
  birth = simpleworld.env().time_birth();
  old = simpleworld.env().time_mutate();
  laziness = simpleworld.env().time_laziness();
  elaziness = simpleworld.env().energy_laziness();
  multiplier = simpleworld.env().attack_multiplier();
  tnothing = simpleworld.env().time_nothing();
  tmyself = simpleworld.env().time_myself();
  tdetect = simpleworld.env().time_detect();
  tinfo = simpleworld.env().time_info();
  tmove = simpleworld.env().time_move();
  tturn = simpleworld.env().time_turn();
  tattack = simpleworld.env().time_attack();
  teat = simpleworld.env().time_eat();
  tegg = simpleworld.env().time_egg();
  nothing = simpleworld.env().energy_nothing();
  myself = simpleworld.env().energy_myself();
  detect = simpleworld.env().energy_detect();
  info = simpleworld.env().energy_info();
  move = simpleworld.env().energy_move();
  turn = simpleworld.env().energy_turn();
  attack = simpleworld.env().energy_attack();
  eat = simpleworld.env().energy_eat();
  egg = simpleworld.env().energy_egg();

  // update the environment
  parse_cmd(argc, argv);

  db::Environment::insert(&simpleworld, simpleworld.env().time(),
                          simpleworld.env().size_x(),
                          simpleworld.env().size_y(), rot, srot,
                          mutations, birth, old, laziness, elaziness,
                          multiplier, tnothing, tmyself, tdetect, tinfo,
                          tmove, tturn, tattack, teat, tegg, nothing, myself,
                          detect, info, move, turn, attack, eat, egg);
}
