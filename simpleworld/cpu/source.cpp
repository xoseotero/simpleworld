/**
 * @file simpleworld/cpu/source.cpp
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

#include <cstdlib>

#include <boost/format.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/regex.hpp>
namespace fs = boost::filesystem;

#include <simpleworld/ioerror.hpp>

#include "word.hpp"
#include "instruction.hpp"
#include "source.hpp"
#include "exception.hpp"
#include "parsererror.hpp"

namespace SimpleWorld
{
namespace CPU
{

// Regular expressions for the code
#define BEGIN_LINE "^"
#define END_LINE "$"
#define BEGIN_WORD "\\<"
#define END_WORD "\\>"
#define SPACE "[[:blank:]]+"
#define OPTIONAL_SPACE "[[:blank:]]*"
#define ANYTHING ".*"
#define KEYWORD "[[:word:]]+"
#define NUMBER16 "0x[[:xdigit:]]{1,4}"
#define NUMBER32 "0x[[:xdigit:]]{1,8}"
#define COMMENT "#.*"
#define OPTIONAL_COMMENT "(?:#.*)?"

// A blank line
static const boost::regex re_blank(BEGIN_LINE
                                   OPTIONAL_SPACE
                                   END_LINE);
// A line with only a comment
static const boost::regex re_comment(BEGIN_LINE
                                     OPTIONAL_SPACE
                                     "(" COMMENT ")"
                                     END_LINE);
// A line with a include
static const boost::regex re_include(BEGIN_LINE
                                     OPTIONAL_SPACE
                                     "\\.include"
                                     SPACE
                                     "\""
                                     "(" ANYTHING ")"
                                     "\""
                                     OPTIONAL_SPACE
                                     OPTIONAL_COMMENT
                                     END_LINE);
// A line with a define
static const boost::regex re_define(BEGIN_LINE
                                    OPTIONAL_SPACE
                                    "\\.define"
                                    SPACE
                                    "(" KEYWORD ")"
                                    SPACE
                                    "(" NUMBER16 ")"
                                    OPTIONAL_SPACE
                                    OPTIONAL_COMMENT
                                    END_LINE);
// A line with a block of memory
static const boost::regex re_block(BEGIN_LINE
                                   OPTIONAL_SPACE
                                   "\\.block"
                                   SPACE
                                   "(" NUMBER32 ")"
                                   OPTIONAL_SPACE
                                   OPTIONAL_COMMENT
                                   END_LINE);
// A line with a label definition
static const boost::regex re_label(BEGIN_LINE
                                   OPTIONAL_SPACE
                                   "\\.label"
                                   SPACE
                                   "(" KEYWORD ")"
                                   OPTIONAL_SPACE
                                   OPTIONAL_COMMENT
                                   END_LINE);
// A line with a label used as data
static const boost::regex re_label_as_data(BEGIN_LINE
                                           OPTIONAL_SPACE
                                           "(" KEYWORD ")"
                                           OPTIONAL_SPACE
                                           OPTIONAL_COMMENT
                                           END_LINE);
// Data
static const boost::regex re_data(BEGIN_LINE
                                  OPTIONAL_SPACE
                                  "(" NUMBER32 ")"
                                  OPTIONAL_SPACE
                                  OPTIONAL_COMMENT
                                  END_LINE);
// A instruction with 0 registers
static const boost::regex re_inst0(BEGIN_LINE
                                   OPTIONAL_SPACE
                                   "(" KEYWORD ")"
                                   OPTIONAL_SPACE
                                   OPTIONAL_COMMENT
                                   END_LINE);
// A instruction with 0 registers + data
static const boost::regex re_inst0i(BEGIN_LINE
                                    OPTIONAL_SPACE
                                    "(" KEYWORD ")"
                                    SPACE
                                    "(" NUMBER16 ")"
                                    OPTIONAL_SPACE
                                    OPTIONAL_COMMENT
                                    END_LINE);
// A instruction with 1 register
static const boost::regex re_inst1(BEGIN_LINE
                                   OPTIONAL_SPACE
                                   "(" KEYWORD ")"
                                   SPACE
                                   "(" KEYWORD ")"
                                   OPTIONAL_SPACE
                                   OPTIONAL_COMMENT
                                   END_LINE);
// A instruction with 1 register + data
static const boost::regex re_inst1i(BEGIN_LINE
                                    OPTIONAL_SPACE
                                    "(" KEYWORD ")"
                                    SPACE
                                    "(" KEYWORD ")"
                                    SPACE
                                    "(" NUMBER16 ")"
                                    OPTIONAL_SPACE
                                    OPTIONAL_COMMENT
                                    END_LINE);
// A instruction with 2 registers
static const boost::regex re_inst2(BEGIN_LINE
                                   OPTIONAL_SPACE
                                   "(" KEYWORD ")"
                                   SPACE
                                   "(" KEYWORD ")"
                                   SPACE
                                   "(" KEYWORD ")"
                                   OPTIONAL_SPACE
                                   OPTIONAL_COMMENT
                                   END_LINE);
// A instruction with 2 registers + data
static const boost::regex re_inst2i(BEGIN_LINE
                                    OPTIONAL_SPACE
                                    "(" KEYWORD ")"
                                    SPACE
                                    "(" KEYWORD ")"
                                    SPACE
                                    "(" KEYWORD ")"
                                    SPACE
                                    "(" NUMBER16 ")"
                                    OPTIONAL_SPACE
                                    OPTIONAL_COMMENT
                                    END_LINE);
// A instruction with 3 registers
static const boost::regex re_inst3(BEGIN_LINE
                                   OPTIONAL_SPACE
                                   "(" KEYWORD ")"
                                   SPACE
                                   "(" KEYWORD ")"
                                   SPACE
                                   "(" KEYWORD ")"
                                   SPACE
                                   "(" KEYWORD ")"
                                   OPTIONAL_SPACE
                                   OPTIONAL_COMMENT
                                   END_LINE);


/**
 * Find a file in the directories.
 * @param path Directories where to find the file.
 * @param filename Name of the file.
 * @return the path to the file found.
 */
static fs::path find_file(const std::vector<std::string>& path,
                          const std::string& filename)
{
  std::vector<std::string>::const_iterator iter = path.begin();
  while (iter != path.end()) {
    fs::path path(*iter, fs::native);
    path /= filename;
    if (fs::exists(path))
      return path;
    ++iter;
  }

  return fs::path();
}


/**
 * Constructor for a empty file.
 * @param isa Instruction set architecture of the CPU
 * @param include_path Paths where to search the files to include.
 */
Source::Source(const ISA& isa,
               const std::vector<std::string>& include_path)
  : File(), isa_(isa), include_path_(include_path)
{
}

/**
 * Constructor.
 * @param isa Instruction set architecture of the CPU
 * @param include_path Paths where to search the files to include.
 * @param filename File to open.
 * @exception IOError if file can't be opened
 */
Source::Source(const ISA& isa,
               const std::vector<std::string>& include_path,
               const std::string& filename)
  : File(filename), isa_(isa), include_path_(include_path)
{
  this->load(filename);
}


/**
 * Load from a file.
 * Before the load, all the lines of the File are removed.
 * @param filename File to open.
 * @exception IOError File can't be opened
 */
void Source::load(std::string filename)
{
  File::load(filename);

  // Clear the includes in a reload
  this->includes_.clear();

  // The main file can't be included
  std::string abs_path(fs::complete(fs::path(filename)).normalize().string());
  this->includes_.insert(abs_path); // Using the absolute path
}


/**
 * Preprocess the source code.
 * @exception IOError if a file can't be found.
 * @exception ParserError file included two times.
 */
void Source::preprocess()
{
  this->replace_includes();
  this->replace_blocks();
  this->replace_defines();
  this->replace_labels();
}


/**
 * Compile the source code to object code.
 * @param filename File where to save.
 * @exception IOERROR if a problem with file happen.
 * @exception ParserError error found in the code.
 */
void Source::compile(std::string filename)
{
  this->preprocess();

  std::ofstream file(filename.c_str(), std::ios::binary | std::ios::trunc);
  if (file.rdstate() & std::ofstream::failbit)
    throw EXCEPTION(IOError, boost::str(boost::format("\
File %1% is not writable")
                                        % filename));

  File::size_type i;
  for (i = 0; i < this->lines(); i++) {
    if (this->is_blank(i) or this->is_comment(i))
      continue;

    Word code = this->compile(i);
    file.write(reinterpret_cast<char*>(&code), sizeof(Word));
    if (file.fail())
      throw EXCEPTION(IOError, boost::str(boost::format("\
Can't write in file %1%")
                                           % filename));
  }

  file.close();
}


/**
 * Replace the .include lines with the file contents.
 * @exception IOError if a file can't be found.
 * @exception ParserError file included two times.
 */
void Source::replace_includes()
{
  File::size_type i;
  for (i = 0; i < this->lines(); i++)
    if (this->is_include(i)) {
      fs::path filename(find_file(this->include_path_, this->get_include(i)));
      if (filename.empty())
        throw EXCEPTION(IOError, boost::str(boost::format("\
File %1% not found")
                                            % this->get_include(i)));

      std::string
        abs_path(fs::complete(filename).normalize().string());
      if (this->includes_.find(abs_path) != this->includes_.end())
        throw EXCEPTION(ParserError, boost::str(boost::format("\
Line: %1%\n\
File %2% already included")
                                                % this->get_line(i)
                                                % abs_path));
      this->remove(i, 1);
      this->insert(i, File(abs_path));
      this->includes_.insert(abs_path);
    }
}

/**
 * Replace the defines with its value.
 * @exception ParserError error found in the code.
 */
void Source::replace_defines()
{
  // Search defines
  File::size_type i = 0;
  while (i < this->lines())
    if (this->is_define(i)) {
      std::vector<std::string> define(this->get_define(i));
      if (this->defines_.find(define[0]) != this->defines_.end())
        throw EXCEPTION(ParserError, boost::str(boost::format("\
Line: %1%\n\
Constant %2% already defined")
                                                % this->get_line(i)
                                                % define[0]));

      this->remove(i, 1);
      this->defines_.insert(std::pair<std::string,
                            std::string>(define[0], define[1]));
    } else
      i++;

  // Replace defines
  for (i = 0; i < this->lines(); i++)
    // Don't replace defines in comments
    // TODO: the defines is replaced if the it is in a inline comment
    if (not this->is_comment(i)) {
      std::map<std::string, std::string>::const_iterator iter =
        this->defines_.begin();
      while (iter != this->defines_.end()) {
        this->get_line(i) =
          boost::regex_replace(this->get_line(i),
                               boost::regex(std::string(BEGIN_WORD) +
                                            (*iter).first +
                                            std::string(END_WORD)),
                               (*iter).second);

        ++iter;
      }
    }
}

/**
 * Replace the labels with its value.
 * @exception ParserError error found in the code.
 */
void Source::replace_labels()
{
  // Search labels
  File::size_type i = 0;
  File::size_type lines_code = 0;
  while (i < this->lines())
    if (this->is_label(i)) {
      std::string label(this->get_label(i));
      if (this->labels_.find(label) != this->labels_.end())
        throw EXCEPTION(ParserError, boost::str(boost::format("\
Line: %1%\n\
Label %1% already defined")
                                                % this->get_line(i)
                                                % label));
      this->remove(i, 1);
      this->labels_.insert(std::pair<std::string, Address>(label,
        lines_code * sizeof(Word)));
    } else if (this->is_instruction(i) or this->is_data(i)) {
      lines_code++;
      i++;
    } else
      i++;

  // Replace labels
  for (i = 0, lines_code = 0; i < this->lines(); i++)
    // is_label_as_data() must be checked first because a label could be
    // confused with a instruction without arguments
    if (this->is_label_as_data(i)) {
      // If a label is used as data, it's replaced by the address
      std::map<std::string, Address>::const_iterator iter =
        this->labels_.begin();
      while (iter != this->labels_.end()) {
        std::string address(boost::str(boost::format("0x%x") %
                                       (*iter).second));
        this->get_line(i) =
          boost::regex_replace(this->get_line(i),
                               boost::regex(std::string(BEGIN_WORD) +
                                            (*iter).first +
                                            std::string(END_WORD)),
                               address);

        ++iter;
      }

      lines_code++;
    } else if (this->is_instruction(i)) {
      // If a label is used in a instruction, it's replaced by the offset to pc
      std::map<std::string, Address>::const_iterator iter =
        this->labels_.begin();
      while (iter != this->labels_.end()) {
        std::string offset(boost::str(boost::format("0x%x") %
                                      ((*iter).second -
                                       lines_code * sizeof(Word))));
        this->get_line(i) =
          boost::regex_replace(this->get_line(i),
                               boost::regex(std::string(BEGIN_WORD) +
                                            (*iter).first +
                                            std::string(END_WORD)),
                               offset);

        ++iter;
      }

      lines_code++;
    } else if(this->is_data(i)) {
      lines_code++;
    }
}

/**
 * Replace the blocks of memory with the zero values.
 * @exception ParserError error found in the code.
 */
void Source::replace_blocks()
{
  File::size_type i;
  for (i = 0; i < this->lines(); i++)
    if (this->is_block(i)) {
      Address size = this->get_block(i);
      if (size > 0) {
        // Round to the next multiple of 4
        size = ((size - 1) / sizeof(Word) + 1);

        this->remove(i, 1);
        while (size-- > 0)
          this->insert(i, "0x00000000");
      }
    }
}


/**
 * Compile a line.
 * @param line Number of the line.
 * @return the instruction compiled.
 * @exception ParserError error found in the code.
 */
Word Source::compile(File::size_type line) const
{
  if (this->is_data(line)) {
#ifdef IS_BIG_ENDIAN
    return this->get_data(line);
#else
    return change_byte_order(this->get_data(line));
#endif
  }

  // Is a instruction
  std::vector<std::string> keywords(this->get_instruction(line));

  InstructionInfo info;
  try {
    info =
      this->isa_.instruction_info(this->isa_.instruction_code(keywords[0]));
  }
  catch (const CPUException& e) {
    throw EXCEPTION(ParserError, boost::str(boost::format("\
Line: %1%\n\
%2%")
                                            % this->get_line(line)
                                            % e.what));
  }

  if ((info.nregs + info.has_inmediate + 1) != keywords.size())
    throw EXCEPTION(ParserError, boost::str(boost::format("\
Line: %1%\n\
Wrong number of parameters (%2%)")
                                            % this->get_line(line)
                                            % (keywords.size() - 1)));

  Instruction inst;
  try {
    inst.code = info.code;
    if (info.nregs >= 1)
      inst.first = this->isa_.register_code(keywords[1]);
    if (info.nregs >= 2)
      inst.second = this->isa_.register_code(keywords[2]);
    if (info.nregs == 3)
      inst.data = this->isa_.register_code(keywords[3]);
    else if (info.has_inmediate) {
      const char* str = keywords[info.nregs + 1].c_str();
      char* ptr;
      inst.data = std::strtoul(str, &ptr, 16);
    }
  } catch (const CPUException& e) {
    throw EXCEPTION(ParserError, boost::str(boost::format("\
Line: %1%\n\
%2%")
                                            % this->get_line(line)
                                            % e.what));
  }

  return inst.encode();
}


/**
 * Check if a line is a blank one.
 * @param line Number of the line.
 * @return the check result.
 * @exception CPUException if line > lines of the file.
 */
bool Source::is_blank(File::size_type line) const
{
  return boost::regex_match(this->get_line(line), re_blank);
}


/**
 * Check if a line is a comment.
 * @param line Number of the line.
 * @return the check result.
 * @exception CPUException if line > lines of the file.
 */
bool Source::is_comment(File::size_type line) const
{
  return boost::regex_match(this->get_line(line), re_comment);
}

/**
 * Check if a line is a define.
 * @param line Number of the line.
 * @return the check result.
 * @exception CPUException if line > lines of the file.
 */
bool Source::is_define(File::size_type line) const
{
  return boost::regex_match(this->get_line(line), re_define);
}

/**
 * Check if a line is a block of memory.
 * @param line Number of the line.
 * @return the check result.
 * @exception CPUException if line > lines of the file.
 */
bool Source::is_block(File::size_type line) const
{
  return boost::regex_match(this->get_line(line), re_block);
}

/**
 * Check if a line is a label definition.
 * @param line Number of the line.
 * @return the check result.
 * @exception CPUException if line > lines of the file.
 */
bool Source::is_label(File::size_type line) const
{
  return boost::regex_match(this->get_line(line), re_label);
}

/**
 * Check if a line is a label used as data.
 * @param line Number of the line.
 * @return the check result.
 * @exception CPUException if line > lines of the file.
 */
bool Source::is_label_as_data(File::size_type line) const
{
  std::string result;

  boost::smatch what;
  if (boost::regex_match(this->get_line(line), what, re_label_as_data))
    result = what[1];

  return this->labels_.find(result) != this->labels_.end();
}

/**
 * Check if a line is a include.
 * @param line Number of the line.
 * @return the check result.
 * @exception CPUException if line > lines of the file.
 */
bool Source::is_include(File::size_type line) const
{
  return boost::regex_match(this->get_line(line), re_include);
}

/**
 * Check if a line is data (32 bits number).
 * @param line Number of the line.
 * @return the check result.
 * @exception CPUException if line > lines of the file.
 */
bool Source::is_data(File::size_type line) const
{
  return boost::regex_match(this->get_line(line), re_data);
}

/**
 * Check if a line is a instruction.
 * @param line Number of the line.
 * @return the check result.
 * @exception CPUException if line > lines of the file.
 */
bool Source::is_instruction(File::size_type line) const
{
  return boost::regex_match(this->get_line(line), re_inst0) |
    boost::regex_match(this->get_line(line), re_inst0i) |
    boost::regex_match(this->get_line(line), re_inst1) |
    boost::regex_match(this->get_line(line), re_inst1i) |
    boost::regex_match(this->get_line(line), re_inst2) |
    boost::regex_match(this->get_line(line), re_inst2i) |
    boost::regex_match(this->get_line(line), re_inst3);
}


/**
 * Return the components of a define.
 *
 * If the line is not a define a empty vector is returned.
 * The first position is the name and the second is the value.
 * @param line Number of the line.
 * @return the components of a define.
 * @exception CPUException if line > lines of the file.
 */
std::vector<std::string> Source::get_define(File::size_type line) const
{
  std::vector<std::string> result;

  boost::smatch what;
  if (boost::regex_match(this->get_line(line), what, re_define))
    result.insert(result.begin(), what.begin() + 1, what.end());

  return result;
}

/**
 * Return the value of the block of memory.
 *
 * If the line is not a block of memory zero value is returned.
 * @param line Number of the line.
 * @return the components of a define.
 * @exception CPUException if line > lines of the file.
 */
Address Source::get_block(File::size_type line) const
{
  Address result = 0;

  boost::smatch what;
  if (boost::regex_match(this->get_line(line), what, re_block))
    sscanf(std::string(what[1]).c_str(), "%x", &result);

  return result;
}

/**
 * Return the label name.
 *
 * If the line is not a label definition a empty string is returned.
 * @param line Number of the line.
 * @return the label.
 * @exception CPUException if line > lines of the file.
 */
std::string Source::get_label(File::size_type line) const
{
  std::string result;

  boost::smatch what;
  if (boost::regex_match(this->get_line(line), what, re_label))
    result = what[1];

  return result;
}

/**
 * Return the included file.
 *
 * If the line is not a include a empty string is returned.
 * @param line Number of the line.
 * @return the file name.
 * @exception CPUException if line > lines of the file.
 */
std::string Source::get_include(File::size_type line) const
{
  std::string result;

  boost::smatch what;
  if (boost::regex_match(this->get_line(line), what, re_include))
    result = what[1];

  return result;
}

/**
 * Return the data.
 *
 * If the line is not data 0 is returned.
 * @param line Number of the line.
 * @return the data.
 * @exception CPUException if line > lines of the file.
 */
Word Source::get_data(File::size_type line) const
{
  Word result = 0;

  boost::smatch what;
  if (boost::regex_match(this->get_line(line), what, re_data)) {
    const char* str = std::string(what[1]).c_str();
    char* ptr;
    result = std::strtoul(str, &ptr, 16);
  }

  return result;
}

/**
 * Return the components of a instruction.
 *
 * If the line is not a instruction a empty vector is returned.
 * A instruction can have 0-3 operators.
 * The firs position is the instruction and the next ones are the operators.
 * @param line Number of the line.
 * @return the components of a define.
 * @exception CPUException if line > lines of the file.
 */
std::vector<std::string> Source::get_instruction(File::size_type line) const
{
  std::vector<std::string> result;

  boost::smatch what;
  if (boost::regex_match(this->get_line(line), what, re_inst0)) {
    result.insert(result.begin(), what.begin() + 1, what.end());
    return result;
  } else if (boost::regex_match(this->get_line(line), what, re_inst0i)) {
    result.insert(result.begin(), what.begin() + 1, what.end());
    return result;
  } else if (boost::regex_match(this->get_line(line), what, re_inst1)) {
    result.insert(result.begin(), what.begin() + 1, what.end());
    return result;
  } else if (boost::regex_match(this->get_line(line), what, re_inst1i)) {
    result.insert(result.begin(), what.begin() + 1, what.end());
    return result;
  } else if (boost::regex_match(this->get_line(line), what, re_inst2)) {
    result.insert(result.begin(), what.begin() + 1, what.end());
    return result;
  } else if (boost::regex_match(this->get_line(line), what, re_inst2i)) {
    result.insert(result.begin(), what.begin() + 1, what.end());
    return result;
  } else if (boost::regex_match(this->get_line(line), what, re_inst3)) {
    result.insert(result.begin(), what.begin() + 1, what.end());
    return result;
  }

  return result;
}

}
}
