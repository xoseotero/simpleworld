/**
 * @file src/egg.cpp
 * Command egg of Simple World.
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
#include <simpleworld/cpu/memory_file.hpp>
namespace sw = simpleworld;
namespace cpu = simpleworld::cpu;

#include "simpleworld.hpp"
#include "random.hpp"


// Default values
#define DEFAULT_ENERGY 128


/**
 * Show the usage of the command.
 * @param error a text to show as error.
 */
static void usage(std::string error)
{
  std::cerr << boost::format(\
"%1% egg: %2%\n\
Try `%1% egg --help' for more information.")
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
"Usage: %1% egg [OPTION]... [DATABASE]\n\
Add a egg to the World.\n\
The path to the code is needed.\n\
\n\
Mandatory arguments to long options are mandatory for short options too.\n\
      --position=X,Y         position of the egg\n\
      --orientation=ORIENT   orientation of the egg (from 0 to 3)\n\
      --energy=ENERGY        energy of the egg\n\
      --code=PATH            path to the code of the egg\n\
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

static sw::Position position;
static sw::Orientation orientation;
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
    {"position", required_argument, NULL, 'p'},
    {"orientation", required_argument, NULL, 'o'},
    {"energy", required_argument, NULL, 'e'},
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
    case 'p': // position
      if (sscanf(optarg, "%u,%u", &position.x, &position.y) != 2)
        usage(boost::str(boost::format("Invalid value for --position (%1%)")
                         % optarg));
      break;
    case 'o': // orientation
      if (sscanf(optarg, "%d", &orientation) != 1)
        usage(boost::str(boost::format("Invalid value for --orientation (%1%)")
                         % optarg));
      break;
    case 'e': // energy
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
 * Simple World egg command.
 * @param argc number of parameters.
 * @param argv array of parameters.
 */
void sw_egg(int argc, char* argv[])
{
  // set a random position and orientation as the default
  parse_cmd(argc, argv);
  sw::SimpleWorld simpleworld(database_path);
  position = random_position(simpleworld.world().size());
  orientation = random_orientation();

  parse_cmd(argc, argv);
  simpleworld.add_egg(energy, position, orientation,
                      cpu::MemoryFile(code_path));
}
