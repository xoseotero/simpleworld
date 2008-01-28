/**
 * @file src/swldc.cpp
 * Simple World Language decompiler
 *
 * begin:     Sun, 19 Nov 2006 17:58:49 +0100
 * last:      $Date$
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

#include <getopt.h>

#include <boost/format.hpp>

#include <simpleworld/config.hpp>
#include <simpleworld/exception.hpp>
#include <simpleworld/cpu/memory.hpp>
#include <simpleworld/cpu/object.hpp>
namespace sw = SimpleWorld;
namespace cpu = SimpleWorld::CPU;

#include "printexc.hpp"
#include "fakecpu.hpp"

#define DEFAULT_OUTPUT "out.swl"

const char* program_short_name = "swld";
const char* program_name = "Simple World Language decompiler";
const char* program_version = "1.0b1-svn";
const char* program_years = "2006-2008";
const char* program_author = "Xosé Otero";
const char* program_author_email = "xoseotero@users.sourceforge.net";
const char* program_mailbugs = "simpleworld-list@lists.sourceforge.net";


/**
 * Show the usage of the program.
 * @param error a text to show as error.
 */
void usage(std::string error)
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
void help()
{
  std::cout << boost::format(\
"Usage: %1% [OPTION]... [FILE]\n\
Simple World Language decompiler.\n\
\n\
Mandatory arguments to long options are mandatory for short options too.\n\
  -o, --output=FILE          place the output into FILE\n\
                               the default value is %2%\n\
\n\
  -h, --help                 display this help and exit\n\
  -v, --version              output version information and exit\n\
\n\
Exit status is 0 if OK, 1 if minor problems, 2 if serious trouble.\n\
\n\
Report bugs to <%3%>.")
    % program_short_name
    % DEFAULT_OUTPUT
    % program_mailbugs
    << std::endl;
  std::exit(0);
}

/**
 * Show the version of the program.
 */
void version()
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

  std::exit(0);
}


// information from the command line
static std::string input;
static std::string output(DEFAULT_OUTPUT);

/**
 * Parse the command line.
 * @param argc number of parameters.
 * @param argv parameters.
 */
void parse_cmd(int argc, char* argv[])
{
  struct option long_options[] = {
    {"output", required_argument, NULL, 'o'},

    {"version", no_argument, NULL, 'v'},
    {"help", no_argument, NULL, 'h'},

    {NULL, 0, NULL, 0}
  };

  // avoid that getopt prints any message
  opterr = 0;

  while (true) {
    /* getopt_long stores the option index here. */
    int option_index = 0;

    int c = getopt_long(argc, argv, "o:vh", long_options,
                        &option_index);

    /* Detect the end of the options. */
    if (c == -1)
      break;

    switch (c)
    {
    case 'o':
      output = optarg;

      break;

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

  if (argc == optind)
    usage("a object file is needed");
  else if ((optind + 1) < argc)
    usage("too many object files");

  input = argv[optind];
}


int main(int argc, char *argv[])
try {
  parse_cmd(argc, argv);


  // This CPU doesn't need memory because only the instruction set is needed
  cpu::Memory registers;
  FakeCPU cpu(&registers, NULL);
  cpu::Object(cpu.isa(), input).decompile(output);

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
