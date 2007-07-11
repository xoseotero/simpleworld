/**
 * @file cpu/file.cpp
 * Arithmetic logic unit
 *
 * begin:     Fri, 10 Nov 2006 08:27:00 +0100
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

#include "file.hpp"

namespace SimpleWorld
{
namespace CPU
{

File::File()
{
}

File::File(const File& file)
{
  std::vector<std::string>::const_iterator iter = file.lines_.begin();
  while (iter != file.lines_.end()) {
    this->lines_.push_back(*iter);
    ++iter;
  }
}

File::File(std::string filename)
{
  this->load(filename);
}

File::size_type File::lines() const
{
  return this->lines_.size();
}

std::string& File::get_line(File::size_type pos)
{
  if ((pos + 1) > this->lines_.size())
    throw LineOutOfRange(__FILE__, __LINE__, pos);

  return this->lines_[pos];
}

const std::string& File::get_line(File::size_type pos) const
{
  if ((pos + 1) > this->lines_.size())
    throw LineOutOfRange(__FILE__, __LINE__, pos);

  return this->lines_[pos];
}

void File::insert(File::size_type pos, const std::string& line)
{
  if (pos > this->lines_.size())
    throw LineOutOfRange(__FILE__, __LINE__, pos);

  this->lines_.insert(this->lines_.begin() + pos, line);
}

void File::insert(File::size_type pos, const File& file)
{
  if (pos > this->lines_.size())
    throw LineOutOfRange(__FILE__, __LINE__, pos);

  this->lines_.insert(this->lines_.begin() + pos,
                      file.lines_.begin(), file.lines_.end());
}

void File::remove(File::size_type pos, File::size_type n)
{
  if ((pos + 1) > this->lines_.size())
    throw LineOutOfRange(__FILE__, __LINE__, n);

  this->lines_.erase(this->lines_.begin() + pos);
}


void File::load(std::string filename)
{
  this->lines_.clear();

  std::ifstream is(filename.c_str());
  if (is.rdstate() & std::ifstream::failbit)
    throw FileAccessError(__FILE__, __LINE__,
			  filename, "File can't be opened to read.");

  std::string str;
  while (std::getline(is, str))
    this->lines_.push_back(str);
}

void File::save(std::string filename) const
{
  std::ofstream os(filename.c_str());
  if (os.rdstate() & std::ofstream::failbit)
    throw FileAccessError(__FILE__, __LINE__,
			  filename, "File can't be opened to write.");

  std::vector<std::string>::const_iterator iter = this->lines_.begin();
  while (iter != this->lines_.end()) {
    os << *iter << std::endl;
    ++iter;
  }
}

std::string& File::operator [](File::size_type pos)
{
  return this->get_line(pos);
}

const std::string& File::operator [](File::size_type pos) const
{
  return this->get_line(pos);
}


std::istream& operator >>(std::istream& is, File& file)
{
  file.lines_.clear();

  std::string str;
  while (std::getline(is, str))
    file.lines_.push_back(str);

  return is;
}

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
