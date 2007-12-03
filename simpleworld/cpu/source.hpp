/**
 * @file simpleworld/cpu/source.hpp
 * Simple World Language source file.
 *
 * begin:     Wed, 15 Nov 2006 21:31:26 +0100
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

#ifndef SIMPLEWORLD_CPU_SOURCE_HPP
#define SIMPLEWORLD_CPU_SOURCE_HPP

#include <vector>
#include <set>
#include <map>
#include <string>

#include <simpleworld/cpu/types.hpp>
#include <simpleworld/cpu/isa.hpp>
#include <simpleworld/cpu/file.hpp>

namespace SimpleWorld
{
namespace CPU
{

/**
 * Simple World Language source file.
 *
 * The source code can be compiled to object code.
 * Labels and constants are the same, so a label and a constant can't have
 * the same name.
 * A file only can be included 1 time.
 */
class Source: public File
{
public:
  /**
   * Constructor for a empty file.
   * @param isa Instruction set architecture of the CPU
   * @param include_path Paths where to search the files to include.
   */
  Source(const ISA& isa,
         const std::vector<std::string>& include_path);

  /**
   * Constructor.
   * @param isa Instruction set architecture of the CPU
   * @param include_path Paths where to search the files to include.
   * @param filename File to open.
   * @exception IOError if file can't be opened
   */
  Source(const ISA& isa,
         const std::vector<std::string>& include_path,
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
   * @exception IOError if a file can't be found.
   * @exception ParserError file included two times.
   */
  void preprocess();


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
   * @exception ParserError file included two times.
   */
  void replace_includes();

  /**
   * Replace the constants and labels with its value.
   * @exception ParserError error found in the code.
   */
  void replace_constants();

  /**
   * Replace the blocks of memory with the zero values.
   * @exception ParserError error found in the code.
   */
  void replace_blocks();


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
   * Check if a line is a constant definition.
   * @param line Number of the line.
   * @return the check result.
   * @exception CPUException if line > lines of the file.
   */
  bool is_constant(File::size_type line) const;

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
   * Check if a line is a include.
   * @param line Number of the line.
   * @return the check result.
   * @exception CPUException if line > lines of the file.
   */
  bool is_include(File::size_type line) const;

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
   * Return the components of a constant.
   *
   * If the line is not a constant definition a empty vector is returned.
   * The first position is the name and the second is the value.
   * @param line Number of the line.
   * @return the components of a constant.
   * @exception CPUException if line > lines of the file.
   */
  std::vector<std::string> get_constant(File::size_type line) const;

  /**
   * Return the value of the block of memory.
   *
   * If the line is not a block of memory zero value is returned.
   * @param line Number of the line.
   * @return the components of a constant.
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
   * Return the included file.
   *
   * If the line is not a include a empty string is returned.
   * @param line Number of the line.
   * @return the file name.
   * @exception CPUException if line > lines of the file.
   */
  std::string get_include(File::size_type line) const;

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
   * @return the components of a constant.
   * @exception CPUException if line > lines of the file.
   */
  std::vector<std::string> get_instruction(File::size_type line) const;

private:
  const ISA& isa_;
  std::vector<std::string> include_path_;
  std::set<std::string> includes_;
  std::map<std::string, std::string> constants_;
};

}
}

#endif // SIMPLEWORLD_CPU_SOURCE_HPP
