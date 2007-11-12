/**
 * @file src/swlcpu.cpp
 * Simple World CPU
 *
 * begin:     Tue, 21 Nov 2006 08:40:54 +0100
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
#include <simple/types.hpp>
#include <simple/exception.hpp>
#include <cpu/word.hpp>
#include <cpu/memory.hpp>
#include <cpu/memory_file.hpp>
#include <cpu/instruction.hpp>
#include <cpu/cpu.hpp>
#include <cpu/object.hpp>
namespace sw = SimpleWorld;
namespace cpu = SimpleWorld::CPU;

const char* program_short_name = "swcpu";
const char* program_name = "Simple World CPU";
const char* program_version = "0.1";
const char* program_years = "2006, 2007";
const char* program_author = "Xosé Otero";
const char* program_author_email = "xoseotero@users.sourceforge.net";


class CPU: public cpu::CPU, cpu::Object
{
public:
  /**
   * Constructor.
   * @param filename filename from where to load the code.
   * @exception FileAccessError problem with the file.
   */
  CPU(const std::string& filename) throw ();

  /**
   * Execute the next instruction.
   * @exception CPUStopped A stop instruction was found
   */
  void next() throw (cpu::CPUStopped);

protected:
  cpu::Memory registers_;
  cpu::MemoryFile memory_;
};

CPU::CPU(const std::string& filename) throw ()
  : cpu::CPU(&this->registers_, &this->memory_),
    cpu::Object(cpu::CPU::set_, filename),
    registers_(sizeof(cpu::Word) * 16), memory_(filename)
{
}

void CPU::next() throw (cpu::CPUStopped)
{
  cpu::Instruction instruction = this->fetch_instruction_();
  std::cout
    << this->decompile(cpu::InstructionSet::encode(instruction))
    << std::endl;

  cpu::CPU::next();

  sw::Uint8 i = 1;
  std::vector<sw::Uint8> regs_codes = cpu::CPU::set_.register_codes();
  std::vector<sw::Uint8>::const_iterator reg = regs_codes.begin();
  while (reg != regs_codes.end()) {
    std::cout << boost::str(boost::format("%3s = %8X")
                            % cpu::CPU::set_.register_name(*reg)
                            % cpu::change_byte_order(this->registers_[*reg *
                              4]));
    if (i % 4 == 0)
      std::cout << std::endl;
    else
      std::cout << ", ";

    ++reg;
    i++;
  }

  std::cout << std::endl;
}


int main(int argc, char *argv[])
try {
  std::string input;

  // Declare the supported options.
  po::options_description generic("Generic options");
  generic.add_options()
    ("version,v", "print version string")
    ("help,h", "produce help message")
    ;

  po::options_description config("Configuration");
  config.add_options()
    ("input", po::value<std::string>(&input), "File to execute")
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

  CPU cpu(input);
  cpu.execute();

  std::exit(EXIT_SUCCESS);
}
catch (const sw::Exception& e) {
  std::cout << boost::format("Exception thrown in %1%:%2%")
    % e.file
    % e.line
    << std::endl;
}
catch (const std::exception& e) {
  std::cout << boost::format("Exception thrown: %1%") % e.what() << std::endl;
}
catch (...) {
  std::cout << "Unknown exception thrown" << std::endl;
}
