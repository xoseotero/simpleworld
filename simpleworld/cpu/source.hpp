/**
 * @file simpleworld/cpu/source.hpp
 * Simple World Language source file.
 *
 *  Copyright (C) 2006-2012  Xosé Otero <xoseotero@gmail.com>
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
  struct Macro {
    std::vector<std::string> params;
    std::vector<std::string> code;
  };

public:
  /**
   * Constructor for a empty file.
   * @param isa Instruction set architecture of the CPU
   * @param include_path Paths where to search the files to include.
   * @param definitions Definitions.
   */
  Source(const ISA& isa,
         const std::vector<std::string>& include_path,
         const std::map<std::string, std::string>& definitions);

  /**
   * Constructor.
   * @param isa Instruction set architecture of the CPU
   * @param include_path Paths where to search the files to include.
   * @param definitions Definitions.
   * @param filename File to open.
   * @exception IOError if file can't be opened
   */
  Source(const ISA& isa,
         const std::vector<std::string>& include_path,
         const std::map<std::string, std::string>& definitions,
         const std::string& filename);


  /**
   * Load from a file.
   * Before the load, all the lines of the File are removed.
   * @param filename File to open.
   * @exception IOError File can't be opened
   */
  void load(std::string filename);


  /**
   * Preprocess the source code.
   * @param strip if the comments and blank lines can be deleted.
   * @exception IOError if a file can't be found.
   */
  void preprocess(bool strip = false);


  /**
   * Compile the source code to object code.
   * @param filename File where to save.
   * @exception IOERROR if a problem with file happen.
   * @exception ParserError error found in the code.
   */
  void compile(std::string filename);


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


  /**
   * Check if a line is a blank one.
   * @param line Number of the line.
   * @return the check result.
   * @exception CPUException if line > lines of the file.
   */
  bool is_blank(File::size_type line) const;

  /**
   * Check if a line is a comment.
   * @param line Number of the line.
   * @return the check result.
   * @exception CPUException if line > lines of the file.
   */
  bool is_comment(File::size_type line) const;

  /**
   * Check if a line is a include.
   * @param line Number of the line.
   * @return the check result.
   * @exception CPUException if line > lines of the file.
   */
  bool is_include(File::size_type line) const;

  /**
   * Check if a line is the begining of a macro.
   * @param line Number of the line.
   * @return the check result.
   * @exception CPUException if line > lines of the file.
   */
  bool is_macro(File::size_type line) const;

  /**
   * Check if a line is the end of a macro.
   * @param line Number of the line.
   * @return the check result.
   * @exception CPUException if line > lines of the file.
   */
  bool is_endmacro(File::size_type line) const;

  /**
   * Check if a line is a define.
   * @param line Number of the line.
   * @return the check result.
   * @exception CPUException if line > lines of the file.
   */
  bool is_define(File::size_type line) const;

  /**
   * Check if a line is the begining of a ifdef block.
   * @param line Number of the line.
   * @return the check result.
   * @exception CPUException if line > lines of the file.
   */
  bool is_ifdef(File::size_type line) const;

  /**
   * Check if a line is the begining of a ifndef.
   * @param line Number of the line.
   * @return the check result.
   * @exception CPUException if line > lines of the file.
   */
  bool is_ifndef(File::size_type line) const;

  /**
   * Check if a line is the end of a if.
   * @param line Number of the line.
   * @return the check result.
   * @exception CPUException if line > lines of the file.
   */
  bool is_endif(File::size_type line) const;

  /**
   * Check if a line is a block of memory.
   * @param line Number of the line.
   * @return the check result.
   * @exception CPUException if line > lines of the file.
   */
  bool is_block(File::size_type line) const;

  /**
   * Check if a line is a label definition.
   * @param line Number of the line.
   * @return the check result.
   * @exception CPUException if line > lines of the file.
   */
  bool is_label(File::size_type line) const;

  /**
   * Check if a line is a label used as data.
   * @param line Number of the line.
   * @return the check result.
   * @exception CPUException if line > lines of the file.
   */
  bool is_label_as_data(File::size_type line) const;

  /**
   * Check if a line is data (32 bits number).
   * @param line Number of the line.
   * @return the check result.
   * @exception CPUException if line > lines of the file.
   */
  bool is_data(File::size_type line) const;

  /**
   * Check if a line is a instruction.
   * @param line Number of the line.
   * @return the check result.
   * @exception CPUException if line > lines of the file.
   */
  bool is_instruction(File::size_type line) const;


  /**
   * Return all the keywords of a line.
   *
   * If the line has not any keyword a empty vector is returned.
   * @param line Number of the line.
   * @return the components of a define.
   * @exception CPUException if line > lines of the file.
   */
  std::vector<std::string> get_keywords(File::size_type line) const;

  /**
   * Return the included file.
   *
   * If the line is not a include a empty string is returned.
   * @param line Number of the line.
   * @return the file name.
   * @exception CPUException if line > lines of the file.
   */
  std::string get_include(File::size_type line) const;

  /**
   * Return the components of a macro.
   *
   * If the line is not the begining of a macro a empty vector is returned.
   * @param line Number of the line.
   * @return the components of a macro.
   * @exception CPUException if line > lines of the file.
   */
  std::pair<std::string, Macro> get_macro(File::size_type line) const;

  /**
   * Return the components of a define.
   *
   * If the line is not a define a empty vector is returned.
   * The first position is the name and the second is the value.
   * @param line Number of the line.
   * @return the components of a define.
   * @exception CPUException if line > lines of the file.
   */
  std::vector<std::string> get_define(File::size_type line) const;

  /**
   * Return the identificator of a ifdef.
   *
   * If the line is not a define a empty vector is returned.
   * The first position is the name and the second is the value.
   * @param line Number of the line.
   * @return the identificator.
   * @exception CPUException if line > lines of the file.
   */
  std::string get_ifdef(File::size_type line) const;

  /**
   * Return the identificator of a ifndef.
   *
   * If the line is not a define a empty vector is returned.
   * The first position is the name and the second is the value.
   * @param line Number of the line.
   * @return the identificator.
   * @exception CPUException if line > lines of the file.
   */
  std::string get_ifndef(File::size_type line) const;

  /**
   * Return the value of the block of memory.
   *
   * If the line is not a block of memory zero value is returned.
   * @param line Number of the line.
   * @return the components of a define.
   * @exception CPUException if line > lines of the file.
   */
  Address get_block(File::size_type line) const;

  /**
   * Return the label name.
   *
   * If the line is not a label definition a empty string is returned.
   * @param line Number of the line.
   * @return the label.
   * @exception CPUException if line > lines of the file.
   */
  std::string get_label(File::size_type line) const;

  /**
   * Return the data.
   *
   * If the line is not data 0 is returned.
   * @param line Number of the line.
   * @return the data.
   * @exception CPUException if line > lines of the file.
   */
  Word get_data(File::size_type line) const;

  /**
   * Return the components of a instruction.
   *
   * If the line is not a instruction a empty vector is returned.
   * A instruction can have 0-3 operators.
   * The firs position is the instruction and the next ones are the operators.
   * @param line Number of the line.
   * @return the components of a instruction.
   * @exception CPUException if line > lines of the file.
   */
  std::vector<std::string> get_instruction(File::size_type line) const;

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
