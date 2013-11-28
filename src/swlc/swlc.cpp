/**
 * @file src/swlc/swlc.cpp
 * Simple World Language compiler
 *
 *  Copyright (C) 2006-2013  Xosé Otero <xoseotero@gmail.com>
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
#include <vector>
#include <string>
#include <cstdlib>

#include <getopt.h>

#include <boost/format.hpp>
#include <boost/regex.hpp>

#include <simpleworld/config.hpp>
#include <simpleworld/exception.hpp>
#include <simpleworld/ioerror.hpp>
#include <simpleworld/cpu/parsererror.hpp>
#include <simpleworld/cpu/errordirective.hpp>
#include <simpleworld/cpu/memory.hpp>
#include <simpleworld/cpu/source.hpp>
#include <simpleworld/cpu/cpu.hpp>
namespace sw = simpleworld;
namespace cpu = simpleworld::cpu;

#include "../common/info.hpp"
#include "../common/printexc.hpp"
#include "../common/fakeisa.hpp"

#define DEFAULT_OUTPUT "out.swo"
#define DEFAULT_PREPROCCESS_OUTPUT "out.swe"

const char* program_short_name = "swlc";
const char* program_name = "Simple World Language compiler";
const char* program_version = VERSION;
const char* program_years = YEARS;
const char* program_author = AUTHOR;
const char* program_author_email = EMAIL;
const char* program_mailbugs = MAILBUGS;


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
Simple World Language compiler.\n\
\n\
Mandatory arguments to long options are mandatory for short options too.\n\
  -E, --preprocess           preprocess only; do not compile\n\
  -I, --include=PATH         add a directory where to search the\n\
                               included files\n\
  -D, --define=ID            add the definition ID with the value 1\n\
  -D, --define=ID=VALUE      add the definition ID with teh value VALUE\n\
  -o, --output=FILE          place the output into FILE\n\
                               the default file name is \"%2%\"\n\
                               the default preprocess file name is \"%3%\"\n\
\n\
  -h, --help                 display this help and exit\n\
  -v, --version              output version information and exit\n\
\n\
Exit status is 0 if OK, 1 if minor problems, 2 if serious trouble.\n\
\n\
Report bugs to <%4%>.")
    % program_short_name
    % DEFAULT_OUTPUT
    % DEFAULT_PREPROCCESS_OUTPUT
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
static std::vector<std::string> include_path;
static std::map<std::string, std::string> definitions;

// if --preprocess was used
static bool preprocess_set = false;


/**
 * Parse the command line.
 * @param argc number of parameters.
 * @param argv parameters.
 */
void parse_cmd(int argc, char* argv[])
{
  struct option long_options[] = {
    {"preprocess", no_argument, NULL, 'E'},
    {"include", required_argument, NULL, 'I'},
    {"define", required_argument, NULL, 'D'},
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

    int c = getopt_long(argc, argv, "EI:D:o:vh", long_options,
                        &option_index);

    /* Detect the end of the options. */
    if (c == -1)
      break;

    static const boost::regex define_only("^([[:word:]]+)$");
    static const boost::regex define_with_value("^([[:word:]]+)=(.*)$");
    boost::smatch what;
    std::string define_id;
    std::string define_value;

    switch (c)
    {
    case 'E':
      preprocess_set = true;
      output = DEFAULT_PREPROCCESS_OUTPUT;

      break;

    case 'I':
      include_path.push_back(optarg);

      break;

    case 'D':
      if (boost::regex_match(std::string(optarg), what, define_only)) {
        define_id = std::string(what[1].first, what[1].second);
        define_value = "1";
      } else if (boost::regex_match(std::string(optarg), what,
                                    define_with_value)) {
        define_id = std::string(what[1].first, what[1].second);
        define_value = std::string(what[2].first, what[2].second);
      } else
        usage(boost::str(boost::format("wrong format of the definition: `%1%'")
                         % optarg));

      if (definitions.find(define_id) != definitions.end())
        usage(boost::str(boost::format("definition `%1%' already defined")
                         % define_id));

      definitions.insert(std::pair<std::string, std::string>(define_id,
                                                             define_value));

      break;

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
    usage("a source file is needed");
  else if ((optind + 1) < argc)
    usage("too many source files");

  input = argv[optind];
}


int main(int argc, char *argv[])
try {
  parse_cmd(argc, argv);

  // This CPU doesn't need memory because only the instruction set is needed
  cpu::Memory registers;
  cpu::CPU cpu(fakeisa, &registers, NULL);
  cpu::Source source(cpu.isa(), input);
  for (std::vector<std::string>::const_iterator iter = include_path.begin();
       iter != include_path.end();
       ++iter)
    source.add_include_path(*iter);
  for (std::map<std::string, std::string>::const_iterator iter = definitions.begin();
       iter != definitions.end();
       ++iter)
    source.add_define(iter->first, iter->second);

  if (preprocess_set) {
    source.preprocess();
    source.save(output);
  } else {
    std::vector<std::string> warnings = source.compile(output);
    std::vector<std::string>::const_iterator iter;
    for (iter = warnings.begin();
         iter != warnings.end();
         ++iter) {
      std::cout << (*iter) << std::endl;
    }
  }

  std::exit(EXIT_SUCCESS);
}
catch (const cpu::ErrorDirective& e) {
  std::cout << e.info << std::endl;
}
catch (const cpu::ParserError& e) {
  std::cout << e.info << std::endl;
}
catch (const sw::IOError& e) {
  std::cout << e.info << std::endl;
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
