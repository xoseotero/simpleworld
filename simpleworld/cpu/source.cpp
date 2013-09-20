/**
 * @file simpleworld/cpu/source.cpp
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

#include <algorithm>
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
#include "errordirective.hpp"

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

/**
 * Return all the keywords of a line.
 *
 * If the line has not any keyword a empty vector is returned.
 * @param line Text to check.
 * @return the components of a define.
 */
static std::vector<std::string> get_keywords(const std::string& line)
{
  std::vector<std::string> result;

  std::string::const_iterator start = line.begin(), end = line.end();
  boost::smatch what;

  while (boost::regex_search(start, end, what, re_keyword)) {
    result.insert(result.end(), what.begin() + 1, what.end());

    start = what[0].second;
  }

  return result;
}


// A blank line
static const boost::regex re_blank(BEGIN_LINE
                                   OPTIONAL_SPACE
                                   END_LINE);

/**
 * Check if a line is a blank one.
 * @param line Text to check.
 * @return the check result.
 */
static bool is_blank(const std::string& line)
{
  return boost::regex_match(line, re_blank);
}



// A line with only a comment
static const boost::regex re_comment(BEGIN_LINE
                                     OPTIONAL_SPACE
                                     "(" COMMENT ")"
                                     END_LINE);

/**
 * Check if a line is a comment.
 * @param line Text to check.
 * @return the check result.
 */
static bool is_comment(const std::string& line)
{
  return boost::regex_match(line, re_comment);
}


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

/**
 * Check if a line is a include.
 * @param line Text to check.
 * @return the check result.
 */
static bool is_include(const std::string& line)
{
  return boost::regex_match(line, re_include);
}

/**
 * Return the included file.
 *
 * If the line is not a include a empty string is returned.
 * @param line Text to check.
 * @return the file name.
 */
static std::string get_include(const std::string& line)
{
  std::string result;

  boost::smatch what;
  if (boost::regex_match(line, what, re_include))
    result = what[1];

  return result;
}


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

/**
 * Check if a line is the begining of a macro.
 * @param line Text to check.
 * @return the check result.
 */
static bool is_macro(const std::string& line)
{
  return boost::regex_match(line, re_macro);
}

// A line with the end of a macro block
static const boost::regex re_endmacro(BEGIN_LINE
                                      OPTIONAL_SPACE
                                      "\\.endmacro"
                                      OPTIONAL_SPACE
                                      OPTIONAL_COMMENT
                                      END_LINE);

/**
 * Check if a line is the end of a macro.
 * @param line Text to check.
 * @return the check result.
 */
static bool is_endmacro(const std::string& line)
{
  return boost::regex_match(line, re_endmacro);
}

/**
 * Return the components of a macro.
 *
 * If the line is not the begining of a macro a empty vector is returned.
 * @param file Entire file.
 * @param line Line to check.
 * @return the components of a macro.
 * @exception CPUException if line > lines of the file.
 */
static std::pair<std::string, Source::Macro> get_macro(const File& file,
                                                       File::size_type line)
{
  std::pair<std::string, Source::Macro> result;

  const std::string text(file.get_line(line));
  boost::smatch what;
  if (boost::regex_match(text, what, re_macro)) {
    std::vector<std::string> keywords(get_keywords(text));
    result.first = keywords[1];
    result.second.params.insert(result.second.params.begin(),
                                keywords.begin() + 2, keywords.end());
    File::size_type end = line;
    File::size_type num_lines = file.lines();
    while ((++end < num_lines) and not is_endmacro(file.get_line(end)))
      result.second.code.push_back(file.get_line(end));

    if (end == num_lines)
        throw EXCEPTION(ParserError, boost::str(boost::format("\
Line: %1%\n\
Macro has not .endmacro")
                                                % text));
  }

  return result;
}


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

/**
 * Check if a line is a define.
 * @param line Text to check.
 * @return the check result.
 */
static bool is_define(const std::string& line)
{
  return boost::regex_match(line, re_define);
}

/**
 * Return the components of a define.
 *
 * If the line is not a define a empty vector is returned.
 * The first position is the name and the second is the value.
 * @param line Text to check.
 * @return the components of a define.
 */
static std::vector<std::string> get_define(const std::string& line)
{
  std::vector<std::string> result;

  boost::smatch what;
  if (boost::regex_match(line, what, re_define))
    result.insert(result.begin(), what.begin() + 1, what.end());

  return result;
}


// A line with the begining of a ifdef block
static const boost::regex re_ifdef(BEGIN_LINE
                                   OPTIONAL_SPACE
                                   "\\.ifdef"
                                   SPACE
                                   "(" KEYWORD ")"
                                   OPTIONAL_SPACE
                                   OPTIONAL_COMMENT
                                   END_LINE);

/**
 * Check if a line is the begining of a ifdef block.
 * @param line Text to check.
 * @return the check result.
 */
static bool is_ifdef(const std::string& line)
{
  return boost::regex_match(line, re_ifdef);
}


// A line with the begining of a ifndef block
static const boost::regex re_ifndef(BEGIN_LINE
                                    OPTIONAL_SPACE
                                    "\\.ifndef"
                                    SPACE
                                    "(" KEYWORD ")"
                                    OPTIONAL_SPACE
                                    OPTIONAL_COMMENT
                                    END_LINE);

/**
 * Return the identificator of a ifdef.
 *
 * If the line is not a define a empty vector is returned.
 * The first position is the name and the second is the value.
 * @param line Text to check.
 * @return the identificator.
 */
static std::string get_ifdef(const std::string& line)
{
  std::string result;

  boost::smatch what;
  if (boost::regex_match(line, what, re_ifdef))
    result = what[1];

  return result;
}


/**
 * Check if a line is the begining of a ifndef.
 * @param line Text to check.
 * @return the check result.
 */
static bool is_ifndef(const std::string& line)
{
  return boost::regex_match(line, re_ifndef);
}


// A line with the end of a if block
static const boost::regex re_endif(BEGIN_LINE
                                   OPTIONAL_SPACE
                                   "\\.endif"
                                   OPTIONAL_SPACE
                                   OPTIONAL_COMMENT
                                   END_LINE);

/**
 * Return the identificator of a ifndef.
 *
 * If the line is not a define a empty vector is returned.
 * The first position is the name and the second is the value.
 * @param line Text to check.
 * @return the identificator.
 */
static std::string get_ifndef(const std::string& line)
{
  std::string result;

  boost::smatch what;
  if (boost::regex_match(line, what, re_ifndef))
    result = what[1];

  return result;
}

/**
 * Check if a line is the end of a if.
 * @param line Text to check.
 * @return the check result.
 */
static bool is_endif(const std::string& line)
{
  return boost::regex_match(line, re_endif);
}


// A line with a block of memory
static const boost::regex re_block(BEGIN_LINE
                                   OPTIONAL_SPACE
                                   "\\.block"
                                   SPACE
                                   "(" NUMBER32 ")"
                                   OPTIONAL_SPACE
                                   OPTIONAL_COMMENT
                                   END_LINE);

/**
 * Check if a line is a block of memory.
 * @param line Text to check.
 * @return the check result.
 */
static bool is_block(const std::string& line)
{
  return boost::regex_match(line, re_block);
}

/**
 * Return the value of the block of memory.
 *
 * If the line is not a block of memory zero value is returned.
 * @param line Text to check.
 * @return the components of a define.
 */
static Address get_block(const std::string& line)
{
  Address result = 0;

  boost::smatch what;
  if (boost::regex_match(line, what, re_block))
    sscanf(std::string(what[1]).c_str(), "%x", &result);

  return result;
}


// A line with a label definition
static const boost::regex re_label(BEGIN_LINE
                                   OPTIONAL_SPACE
                                   "\\.label"
                                   SPACE
                                   "(" KEYWORD ")"
                                   OPTIONAL_SPACE
                                   OPTIONAL_COMMENT
                                   END_LINE);

/**
 * Check if a line is a label definition.
 * @param line Text to check.
 * @return the check result.
 */
static bool is_label(const std::string& line)
{
  return boost::regex_match(line, re_label);
}

/**
 * Return the label name.
 *
 * If the line is not a label definition a empty string is returned.
 * @param line Text to check.
 * @return the label.
 */
static std::string get_label(const std::string& line)
{
  std::string result;

  boost::smatch what;
  if (boost::regex_match(line, what, re_label))
    result = what[1];

  return result;
}


// A line with a label used as data
static const boost::regex re_label_as_data(BEGIN_LINE
                                           OPTIONAL_SPACE
                                           "(" KEYWORD ")"
                                           OPTIONAL_SPACE
                                           OPTIONAL_COMMENT
                                           END_LINE);

/**
 * Check if a line is a label used as data.
 * @param label Current labes.
 * @param line Text to check.
 * @return the check result.
 */
static bool is_label_as_data(const std::map<std::string, Address>& labels,
                             const std::string& line)
{
  std::string result;

  boost::smatch what;
  if (boost::regex_match(line, what, re_label_as_data))
    result = what[1];

  return labels.find(result) != labels.end();
}


// A warning
static const boost::regex re_warning(BEGIN_LINE
                                     OPTIONAL_SPACE
                                     "\\.warning"
                                     SPACE
                                     "\""
                                     "(" ANYTHING ")"
                                     "\""
                                     OPTIONAL_SPACE
                                     OPTIONAL_COMMENT
                                     END_LINE);

/**
 * Check if a line is a warning.
 * @param line Text to check.
 * @return the check result.
 */
static bool is_warning(const std::string& line)
{
  return boost::regex_match(line, re_warning);
}

/**
 * Return the warning text.
 *
 * If the line is not a warning a empty string is returned.
 * @param line Text to check.
 * @return the text.
 */
static std::string get_warning(const std::string& line)
{
  std::string result;

  boost::smatch what;
  if (boost::regex_match(line, what, re_warning))
    result = what[1];

  return result;
}


// A error
static const boost::regex re_error(BEGIN_LINE
                                   OPTIONAL_SPACE
                                   "\\.error"
                                   SPACE
                                   "\""
                                   "(" ANYTHING ")"
                                   "\""
                                   OPTIONAL_SPACE
                                   OPTIONAL_COMMENT
                                   END_LINE);

/**
 * Check if a line is a error.
 * @param line Text to check.
 * @return the check result.
 */
static bool is_error(const std::string& line)
{
  return boost::regex_match(line, re_error);
}

/**
 * Return the error text.
 *
 * If the line is not a error a empty string is returned.
 * @param line Text to check.
 * @return the text.
 */
static std::string get_error(const std::string& line)
{
  std::string result;

  boost::smatch what;
  if (boost::regex_match(line, what, re_error))
    result = what[1];

  return result;
}


// Data
static const boost::regex re_data(BEGIN_LINE
                                  OPTIONAL_SPACE
                                  "(" NUMBER32 ")"
                                  OPTIONAL_SPACE
                                  OPTIONAL_COMMENT
                                  END_LINE);

/**
 * Check if a line is data (32 bits number).
 * @param line Text to check.
 * @return the check result.
 */
static bool is_data(const std::string& line)
{
  return boost::regex_match(line, re_data);
}

/**
 * Return the data.
 *
 * If the line is not data 0 is returned.
 * @param line Text to check.
 * @return the data.
 */
static Word get_data(const std::string& line)
{
  Word result = 0;

  boost::smatch what;
  if (boost::regex_match(line, what, re_data)) {
    const char* str = std::string(what[1]).c_str();
    char* ptr;
    result = std::strtoul(str, &ptr, 16);
  }

  return result;
}


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
 * Check if a line is a instruction.
 * @param line Text to check.
 * @return the check result.
 */
static bool is_instruction(const std::string& line)
{
  return boost::regex_match(line, re_inst0) or
    boost::regex_match(line, re_inst0i) or
    boost::regex_match(line, re_inst1) or
    boost::regex_match(line, re_inst1i) or
    boost::regex_match(line, re_inst2) or
    boost::regex_match(line, re_inst2i) or
    boost::regex_match(line, re_inst3);
}

/**
 * Return the components of a instruction.
 *
 * If the line is not a instruction a empty vector is returned.
 * A instruction can have 0-3 operators.
 * The firs position is the instruction and the next ones are the operators.
 * @param line Text to check.
 * @return the components of a define.
 */
static std::vector<std::string> get_instruction(const std::string& line)
{
  std::vector<std::string> result;

  boost::smatch what;
  if (boost::regex_match(line, what, re_inst0)) {
    result.insert(result.begin(), what.begin() + 1, what.end());
    return result;
  } else if (boost::regex_match(line, what, re_inst0i)) {
    result.insert(result.begin(), what.begin() + 1, what.end());
    return result;
  } else if (boost::regex_match(line, what, re_inst1)) {
    result.insert(result.begin(), what.begin() + 1, what.end());
    return result;
  } else if (boost::regex_match(line, what, re_inst1i)) {
    result.insert(result.begin(), what.begin() + 1, what.end());
    return result;
  } else if (boost::regex_match(line, what, re_inst2)) {
    result.insert(result.begin(), what.begin() + 1, what.end());
    return result;
  } else if (boost::regex_match(line, what, re_inst2i)) {
    result.insert(result.begin(), what.begin() + 1, what.end());
    return result;
  } else if (boost::regex_match(line, what, re_inst3)) {
    result.insert(result.begin(), what.begin() + 1, what.end());
    return result;
  }

  return result;
}


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
 */
Source::Source(const ISA& isa)
  : File(), isa_(isa)
{
}

/**
 * Constructor.
 * @param isa Instruction set architecture of the CPU
 * @param file File to copy.
 * @exception IOError if file can't be opened
 */
Source::Source(const ISA& isa, const File& file)
  : File(file), isa_(isa)
{
}

/**
 * Constructor.
 * @param isa Instruction set architecture of the CPU
 * @param filename File to open.
 * @exception IOError if file can't be opened
 */
Source::Source(const ISA& isa, const std::string& filename)
  : File(filename), isa_(isa)
{
  // The main file can't be included
  std::string abs_path(fs::absolute(fs::path(filename)).string());
  this->includes_.insert(abs_path); // Using the absolute path
}


/**
 * Clear the content of the source code and reset the internal state.
 */
void Source::clear()
{
  this->remove(0, this->lines());
  this->include_path_.clear();
  this->includes_.clear();
  this->macros_.clear();
  this->defines_.clear();
  this->labels_.clear();
}


/**
 * Load from a file.
 * Before the load, all the lines of the File are removed.
 * @param filename File to open.
 */
void Source::load(const File& file)
{
  File::remove(0, this->lines());
  this->clear();
  File::insert(file);
}

/**
 * Load from a file.
 * Before the load, all the lines of the File are removed.
 * @param filename File to open.
 * @exception IOError File can't be opened
 */
void Source::load(std::string filename)
{
  this->clear();
  File::load(filename);

  // The main file can't be included
  std::string abs_path(fs::absolute(fs::path(filename)).string());
  this->includes_.insert(abs_path); // Using the absolute path
}


/**
 * Add a include path.
 * @param path Path to add.
 * @exception CPUException Path already added.
 */
void Source::add_include_path(std::string path)
{
  if (std::find(this->include_path_.begin(), this->include_path_.end(),
		path) != this->include_path_.end())
    throw EXCEPTION(CPUException, boost::str(boost::format("\
Path %1% already added")
                                                % path));

  this->include_path_.push_back(path);
}

/**
 * Add a define.
 * @param name Name of the define.
 * @param value Value of the define.
 * @exception CPUException name duplicated.
 */
void Source::add_define(std::string name, std::string value)
{
  if (this->defines_.find(name) != this->defines_.end())
    throw EXCEPTION(CPUException, boost::str(boost::format("\
Constant %1% already defined")
                                                % name));

  this->defines_.insert(std::pair<std::string, std::string>(name, value));
}

/**
 * Add a macro.
 * @param name Name of the macro.
 * @param value Value of the macro.
 * @exception CPUException name duplicated.
 */
void Source::add_macro(std::string name, Macro value)
{
  if (this->macros_.find(name) != this->macros_.end())
    throw EXCEPTION(CPUException, boost::str(boost::format("\
Constant %1% already defined")
                                                % name));

  this->macros_.insert(std::pair<std::string, Macro>(name, value));
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
 * @return the warning messages generated during the compilation.
 * @exception IOERROR if a problem with file happen.
 * @exception ParserError error found in the code.
 * @exception ErrorDirective error directive found in the code.
 */
std::vector<std::string> Source::compile(std::string filename)
{
  this->preprocess(true);

  std::ofstream file(filename.c_str(), std::ios::binary | std::ios::trunc);
  if (file.rdstate() & std::ofstream::failbit)
    throw EXCEPTION(IOError, boost::str(boost::format("\
File %1% is not writable")
                                        % filename));

  std::vector<std::string> warnings;
  for (File::size_type i = 0; i < this->lines(); i++) {
    if (is_blank(this->get_line(i)) or is_comment(this->get_line(i)))
      continue;

    if (is_warning(this->get_line(i))) {
      warnings.push_back(get_warning(this->get_line(i)));
      continue;
    }

    if (is_error(this->get_line(i)))
      throw EXCEPTION(ErrorDirective, get_error(this->get_line(i)));

    Word code = this->compile(i);
    file.write(reinterpret_cast<char*>(&code), sizeof(Word));
    if (file.fail())
      throw EXCEPTION(IOError, boost::str(boost::format("\
Can't write in file %1%")
                                           % filename));
  }

  file.close();

  return warnings;
}


/**
 * Replace the .include lines with the file contents.
 * @exception IOError if a file can't be found.
 */
void Source::replace_includes()
{
  File::size_type i = 0;
  while (i < this->lines()) {
    if (is_include(this->get_line(i))) {
      fs::path filename(find_file(this->include_path_,
                                  get_include(this->get_line(i))));
      if (filename.empty())
        throw EXCEPTION(IOError, boost::str(boost::format("\
File %1% not found")
                                            % get_include(this->get_line(i))));

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
    if (is_macro(this->get_line(i))) {
      std::pair<std::string, Source::Macro> macro(get_macro(*this, i));
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
    if (not is_blank(this->get_line(i)) and
        not is_comment(this->get_line(i))) {
      std::vector<std::string> keywords(get_keywords(this->get_line(i)));
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
    if (is_define(this->get_line(i))) {
      std::vector<std::string> define(get_define(this->get_line(i)));
      if (this->defines_.find(define[0]) != this->defines_.end())
        throw EXCEPTION(ParserError, boost::str(boost::format("\
Line: %1%\n\
Constant %2% already defined")
                                                % this->get_line(i)
                                                % define[0]));

      this->remove(i, 1);
      this->defines_.insert(std::pair<std::string,
                            std::string>(define[0], define[1]));
    } else if (is_ifdef(this->get_line(i))) {
      std::string ifdef(get_ifdef(this->get_line(i)));

      File::size_type end = i;
      while ((end < this->lines()) and not is_endif(this->get_line(end)))
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
    } else if (is_ifndef(this->get_line(i))) {
      std::string ifndef(get_ifndef(this->get_line(i)));

      File::size_type end = i;
      while ((end < this->lines()) and not is_endif(this->get_line(end)))
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
    if (not is_blank(this->get_line(i)) and not is_comment(this->get_line(i)))
      while (again) {
        again = false;

        std::vector<std::string> keywords(get_keywords(this->get_line(i)));
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
    if (is_block(this->get_line(i))) {
      Address size = get_block(this->get_line(i));
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
    if (is_label(this->get_line(i))) {
      std::string label(get_label(this->get_line(i)));
      if (this->labels_.find(label) != this->labels_.end())
        throw EXCEPTION(ParserError, boost::str(boost::format("\
Line: %1%\n\
Label %2% already defined")
                                                % this->get_line(i)
                                                % label));
      this->remove(i, 1);
      this->labels_.insert(std::pair<std::string, Address>(label,
        lines_code * sizeof(Word)));
    } else if (is_instruction(this->get_line(i)) or
               is_data(this->get_line(i))) {
      lines_code++;
      i++;
    } else
      i++;

  // Replace labels
  for (i = 0, lines_code = 0; i < this->lines(); i++)
    // is_label_as_data() must be checked first because a label could be
    // confused with a instruction without arguments
    if (is_label_as_data(this->labels_, this->get_line(i))) {
      // If a label is used as data, it's replaced by the address
      std::vector<std::string> keywords(get_keywords(this->get_line(i)));
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
    } else if (is_instruction(this->get_line(i))) {
      // If a label is used in a instruction, it's replaced by the offset to pc
      std::vector<std::string> keywords(get_keywords(this->get_line(i)));
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
    } else if(is_data(this->get_line(i))) {
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
    if (is_blank(this->get_line(i)) or is_comment(this->get_line(i)))
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
  if (is_data(this->get_line(line))) {
#if defined(IS_BIG_ENDIAN)
    return get_data(this->get_line(line));
#elif defined(IS_LITTLE_ENDIAN)
    return change_byte_order(get_data(this->get_line(line)));
#else
#error endianness not specified
#endif
  }

  // Is a instruction
  std::vector<std::string> keywords(get_instruction(this->get_line(line)));

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
      // check if the string is not a number
      if (ptr == str)
        throw EXCEPTION(ParserError, boost::str(boost::format("\
%1% is not a valid number")
                                                % str));
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

}
}
