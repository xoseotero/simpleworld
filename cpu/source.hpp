/**
 * @file cpu/source.hpp
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

#ifndef __CPU_SOURCE_HPP__
#define __CPU_SOURCE_HPP__

#include <vector>
#include <set>
#include <map>
#include <string>

#include <simple/types.hpp>
#include <cpu/types.hpp>
#include <cpu/exception.hpp>
#include <cpu/instruction.hpp>
#include <cpu/file.hpp>

namespace SimpleWorld
{
namespace CPU
{

/**
 * Compiler exception.
 * It's raised if a error is found in the source code.
 */
class ParseError: public std::runtime_error, public CPUException
{
public:
  /**
   * Constructor.
   * @param file File where the exception is raised.
   * @param line Line where the exception is raised.
   * @param source_line Line where the error was found.
   * @param what Reason of the error.
   */
  ParseError(std::string file, Uint32 line,
	     File::size_type source_line,
	     const std::string& what = "Parse error") throw ()
    : runtime_error(what), CPUException(file, line), source_line(source_line)
  {}

  /**
   * Destructor.
   */
  ~ParseError() throw () {}


  File::size_type source_line;	/**< Line with the error. */
};


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
   * Constructor.
   * @param set Instruction set of the CPU
   * @param include_path Paths where to search the files to include.
   * @param filename File to open.
   * @exception FileAccessError File can't be opened
   */
  Source(const InstructionSet& set, std::vector<std::string> include_path,
         const std::string& filename);


  /**
   * Compile the source code to object code.
   * @param filename File where to save.
   * @exception FileAccessError problem with file.
   * @exception ParseError error found in the code.
   */
  void compile(std::string filename);


  /**
   * Replace the .include lines with the file contents.
   * @exception FileAccessError file can't be opened.
   * @exception ParseError file included two times.
   */
  void replace_includes();

  /**
   * Replace the constants and labels with its value.
   * @exception ParseError error found in the code.
   */
  void replace_constants();

  /**
   * Replace the blocks of memory with the zero values.
   * @exception ParseError error found in the code.
   */
  void replace_blocks();

protected:
  /**
   * Compile a line.
   * @param line Number of the line.
   * @return the instruction compiled.
   * @exception ParseError error found in the code.
   */
  Word compile(File::size_type line) const;


  /**
   * Check if a line is a blank one.
   * @param line Number of the line.
   * @return the check result.
   * @exception LineOutOfRange line > lines of the file.
   */
  bool is_blank(File::size_type line) const;

  /**
   * Check if a line is a comment.
   * @param line Number of the line.
   * @return the check result.
   * @exception LineOutOfRange line > lines of the file.
   */
  bool is_comment(File::size_type line) const;

  /**
   * Check if a line is a constant definition.
   * @param line Number of the line.
   * @return the check result.
   * @exception LineOutOfRange line > lines of the file.
   */
  bool is_constant(File::size_type line) const;

  /**
   * Check if a line is a block of memory.
   * @param line Number of the line.
   * @return the check result.
   * @exception LineOutOfRange line > lines of the file.
   */
  bool is_block(File::size_type line) const;

  /**
   * Check if a line is a label definition.
   * @param line Number of the line.
   * @return the check result.
   * @exception LineOutOfRange line > lines of the file.
   */
  bool is_label(File::size_type line) const;

  /**
   * Check if a line is a include.
   * @param line Number of the line.
   * @return the check result.
   * @exception LineOutOfRange line > lines of the file.
   */
  bool is_include(File::size_type line) const;

  /**
   * Check if a line is data (32 bits number).
   * @param line Number of the line.
   * @return the check result.
   * @exception LineOutOfRange line > lines of the file.
   */
  bool is_data(File::size_type line) const;

  /**
   * Check if a line is a instruction.
   * @param line Number of the line.
   * @return the check result.
   * @exception LineOutOfRange line > lines of the file.
   */
  bool is_instruction(File::size_type line) const;


  /**
   * Return the components of a constant.
   *
   * If the line is not a constant definition a empty vector is returned.
   * The first position is the name and the second is the value.
   * @param line Number of the line.
   * @return the components of a constant.
   * @exception LineOutOfRange line > lines of the file.
   */
  std::vector<std::string> get_constant(File::size_type line) const;

  /**
   * Return the value of the block of memory.
   *
   * If the line is not a block of memory zero value is returned.
   * @param line Number of the line.
   * @return the components of a constant.
   * @exception LineOutOfRange line > lines of the file.
   */
  Address get_block(File::size_type line) const;

  /**
   * Return the label name.
   *
   * If the line is not a label definition a empty string is returned.
   * @param line Number of the line.
   * @return the label.
   * @exception LineOutOfRange line > lines of the file.
   */
  std::string get_label(File::size_type line) const;

  /**
   * Return the included file.
   *
   * If the line is not a include a empty string is returned.
   * @param line Number of the line.
   * @return the file name.
   * @exception LineOutOfRange line > lines of the file.
   */
  std::string get_include(File::size_type line) const;

  /**
   * Return the data.
   *
   * If the line is not data 0 is returned.
   * @param line Number of the line.
   * @return the data.
   * @exception LineOutOfRange line > lines of the file.
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
   * @exception LineOutOfRange line > lines of the file.
   */
  std::vector<std::string> get_instruction(File::size_type line) const;

private:
  const InstructionSet& set_;
  std::vector<std::string> include_path_;
  std::set<std::string> includes_;
  std::map<std::string, std::string> constants_;
};

}
}

#endif // __CPU_SOURCE_HPP__
