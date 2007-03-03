/**
 * @file cpu/source.h
 * Simple World Language source file.
 *
 * begin:     Wed, 15 Nov 2006 21:31:26 +0100
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2006, Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __CPU_SOURCE_H__
#define __CPU_SOURCE_H__

#include <vector>
#include <set>
#include <map>
#include <string>

#include <simple/types.h>
#include <cpu/types.h>
#include <cpu/exception.h>
#include <cpu/instruction.h>
#include <cpu/file.h>

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
   * @param what Reason of the error.
   * @param source_line Line where the error was found.
   */
  ParseError(std::string file = "", Uint32 line = 0,
             const std::string& what = "Parse error",
             File::size_type source_line = 0) throw ()
    : runtime_error(what), CPUException(file, line), source_line_(source_line)
  {}

  /**
   * Destructor.
   */
  ~ParseError() throw () {}


  /**
   * Line with the error.
   * @return the line number.
   */
  File::size_type source_line() const throw () { this->source_line_; }

private:
  File::size_type source_line_;
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
         const std::string& filename) throw (FileAccessError);


  /**
   * Compile the source code to object code.
   * @param filename File where to save.
   * @exception FileAccessError problem with file.
   * @exception ParseError error found in the code.
   */
  void compile(std::string filename) throw (FileAccessError, ParseError);


  /**
   * Replace the .include lines with the file contents.
   * @exception FileAccessError file can't be opened.
   * @exception ParseError file included two times.
   */
  void replace_includes() throw (FileAccessError, ParseError);
 
  /**
   * Replace the constants and labels with its value.
   * @exception ParseError error found in the code.
   */
  void replace_constants() throw (ParseError);

protected:
  /**
   * Compile a line.
   * @param line Number of the line.
   * @return the instruction compiled.
   * @exception ParseError error found in the code.
   */
  Word compile(File::size_type line) const throw (ParseError);


  /**
   * Check if a line is a blank one.
   * @param line Number of the line.
   * @return the check result.
   * @exception LineOutOfRange line > lines of the file.
   */
  bool is_blank(File::size_type line) const throw (LineOutOfRange);

  /**
   * Check if a line is a comment.
   * @param line Number of the line.
   * @return the check result.
   * @exception LineOutOfRange line > lines of the file.
   */
  bool is_comment(File::size_type line) const throw (LineOutOfRange);

  /**
   * Check if a line is a constant definition.
   * @param line Number of the line.
   * @return the check result.
   * @exception LineOutOfRange line > lines of the file.
   */
  bool is_constant(File::size_type line) const throw (LineOutOfRange);

  /**
   * Check if a line is a label definition.
   * @param line Number of the line.
   * @return the check result.
   * @exception LineOutOfRange line > lines of the file.
   */
  bool is_label(File::size_type line) const throw (LineOutOfRange);

  /**
   * Check if a line is a include.
   * @param line Number of the line.
   * @return the check result.
   * @exception LineOutOfRange line > lines of the file.
   */
  bool is_include(File::size_type line) const throw (LineOutOfRange);

  /**
   * Check if a line is data (32 bits number).
   * @param line Number of the line.
   * @return the check result.
   * @exception LineOutOfRange line > lines of the file.
   */
  bool is_data(File::size_type line) const throw (LineOutOfRange);

  /**
   * Check if a line is a instruction.
   * @param line Number of the line.
   * @return the check result.
   * @exception LineOutOfRange line > lines of the file.
   */
  bool is_instruction(File::size_type line) const throw (LineOutOfRange);


  /**
   * Return the components of a constant.
   *
   * If the line is not a constant definition a empty vector is returned.
   * The first position is the name and the second is the value.
   * @param line Number of the line.
   * @return the components of a constant.
   * @exception LineOutOfRange line > lines of the file.
   */
  std::vector<std::string> get_constant(File::size_type line) const
    throw (LineOutOfRange);

  /**
   * Return the label name.
   *
   * If the line is not a label definition a empty string is returned.
   * @param line Number of the line.
   * @return the label.
   * @exception LineOutOfRange line > lines of the file.
   */
  std::string get_label(File::size_type line) const throw (LineOutOfRange);

  /**
   * Return the included file.
   *
   * If the line is not a include a empty string is returned.
   * @param line Number of the line.
   * @return the file name.
   * @exception LineOutOfRange line > lines of the file.
   */
  std::string get_include(File::size_type line) const throw (LineOutOfRange);

  /**
   * Return the data.
   *
   * If the line is not data 0 is returned.
   * @param line Number of the line.
   * @return the data.
   * @exception LineOutOfRange line > lines of the file.
   */
  Word get_data(File::size_type line) const throw (LineOutOfRange);

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
  std::vector<std::string> get_instruction(File::size_type line) const
    throw (LineOutOfRange);

private:
  const InstructionSet& set_;
  std::vector<std::string> include_path_;
  std::set<std::string> includes_;
  std::map<std::string, std::string> constants_;
};

}
}

#endif // __CPU_SOURCE_H__
