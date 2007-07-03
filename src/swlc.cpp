/**
 * @file src/swlc.cpp
 * Simple World Language compiler
 *
 * begin:     Sat, 18 Nov 2006 12:26:31 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2006-2007  Xosé Otero <xoseotero@users.sourceforge.net>
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
#include <cstdlib>
#include <string>

#include <boost/format.hpp>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <simple/config.hpp>
#include <simple/exception.hpp>
#include <cpu/cpu.hpp>
#include <cpu/source.hpp>
namespace sw = SimpleWorld;
namespace cpu = SimpleWorld::CPU;

const char* program_short_name = "swlc";
const char* program_name = "Simple World Language compiler";
const char* program_version = "0.1";
const char* program_years = "2006";
const char* program_author = "Xosé Otero";
const char* program_author_email = "xoseotero@users.sourceforge.net";

int main(int argc, char *argv[])
try {
  std::string input;
  std::string output;
  std::vector<std::string> include_path;

  // Declare the supported options.
  po::options_description generic("Generic options");
  generic.add_options()
    ("version,v", "print version string")
    ("help,h", "produce help message")
    ;

  po::options_description config("Configuration");
  config.add_options()
    ("include,I", po::value< std::vector<std::string> >(&include_path),
     "Add a directory where to search the included files")
    ("preprocess,E", "Preprocess only; do not compile")
    ("input", po::value<std::string>(&input), "File to compile")
    ("output,o", po::value<std::string>(&output)->default_value("out.swo"),
     "Place the output into <file>")
    ;

  po::options_description cmdline_options;
  cmdline_options.add(generic).add(config);

  // Positional arguments will be passed as "input"
  po::positional_options_description p;
  p.add("input", -1);

  // Take arguments
  po::variables_map vm;
  po::store(po::command_line_parser(argc,
    argv).options(cmdline_options).positional(p).run(), vm);
  po::notify(vm);


  if (vm.count("version")) {
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
    return 1;
  }

  // Show help if the --help is passed and if there isn't a input file
  if (vm.count("help") or input.empty()) {
    std::cout << cmdline_options << std::endl;
    return 1;
  }

  // This CPU doesn't need memory because only the instruction set is needed
  cpu::CPU cpu(NULL);
  cpu::Source source(cpu.instruction_set(), include_path, input);
  if (vm.count("preprocess")) {
    source.replace_includes();
    source.replace_constants();
    source.replace_blocks();
    source.save(output);
  }else
    source.compile(output);

  std::exit(EXIT_SUCCESS);
}
catch (cpu::ParseError e) {
  std::cout << boost::format("(%1%:%2%) %3%")
    % e.file()
    % e.line()
    % e.what()
    << std::endl;
}
catch (sw::Exception e) {
  std::cout << boost::format("Exception thrown in %1%:%2%")
    % e.file()
    % e.line()
    << std::endl;
}
catch (std::exception e) {
  std::cout << boost::format("Exception thrown: %1%") % e.what() << std::endl;
}
catch (...) {
  std::cout << "Unknown exception thrown" << std::endl;
}
