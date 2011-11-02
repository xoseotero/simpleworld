/**
 * @file simpleworld/cpu/source.cpp
 * Simple World Language source file.
 *
 *  Copyright (C) 2006-2011  Xosé Otero <xoseotero@gmail.com>
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
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
namespace fs = boost::filesystem;

#include <simpleworld/config.hpp>
#include <simpleworld/ints.hpp>
#include <simpleworld/ioerror.hpp>

#include "word.hpp"
#include "instruction.hpp"
#include "source.hpp"
#include "exception.hpp"
#include "parsererror.hpp"

namespace simpleworld
{
namespace cpu
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

// A keyword
static const boost::regex re_keyword("(" KEYWORD ")"
                                     OPTIONAL_SPACE
                                     OPTIONAL_COMMENT);
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
// A line with the begining of a macro block
static const boost::regex re_macro(BEGIN_LINE
                                   OPTIONAL_SPACE
                                   "\\.macro"
                                   SPACE
                                   "(" KEYWORD ")"
                                   OPTIONAL_SPACE
                                   "(" ANYTHING ")"
                                   OPTIONAL_COMMENT
                                   END_LINE);
// A line with the end of a macro block
static const boost::regex re_endmacro(BEGIN_LINE
                                      OPTIONAL_SPACE
                                      "\\.endmacro"
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
                                    "(" ANYTHING ")"
                                    OPTIONAL_SPACE
                                    OPTIONAL_COMMENT
                                    END_LINE);
// A line with the begining of a ifdef block
static const boost::regex re_ifdef(BEGIN_LINE
                                   OPTIONAL_SPACE
                                   "\\.ifdef"
                                   SPACE
                                   "(" KEYWORD ")"
                                   OPTIONAL_SPACE
                                   OPTIONAL_COMMENT
                                   END_LINE);
// A line with the begining of a ifndef block
static const boost::regex re_ifndef(BEGIN_LINE
                                    OPTIONAL_SPACE
                                    "\\.ifndef"
                                    SPACE
                                    "(" KEYWORD ")"
                                    OPTIONAL_SPACE
                                    OPTIONAL_COMMENT
                                    END_LINE);
// A line with the end of a if block
static const boost::regex re_endif(BEGIN_LINE
                                   OPTIONAL_SPACE
                                   "\\.endif"
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
    fs::path path(*iter);
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
  std::string abs_path(fs::absolute(fs::path(filename)).string());
  this->includes_.insert(abs_path); // Using the absolute path
}


/**
 * Preprocess the source code.
 * @param strip if the comments and blank lines can be deleted.
 * @exception IOError if a file can't be found.
 */
void Source::preprocess(bool strip)
{
  this->replace_includes();
  if (strip)
    this->strip();
  this->replace_macros();
  this->replace_defines();
  this->replace_blocks();
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
  this->preprocess(true);

  std::ofstream file(filename.c_str(), std::ios::binary | std::ios::trunc);
  if (file.rdstate() & std::ofstream::failbit)
    throw EXCEPTION(IOError, boost::str(boost::format("\
File %1% is not writable")
                                        % filename));

  for (File::size_type i = 0; i < this->lines(); i++) {
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
 */
void Source::replace_includes()
{
  File::size_type i = 0;
  while (i < this->lines()) {
    if (this->is_include(i)) {
      fs::path filename(find_file(this->include_path_, this->get_include(i)));
      if (filename.empty())
        throw EXCEPTION(IOError, boost::str(boost::format("\
File %1% not found")
                                            % this->get_include(i)));

      this->remove(i, 1);

      // don't include the file more than once
      std::string abs_path(fs::absolute(filename).string());
      if (this->includes_.find(abs_path) != this->includes_.end())
        continue;

      this->insert(i, File(abs_path));
      this->includes_.insert(abs_path);
    }

    i++;
  }
}

/**
 * Replace the macros with its value.
 * @exception ParserError error found in the code.
 */
void Source::replace_macros()
{
  // Search macros
  File::size_type i = 0;
  while (i < this->lines())
    if (this->is_macro(i)) {
      std::pair<std::string, Source::Macro> macro(this->get_macro(i));
      if (this->defines_.find(macro.first) != this->defines_.end())
        throw EXCEPTION(ParserError, boost::str(boost::format("\
Line: %1%\n\
Macro %2% already defined")
                                                % this->get_line(i)
                                                % macro.first));

      this->remove(i, macro.second.code.size() + 2);

      this->macros_.insert(macro);
    } else
      i++;

  // Replace macros
  for (i = 0; i < this->lines(); i++)
    // Don't try to replace macros in blank lines or comments
    if (not this->is_blank(i) and not this->is_comment(i)) {
      std::vector<std::string> keywords(this->get_keywords(i));
      std::map<std::string, Source::Macro>::const_iterator macro =
        this->macros_.find(keywords[0]);
      if (macro != this->macros_.end()) {
        if ((*macro).second.params.size() != (keywords.size() - 1))
          throw EXCEPTION(ParserError, boost::str(boost::format("\
Line: %1%\n\
Wrong number of parameters")
                                                  % this->get_line(i)));

        // Replace the parameters
        std::vector<std::string> code((*macro).second.code);
        std::vector<std::string>::iterator line = code.begin();
        while (line != code.end()) {
          File::size_type p;
          for (p = 0; p < keywords.size() - 1; p++)
            *line = boost::regex_replace(*line,
                                         boost::regex((*macro).second.params[p]),
                                         keywords[p + 1]);

          ++line;
        }

        // Insert the code
        this->remove(i, 1);
        line = code.begin();
        while (line != code.end()) {
          this->insert(i, *line);

          ++line;
          if (line != code.end())
            i++;
        }
      }
  }
}

/**
 * Replace the defines (and ifdef/ifndefs blocks) with its value.
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
    } else if (this->is_ifdef(i)) {
      std::string ifdef(this->get_ifdef(i));

      File::size_type end = i;
      while ((end < this->lines()) and not this->is_endif(end))
        end++;
      if (end == this->lines())
        throw EXCEPTION(ParserError, boost::str(boost::format("\
Line: %1%\n\
ifdef block has not .endifdef")
                                                % this->get_line(i)));

      if (this->defines_.find(ifdef) != this->defines_.end()) {
        // Remove the begining and end of the block
        this->remove(i, 1);
        this->remove(end - 1, 1);
      } else {
        // Remove all the block
        this->remove(i, end - i + 1);
      }
    } else if (this->is_ifndef(i)) {
      std::string ifndef(this->get_ifndef(i));

      File::size_type end = i;
      while ((end < this->lines()) and not this->is_endif(end))
        end++;
      if (end == this->lines())
        throw EXCEPTION(ParserError, boost::str(boost::format("\
Line: %1%\n\
ifndef block has not .endifndef")
                                                % this->get_line(i)));

      if (this->defines_.find(ifndef) == this->defines_.end()) {
        // Remove the begining and end of the block
        this->remove(i, 1);
        this->remove(end - 1, 1);
      } else {
        // Remove all the block
        this->remove(i, end - i + 1);
      }
    } else
      i++;

  // Replace defines
  for (i = 0; i < this->lines(); i++) {
    // Definicions can be replaced several times if it's value is other
    // definition instead of a number
    bool again = true;

    // Don't replace defines in blank lines or comments
    // TODO: the defines are replaced if they are in a inline comment
    if (not this->is_blank(i) and not this->is_comment(i))
      while (again) {
        again = false;

        std::vector<std::string> keywords(this->get_keywords(i));
        std::vector<std::string>::const_iterator key = keywords.begin();
        while (key != keywords.end()) {
          std::map<std::string, std::string>::const_iterator define =
            this->defines_.find(*key);
          if (define != this->defines_.end()) {
            this->get_line(i) =
              boost::regex_replace(this->get_line(i),
                                   boost::regex(std::string(BEGIN_WORD) +
                                                (*define).first +
                                                std::string(END_WORD)),
                                   (*define).second);
            again = true;
          }

          ++key;
        }
      }
  }
}

/**
 * Replace the blocks of memory with the zero values.
 * @exception ParserError error found in the code.
 */
void Source::replace_blocks()
{
  for (File::size_type i = 0; i < this->lines(); i++)
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
Label %2% already defined")
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
      std::vector<std::string> keywords(this->get_keywords(i));
      std::vector<std::string>::const_iterator key = keywords.begin();
      while (key != keywords.end()) {
        std::map<std::string, Address>::const_iterator label =
          this->labels_.find(*key);
        if (label != this->labels_.end()) {
          std::string address(boost::str(boost::format("0x%08X") %
                                         (*label).second));
          this->get_line(i) =
            boost::regex_replace(this->get_line(i),
                                 boost::regex(std::string(BEGIN_WORD) +
                                              (*label).first +
                                              std::string(END_WORD)),
                                 address);
        }

        ++key;
      }

      lines_code++;
    } else if (this->is_instruction(i)) {
      // If a label is used in a instruction, it's replaced by the offset to pc
      std::vector<std::string> keywords(this->get_keywords(i));
      std::vector<std::string>::const_iterator key = keywords.begin();
      while (key != keywords.end()) {
        std::map<std::string, Address>::const_iterator label =
          this->labels_.find(*key);
        if (label != this->labels_.end()) {
          std::string offset(boost::str(boost::format("0x%04X") %
                                        static_cast<Uint16>(((*label).second -
                                                             lines_code * sizeof(Word)))));
          this->get_line(i) =
            boost::regex_replace(this->get_line(i),
                                 boost::regex(std::string(BEGIN_WORD) +
                                              (*label).first +
                                              std::string(END_WORD)),
                                 offset);
        }

        ++key;
      }

      lines_code++;
    } else if(this->is_data(i)) {
      lines_code++;
    }
}


/**
 * Delete comments and blank lines.
 */
void Source::strip()
{
  File::size_type i = 0;
  while (i < this->lines())
    if (this->is_blank(i) or this->is_comment(i))
      this->remove(i, 1);
    else
      i++;
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
#if defined(IS_BIG_ENDIAN)
    return this->get_data(line);
#elif defined(IS_LITTLE_ENDIAN)
    return change_byte_order(this->get_data(line));
#else
#error endianness not specified
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
 * Check if a line is the begining of a macro.
 * @param line Number of the line.
 * @return the check result.
 * @exception CPUException if line > lines of the file.
 */
bool Source::is_macro(File::size_type line) const
{
  return boost::regex_match(this->get_line(line), re_macro);
}

/**
 * Check if a line is the end of a macro.
 * @param line Number of the line.
 * @return the check result.
 * @exception CPUException if line > lines of the file.
 */
bool Source::is_endmacro(File::size_type line) const
{
  return boost::regex_match(this->get_line(line), re_endmacro);
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
 * Check if a line is the begining of a ifdef block.
 * @param line Number of the line.
 * @return the check result.
 * @exception CPUException if line > lines of the file.
 */
bool Source::is_ifdef(File::size_type line) const
{
  return boost::regex_match(this->get_line(line), re_ifdef);
}

/**
 * Check if a line is the begining of a ifndef.
 * @param line Number of the line.
 * @return the check result.
 * @exception CPUException if line > lines of the file.
 */
bool Source::is_ifndef(File::size_type line) const
{
  return boost::regex_match(this->get_line(line), re_ifndef);
}

/**
 * Check if a line is the end of a if.
 * @param line Number of the line.
 * @return the check result.
 * @exception CPUException if line > lines of the file.
 */
bool Source::is_endif(File::size_type line) const
{
  return boost::regex_match(this->get_line(line), re_endif);
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
  return boost::regex_match(this->get_line(line), re_inst0) or
    boost::regex_match(this->get_line(line), re_inst0i) or
    boost::regex_match(this->get_line(line), re_inst1) or
    boost::regex_match(this->get_line(line), re_inst1i) or
    boost::regex_match(this->get_line(line), re_inst2) or
    boost::regex_match(this->get_line(line), re_inst2i) or
    boost::regex_match(this->get_line(line), re_inst3);
}


/**
 * Return all the keywords of a line.
 *
 * If the line has not any keyword a empty vector is returned.
 * @param line Number of the line.
 * @return the components of a define.
 * @exception CPUException if line > lines of the file.
 */
std::vector<std::string> Source::get_keywords(File::size_type line) const
{
  std::vector<std::string> result;

  std::string text = this->get_line(line);
  std::string::const_iterator start = text.begin(), end = text.end();
  boost::smatch what;

  while (boost::regex_search(start, end, what, re_keyword)) {
    result.insert(result.end(), what.begin() + 1, what.end());

    start = what[0].second;
  }

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
 * Return the components of a macro.
 *
 * If the line is not the begining of a macro a empty vector is returned.
 * @param line Number of the line.
 * @return the components of a macro.
 * @exception CPUException if line > lines of the file.
 * @exception ParserError error found in the code.
 */
std::pair<std::string, Source::Macro> Source::get_macro(File::size_type line)
  const
{
  std::pair<std::string, Source::Macro> result;

  boost::smatch what;
  if (boost::regex_match(this->get_line(line), what, re_macro)) {
    std::vector<std::string> keywords(this->get_keywords(line));
    result.first = keywords[1];
    result.second.params.insert(result.second.params.begin(),
                                keywords.begin() + 2, keywords.end());
    File::size_type end = line;
    while ((++end < this->lines()) and not this->is_endmacro(end))
      result.second.code.push_back(this->get_line(end));

    if (end == this->lines())
        throw EXCEPTION(ParserError, boost::str(boost::format("\
Line: %1%\n\
Macro has not .endmacro")
                                                % this->get_line(line)));
  }

  return result;
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
 * Return the identificator of a ifdef.
 *
 * If the line is not a define a empty vector is returned.
 * The first position is the name and the second is the value.
 * @param line Number of the line.
 * @return the identificator.
 * @exception CPUException if line > lines of the file.
 */
std::string Source::get_ifdef(File::size_type line) const
{
  std::string result;

  boost::smatch what;
  if (boost::regex_match(this->get_line(line), what, re_ifdef))
    result = what[1];

  return result;
}

/**
 * Return the identificator of a ifndef.
 *
 * If the line is not a define a empty vector is returned.
 * The first position is the name and the second is the value.
 * @param line Number of the line.
 * @return the identificator.
 * @exception CPUException if line > lines of the file.
 */
std::string Source::get_ifndef(File::size_type line) const
{
  std::string result;

  boost::smatch what;
  if (boost::regex_match(this->get_line(line), what, re_ifndef))
    result = what[1];

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
