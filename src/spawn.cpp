/**
 * @file src/spawn.cpp
 * Command spawn of Simple World.
 *
 *  Copyright (C) 2010-2013  Xosé Otero <xoseotero@gmail.com>
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

#include <boost/scoped_array.hpp>
#include <boost/format.hpp>

#include <simpleworld/types.hpp>
#include <simpleworld/simpleworld.hpp>
#include <simpleworld/world.hpp>
#include <simpleworld/worlderror.hpp>
#include <simpleworld/cpu/memory_file.hpp>
#include <simpleworld/db/code.hpp>
#include <simpleworld/db/spawn.hpp>
namespace sw = simpleworld;
namespace cpu = simpleworld::cpu;
namespace db = simpleworld::db;

#include "simpleworld.hpp"


// Default values
#define DEFAULT_FREQUENCY 1024
#define DEFAULT_ENERGY 128


/**
 * Show the usage of the command.
 * @param error a text to show as error.
 */
static void usage(std::string error)
{
  std::cerr << boost::format(\
"%1% spawn: %2%\n\
Try `%1% spawn --help' for more information.")
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
"Usage: %1% spawn [OPTION]... [DATABASE]\n\
Add a spawn to the World.\n\
The path to the code is needed.\n\
\n\
Mandatory arguments to long options are mandatory for short options too.\n\
      --frequency=FREQ       frequency of the spawns\n\
      --max=MAX              maximum number of elements in the region\n\
      --start=X,Y            start of the region\n\
      --end=X,Y              end of the region\n\
      --energy=ENERGY        energy of new bugs\n\
      --code=PATH            path to the code of new bugs\n\
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

static sw::Time frequency = DEFAULT_FREQUENCY;
static sw::Uint16 max;
static sw::Position start;
static sw::Position end;
static sw::Energy energy = DEFAULT_ENERGY;
static std::string code_path;

/**
 * Parse the command line.
 * @param argc number of parameters.
 * @param argv parameters.
 */
static void parse_cmd(int argc, char* argv[])
{
  struct option long_options[] = {
    {"frequency", required_argument, NULL, 'f'},
    {"max", required_argument, NULL, 'm'},
    {"start", required_argument, NULL, 's'},
    {"end", required_argument, NULL, 'e'},
    {"energy", required_argument, NULL, 'n'},
    {"code", required_argument, NULL, 'c'},

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
    case 'f': // frequency
      if (sscanf(optarg, "%u", &frequency) != 1)
        usage(boost::str(boost::format("Invalid value for --energy (%1%)")
                         % optarg));
      break;
    case 'm': // max
      if (sscanf(optarg, "%hu", &max) != 1)
        usage(boost::str(boost::format("Invalid value for --max (%1%)")
                         % optarg));
      break;
    case 's': // start
      if (sscanf(optarg, "%hu,%hu", &start.x, &start.y) != 2)
        usage(boost::str(boost::format("Invalid value for --start (%1%)")
                         % optarg));
      break;
    case 'e': // end
      if (sscanf(optarg, "%hu,%hu", &end.x, &end.y) != 2)
        usage(boost::str(boost::format("Invalid value for --end (%1%)")
                         % optarg));
      break;
    case 'n': // energy
      if (sscanf(optarg, "%d", &energy) != 1)
        usage(boost::str(boost::format("Invalid value for --energy (%1%)")
                         % optarg));
      break;
    case 'c': // code
      code_path = optarg;
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

  if (code_path.empty())
    usage("the path to the code must be set");

  if (argc == optind)
    usage("a database file is needed");
  else if ((optind + 1) < argc)
    usage("too many database files");

  database_path = argv[optind];
}


/**
 * Simple World spawn command.
 * @param argc number of parameters.
 * @param argv array of parameters.
 */
void sw_spawn(int argc, char* argv[])
{
  parse_cmd(argc, argv);
  sw::SimpleWorld simpleworld(database_path);
  sw::World world(simpleworld.world());

  // set max, start and end as the default
  start.x = 0;
  start.y = 0;
  end.x = world.size().x;
  end.y = world.size().y;
  max = end.x - start.x * end.y - start.y;

  parse_cmd(argc, argv);

  cpu::MemoryFile code(code_path);
  boost::scoped_array<cpu::Word> data(new cpu::Word[code.size()]);
  for (cpu::Address i = 0; i < code.size(); i += sizeof(cpu::Word))
    data[i / sizeof(cpu::Word)] = code.get_word(i, false);
  db::Spawn::insert(&simpleworld,
                    db::Code::insert(&simpleworld, data.get(), code.size()),
                    frequency, max, start.x, start.y, end.x, end.y, energy);
}
