/**
 * @file cpu/source.cpp
 * Simple World Language source file.
 *
 * begin:     Wed, 15 Nov 2006 21:31:26 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2006, Xosé Otero <xoseotero@users.sourceforge.net>
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

#include <cstdio>
#include <cstdlib>

#include <boost/format.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/regex.hpp>
namespace fs = boost::filesystem;

#include "word.hpp"
#include "source.hpp"

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
// A line with a constant definition
static const boost::regex re_constant(BEGIN_LINE
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
                                   "(" NUMBER16 ")"
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
// A line with a pragma definition (numeric value)
static const boost::regex re_pragma_number(BEGIN_LINE
                                           OPTIONAL_SPACE
                                           "\\.pragma"
                                           SPACE
                                           "(" KEYWORD ")"
                                           SPACE
                                           "(" NUMBER32 ")"
                                           OPTIONAL_SPACE
                                           OPTIONAL_COMMENT
                                           END_LINE);
// A line with a pragma definition (string value)
static const boost::regex re_pragma_text(BEGIN_LINE
                                         OPTIONAL_SPACE
                                         "\\.pragma"
                                         SPACE
                                         "(" KEYWORD ")"
                                         SPACE
                                         "\""
                                         "(" ANYTHING ")"
                                         "\""
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
// A instruction with 0 registers + inmediate
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
// A instruction with 1 register + inmediate
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
// A instruction with 2 registers + inmediate
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


Source::Source(const InstructionSet& set,
	       std::vector<std::string> include_path,
               const std::string& filename)
  : File(filename), set_(set), include_path_(include_path)
{
  // The main file can't be included
  // Using the absolute path
  std::string abs_path(fs::complete(fs::path(filename)).normalize().string());
  this->includes_.insert(abs_path);
}


void Source::compile(std::string filename)
{
  this->replace_includes();
  this->replace_constants();
  this->replace_blocks();

  std::ofstream file(filename.c_str(), std::ios::binary | std::ios::trunc);
  if (file.rdstate() & std::ofstream::failbit)
    throw FileAccessError(__FILE__, __LINE__,
                          boost::str(boost::format("%1% couldn't be created") %
                                     filename));

  File::size_type i;
  for (i = 0; i < this->lines(); i++) {
    if (this->is_blank(i) or this->is_comment(i))
      continue;

    Word code = this->compile(i);
    file.write(reinterpret_cast<char*>(&code), sizeof(Word));
    if (file.fail())
      throw FileAccessError(__FILE__, __LINE__,
                            boost::str(boost::format("Couldn't write in %1%") %
                                       filename));
  }
  file.close();
}


void Source::replace_includes()
{
  File::size_type i;
  for (i = 0; i < this->lines(); i++)
    if (this->is_include(i)) {
      fs::path filename(find_file(this->include_path_, this->get_include(i)));
      if (filename.empty())
        throw ParseError(__FILE__, __LINE__,
                          boost::str(boost::format("%1% not found") %
                                    this->get_include(i)),
                         i);

      std::string
        abs_path(fs::complete(filename).normalize().string());
      if (this->includes_.find(abs_path) != this->includes_.end())
        throw ParseError(__FILE__, __LINE__,
                         boost::str(boost::format("%1% already included") %
                                    abs_path),
                         i);
      this->remove(i, 1);
      this->insert(i, File(abs_path));
      this->includes_.insert(abs_path);
    }
}

void Source::replace_constants()
{
  // Search constants
  File::size_type i = 0;
  File::size_type lines_code = 0;
  while (i < this->lines())
    if (this->is_constant(i)) {
      std::vector<std::string> constant(this->get_constant(i));
      if (this->constants_.find(constant[0]) != this->constants_.end())
        throw ParseError(__FILE__, __LINE__,
                         boost::str(boost::format("Name %1% already defined") %
                                    constant[0]),
                         i);

      this->remove(i, 1);
      this->constants_.insert(std::pair<std::string,
                              std::string>(constant[0], constant[1]));
    } else if (this->is_label(i)) {
      std::string label(this->get_label(i));
      if (this->constants_.find(label) != this->constants_.end())
        throw ParseError(__FILE__, __LINE__,
                         boost::str(boost::format("Name %1% already defined") %
                                    label),
                         i);
      char address[11];
      std::snprintf(address, 11, "0x%x", lines_code * sizeof(Word));
      this->remove(i, 1);
      this->constants_.insert(std::pair<std::string, std::string>(label,
        address));
    } else if (this->is_instruction(i) or this->is_data(i)) {
      lines_code++;
      i++;
    } else
      i++;

  // Replace constants
  for (i = 0; i < this->lines(); i++) {
    std::map<std::string, std::string>::const_iterator iter =
      this->constants_.begin();
    while (iter != this->constants_.end()) {
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
      this->set_.instruction_info(this->set_.instruction_code(keywords[0]));
  }
  catch (InstructionNotFound& e) {
    throw ParseError(__FILE__, __LINE__,
		     boost::str(boost::format("Unknown instruction %1%") %
                                keywords[0]),
                    line);
  }

  if ((info.nregs + info.has_inmediate + 1) != keywords.size())
    throw ParseError(__FILE__, __LINE__, "Wrong number of parameters", line);

  Instruction inst;
  try {
    inst.code = info.code;
    if (info.nregs >= 1)
      inst.first = this->set_.register_code(keywords[1]);
    if (info.nregs >= 2)
      inst.second = this->set_.register_code(keywords[2]);
    if (info.nregs == 3)
      inst.address = this->set_.register_code(keywords[3]);
    else if (info.has_inmediate) {
      const char* str = keywords[info.nregs + 1].c_str();
      char* ptr;
      inst.address = std::strtoul(str, &ptr, 16);
      // This error must not happen because a regular expresion was used
      //if (ptr == str)
      //  throw ParseError(__FILE__, __LINE__,
      //		 boost::str(boost::format("Invalid inmediate value.")),
      //                 line);
    }
  }
  catch (RegisterNotFound e) {
    throw ParseError(__FILE__, __LINE__, "Unknown register", line);
  }

  return InstructionSet::encode(inst);
}


bool Source::is_blank(File::size_type line) const
{
  return boost::regex_match(this->get_line(line), re_blank);
}


bool Source::is_comment(File::size_type line) const
{
  return boost::regex_match(this->get_line(line), re_comment);
}

bool Source::is_constant(File::size_type line) const
{
  return boost::regex_match(this->get_line(line), re_constant);
}

bool Source::is_block(File::size_type line) const
{
  return boost::regex_match(this->get_line(line), re_block);
}

bool Source::is_label(File::size_type line) const
{
  return boost::regex_match(this->get_line(line), re_label);
}

bool Source::is_include(File::size_type line) const
{
  return boost::regex_match(this->get_line(line), re_include);
}

bool Source::is_data(File::size_type line) const
{
  return boost::regex_match(this->get_line(line), re_data);
}

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


std::vector<std::string> Source::get_constant(File::size_type line) const
{
  std::vector<std::string> result;

  boost::smatch what;
  if (boost::regex_match(this->get_line(line), what, re_constant))
    result.insert(result.begin(), what.begin() + 1, what.end());

  return result;
}

Address Source::get_block(File::size_type line) const
{
  Address result = 0;

  boost::smatch what;
  if (boost::regex_match(this->get_line(line), what, re_block))
    sscanf(std::string(what[1]).c_str(), "%x", &result);

  return result;
}

std::string Source::get_label(File::size_type line) const
{
  std::string result;

  boost::smatch what;
  if (boost::regex_match(this->get_line(line), what, re_label))
    result = what[1];

  return result;
}

std::string Source::get_include(File::size_type line) const
{
  std::string result;

  boost::smatch what;
  if (boost::regex_match(this->get_line(line), what, re_include))
    result = what[1];

  return result;
}

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
