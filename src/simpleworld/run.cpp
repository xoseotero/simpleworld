/**
 * @file src/simpleworld/run.cpp
 * Command run of Simple World.
 *
 * begin:     Tue, 10 Jun 2008 07:40:38 +0200
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
namespace sw = simpleworld;

#include "simpleworld.hpp"


// Default values
#define DEFAULT_CYCLES 1024
#define DEFAULT_VERBOSE 0


/**
 * Show the usage of the command.
 * @param error a text to show as error.
 */
static void usage(std::string error)
{
  std::cerr << boost::format(\
"%1% run: %2%\n\
Try `%1% run --help' for more information.")
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
"Usage: %1% run [OPTION]... [DATABASE]\n\
Execute some cycles in the World.\n\
If the number of cycles are not specified, the default are 1024 cycles.\n\
\n\
Mandatory arguments to long options are mandatory for short options too.\n\
      --cycles=CYCLES        cycles to run\n\
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

static sw::Time cycles = DEFAULT_CYCLES;

/**
 * Parse the command line.
 * @param argc number of parameters.
 * @param argv parameters.
 */
static void parse_cmd(int argc, char* argv[])
{
  struct option long_options[] = {
    {"cycles", required_argument, NULL, 'c'},

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
    case 'c': // cycles
      if (sscanf(optarg, "%d", &cycles) != 1)
        usage(boost::str(boost::format("Invalid value for --cycles (%1%)")
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
 * Simple World run command.
 * @param argc number of parameters.
 * @param argv array of parameters.
 */
void sw_run(int argc, char* argv[])
{
  parse_cmd(argc, argv);

  sw::SimpleWorld simpleworld(database_path);
  simpleworld.run(cycles);
}
