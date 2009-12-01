/**
 * @file src/simpleworld/simpleworld.cpp
 * Simple World.
 *
 *  Copyright (C) 2006-2008  Xosé Otero <xoseotero@users.sourceforge.net>
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
#include <string>
#include <cstdlib>
#include <ctime>
#include <cstring>

#include <getopt.h>

#include <boost/format.hpp>

#include <simpleworld/exception.hpp>
namespace sw = simpleworld;

#include "simpleworld.hpp"
#include "../printexc.hpp"


// information about the program.
const char* program_short_name = "simpleworld";
const char* program_name = "Simple World";
const char* program_version = "1.0b1-svn";
const char* program_years = "2006-2008";
const char* program_author = "Xosé Otero";
const char* program_author_email = "xoseotero@users.sourceforge.net";
const char* program_mailbugs = "simpleworld-list@lists.sourceforge.net";


/**
 * Show the usage of the program.
 * @param error a text to show as error.
 */
static void usage(std::string error)
{
  std::cerr << boost::format(\
"%1%: %2%\n\
Try `%1% --help' for more information.")
    % program_short_name
    % error
    << std::endl;

  std::exit(1);
}

/**
 * Show the help of the program.
 */
static void help()
{
  std::cout << boost::format(\
"Usage: %1% [COMMAND] [OPTION]... [DATABASE]\n\
The simulation of a world and the bugs that live on it.\n\
\n\
Mandatory arguments to long options are mandatory for short options too.\n\
  -h, --help                 display this help and exit\n\
  -v, --version              output version information and exit\n\
\n\
Available commands:\n\
  create                     create a new World\n\
  run                        execute some cycles\n\
  vacuum                     remove not used space from the database\n\
  info                       get information\n\
  env                        set the environment\n\
  egg                        add a new egg\n\
  food                       add a new food\n\
\n\
Exit status is 0 if OK, 1 if minor problems, 2 if serious trouble.\n\
\n\
Report bugs to <%2%>.")
    % program_short_name
    % program_mailbugs
    << std::endl;
  std::exit(EXIT_SUCCESS);
}

/**
 * Show the version of the program.
 */
static void version()
{
  std::cout << boost::format(\
"%1% (%2%) %3%\n\
\n\
Copyright (C) %4%, %5% <%6%>.\n\
This is free software. You may redistribute copies of it under the terms of\n\
the GNU General Public License <http://www.gnu.org/licenses/gpl.html>.\n\
There is NO WARRANTY, to the extent permitted by law.")
    % program_short_name
    % program_name
    % program_version
    % program_years
    % program_author
    % program_author_email
    << std::endl;

  std::exit(EXIT_SUCCESS);
}


/**
 * Parse the command line.
 * @param argc number of parameters.
 * @param argv parameters.
 */
static void parse_cmd(int argc, char* argv[])
{
  struct option long_options[] = {
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
    int c = getopt_long(argc, argv, "vh", long_options, &option_index);
    /* Detect the end of the options. */
    if (c == -1)
      break;
    switch (c)
    {
    case 'v':
      version();
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
}


int main(int argc, char* argv[])
try {
  std::srand(std::time(NULL));

  if (argc < 2)
    usage("no command specified");
  else
    if (std::strcmp(argv[1], "create") == 0)
      sw_create(argc - 1, argv + 1);
    else if (std::strcmp(argv[1], "run") == 0)
      sw_run(argc - 1, argv + 1);
    else if (std::strcmp(argv[1], "vacuum") == 0)
      sw_vacuum(argc - 1, argv + 1);
    else if (std::strcmp(argv[1], "info") == 0)
      sw_info(argc - 1, argv + 1);
    else if (std::strcmp(argv[1], "env") == 0)
      sw_env(argc - 1, argv + 1);
    else if (std::strcmp(argv[1], "egg") == 0)
      sw_egg(argc - 1, argv + 1);
    else if (std::strcmp(argv[1], "food") == 0)
      sw_food(argc - 1, argv + 1);
    else if (std::strlen(argv[1]) >= 1 and argv[1][0] != '-')
      usage(boost::str(boost::format("unrecognized command `%1%'")
                       % argv[1]));
    else
      parse_cmd(argc, argv);

  std::exit(EXIT_SUCCESS);
}
catch (const sw::Exception& e) {
  std::cout << e << std::endl;
}
catch (const std::exception& e) {
  std::cout << boost::format("Exception thrown: %1%") % e.what() << std::endl;
}
catch (...) {
  std::cout << "Unknown exception thrown" << std::endl;
}
