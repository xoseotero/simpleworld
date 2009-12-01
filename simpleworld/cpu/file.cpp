/**
 * @file simpleworld/cpu/file.cpp
 * Arithmetic logic unit
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

#include <boost/format.hpp>

#include <simpleworld/ioerror.hpp>

#include "exception.hpp"
#include "file.hpp"

namespace simpleworld
{
namespace cpu
{

/**
 * Constructor for a empty file.
 */
File::File()
{
}

/**
 * Copy constructor.
 * @param file File to copy.
 */
File::File(const File& file)
{
  std::vector<std::string>::const_iterator iter = file.lines_.begin();
  while (iter != file.lines_.end()) {
    this->lines_.push_back(*iter);
    ++iter;
  }
}

/**
 * Constructor that opens a file.
 * @param filename File to open.
 * @exception IOError File can't be opened
 */
File::File(std::string filename)
{
  this->load(filename);
}


/**
 * Number of lines.
 * @return the number of lines.
 */
File::size_type File::lines() const
{
  return this->lines_.size();
}

/**
 * Get a line.
 * @param pos Position of the line.
 * @exception CPUException if pos > lines
 */
std::string& File::get_line(File::size_type pos)
{
  if ((pos + 1) > this->lines_.size())
    throw EXCEPTION(CPUException, boost::str(boost::format("\
Line %1% is out of range")
                                             % pos));

  return this->lines_[pos];
}

/**
 * Get a line.
 * @param pos Position of the line.
 * @exception CPUException if pos > lines
 */
const std::string& File::get_line(File::size_type pos) const
{
  if ((pos + 1) > this->lines_.size())
    throw EXCEPTION(CPUException, boost::str(boost::format("\
Line %1% is out of range")
                                             % pos));

  return this->lines_[pos];
}

/**
 * Insert a new line in the file.
 * @param pos Position of the new line.
 * @param line Line to insert.
 * @exception CPUException if pos > lines
 */
void File::insert(File::size_type pos, const std::string& line)
{
  if (pos > this->lines_.size())
    throw EXCEPTION(CPUException, boost::str(boost::format("\
Line %1% is out of range")
                                             % pos));

  this->lines_.insert(this->lines_.begin() + pos, line);
}

/**
 * Insert the lines of a File in the file.
 * @param pos Position of the new line.
 * @param file File to insert.
 * @exception CPUException if pos > lines
 */
void File::insert(File::size_type pos, const File& file)
{
  if (pos > this->lines_.size())
    throw EXCEPTION(CPUException, boost::str(boost::format("\
Line %1% is out of range")
                                             % pos));

  this->lines_.insert(this->lines_.begin() + pos,
                      file.lines_.begin(), file.lines_.end());
}

/**
 * Remove n lines from the file.
 * @param pos Position of the first line to remove.
 * @param n Number of lines to remove.
 * @exception CPUException if pos > lines or pos + n > lines
 */
void File::remove(File::size_type pos, File::size_type n)
{
  if ((pos + 1) > this->lines_.size())
    throw EXCEPTION(CPUException, boost::str(boost::format("\
Line %1% is out of range")
                                             % pos));

  this->lines_.erase(this->lines_.begin() + pos,
                     this->lines_.begin() + pos + n);
}


/**
 * Load from a file.
 *
 * Before the load, all the lines of the File are removed.
 * @param filename File to open.
 * @exception IOError File can't be opened
 */
void File::load(std::string filename)
{
  this->lines_.clear();

  std::ifstream is(filename.c_str());
  if (is.rdstate() & std::ifstream::failbit)
    throw EXCEPTION(IOError, boost::str(boost::format("\
File %1% is not readable")
                                        % filename));

  std::string str;
  while (std::getline(is, str))
    this->lines_.push_back(str);
}

/**
 * Save to a file.
 * @param filename File name where to save.
 * @exception IOError File can't be saved
 */
void File::save(std::string filename) const
{
  std::ofstream os(filename.c_str());
  if (os.rdstate() & std::ofstream::failbit)
    throw EXCEPTION(IOError, boost::str(boost::format("\
File %1% is not writable")
                                        % filename));

  std::vector<std::string>::const_iterator iter = this->lines_.begin();
  while (iter != this->lines_.end()) {
    os << *iter << std::endl;
    ++iter;
  }
}


/**
 * Load lines.
 *
 * Before the load, all the lines of the File are removed.
 * @param is From where to get the file
 * @param file Where to load the lines
 * @return Reference to where to get the file
 */
std::istream& operator >>(std::istream& is, File& file)
{
  file.lines_.clear();

  std::string str;
  while (std::getline(is, str))
    file.lines_.push_back(str);

  return is;
}

/**
 * Print the lines.
 * @param os Where to print the file
 * @param file File to print
 * @return Reference to where to print
 */
std::ostream& operator <<(std::ostream& os, const File& file)
{
  std::vector<std::string>::const_iterator iter = file.lines_.begin();
  while (iter != file.lines_.end()) {
    os << *iter << std::endl;
    ++iter;
  }

  return os;
}

}
}
