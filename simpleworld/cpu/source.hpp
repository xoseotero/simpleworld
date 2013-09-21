/**
 * @file simpleworld/cpu/source.hpp
 * Simple World Language source file.
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

#ifndef SIMPLEWORLD_CPU_SOURCE_HPP
#define SIMPLEWORLD_CPU_SOURCE_HPP

#include <vector>
#include <set>
#include <map>
#include <string>
#include <utility>

#include <simpleworld/cpu/types.hpp>
#include <simpleworld/cpu/isa.hpp>
#include <simpleworld/cpu/file.hpp>
#include <simpleworld/cpu/memory.hpp>

namespace simpleworld
{
namespace cpu
{

/**
 * Simple World Language source file.
 *
 * The source code can be compiled to object code.
 * Labels and defines are the same, so a label and a define can't have
 * the same name.
 * A file only can be included 1 time.
 */
class Source: public File
{
public:
  /**
   * Macro parameters.
   */
  struct Macro {
    std::vector<std::string> params;    /**< Parameters of the macro */
    std::vector<std::string> code;      /**< Body of the macro  */
  };


  /**
   * Constructor for a empty file.
   * @param isa Instruction set architecture of the CPU
   */
  Source(const ISA& isa);

  /**
   * Constructor.
   * @param isa Instruction set architecture of the CPU
   * @param file File to copy.
   */
  Source(const ISA& isa, const File& file);

  /**
   * Constructor.
   * @param isa Instruction set architecture of the CPU
   * @param filename File to open.
   * @exception IOError if file can't be opened
   */
  Source(const ISA& isa, const std::string& filename);


  /**
   * Clear the content of the source code and reset the internal state.
   */
  void clear();


  /**
   * Load from a file.
   * Before the load, all the lines of the File are removed.
   * @param filename File to open.
   */
  void load(const File& file);

  /**
   * Load from a file.
   * Before the load, all the lines of the File are removed.
   * @param filename File to open.
   * @exception IOError File can't be opened
   */
  void load(std::string filename);


  /**
   * Add a include path.
   * @param path Path to add.
   * @exception CPUException Path already added.
   */
  void add_include_path(std::string path);

  /**
   * Add a define.
   * @param name Name of the define.
   * @param value Value of the define.
   * @exception CPUException name duplicated.
   */
  void add_define(std::string name, std::string value);

  /**
   * Add a macro.
   * @param name Name of the macro.
   * @param value Value of the macro.
   * @exception CPUException name duplicated.
   */
  void add_macro(std::string name, Macro value);


  /**
   * Preprocess the source code.
   * @param strip if the comments and blank lines can be deleted.
   * @exception IOError if a file can't be found.
   */
  void preprocess(bool strip = false);


  /**
   * Compile the source code to object code.
   * @param mem Memory where to save.
   * @return the warning messages generated during the compilation.
   * @exception ParserError error found in the code.
   * @exception ErrorDirective error directive found in the code.
   */
  std::vector<std::string> compile(Memory* mem);

  /**
   * Compile the source code to object code.
   * @param filename File where to save.
   * @return the warning messages generated during the compilation.
   * @exception IOERROR if a problem with file happen.
   * @exception ParserError error found in the code.
   * @exception ErrorDirective error directive found in the code.
   */
  std::vector<std::string> compile(std::string filename);


protected:
  /**
   * Replace the .include lines with the file contents.
   * @exception IOError if a file can't be found.
   */
  void replace_includes();

  /**
   * Replace the macros with its value.
   * @exception ParserError error found in the code.
   */
  void replace_macros();

  /**
   * Replace the defines (and ifdef/ifndefs blocks) with its value.
   * @exception ParserError error found in the code.
   */
  void replace_defines();

  /**
   * Replace the blocks of memory with the zero values.
   * @exception ParserError error found in the code.
   */
  void replace_blocks();

  /**
   * Replace the labels with its value.
   * @exception ParserError error found in the code.
   */
  void replace_labels();


  /**
   * Delete comments and blank lines.
   */
  void strip();


  /**
   * Compile a line.
   * @param line Number of the line.
   * @return the instruction compiled.
   * @exception ParserError error found in the code.
   */
  Word compile(File::size_type line) const;


private:
  const ISA& isa_;
  std::vector<std::string> include_path_;
  std::set<std::string> includes_;
  std::map<std::string, Macro> macros_;
  std::map<std::string, std::string> defines_;
  std::map<std::string, Address> labels_;
};

}
}

#endif // SIMPLEWORLD_CPU_SOURCE_HPP
