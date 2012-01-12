/**
 * @file simpleworld/cpu/file.hpp
 * A file as a array of lines.
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

#ifndef SIMPLEWORLD_CPU_FILE_HPP
#define SIMPLEWORLD_CPU_FILE_HPP

#include <vector>
#include <string>
#include <fstream>

#include <simpleworld/ints.hpp>
#include <simpleworld/cpu/exception.hpp>

namespace simpleworld
{
namespace cpu
{

/**
 * A file as a array of lines.
 */
class File
{
public:
  /**
   * Size type.
   */
  typedef std::vector<std::string>::size_type size_type;


  /**
   * Constructor for a empty file.
   */
  File();

  /**
   * Copy constructor.
   * @param file File to copy.
   */
  File(const File& file);

  /**
   * Constructor that opens a file.
   * @param filename File to open.
   * @exception IOError File can't be opened
   */
  File(std::string filename);


  /**
   * Number of lines.
   * @return the number of lines.
   */
  size_type lines() const;

  /**
   * Get a line.
   * @param pos Position of the line.
   * @exception CPUException if pos > lines
   */
  std::string& get_line(size_type pos);

  /**
   * Get a line.
   * @param pos Position of the line.
   * @exception CPUException if pos > lines
   */
  const std::string& get_line(size_type pos) const;


  /**
   * Insert a new line in the file.
   * @param pos Position of the new line.
   * @param line Line to insert.
   * @exception CPUException if pos > lines
   */
  void insert(size_type pos, const std::string& line);

  /**
   * Insert a new line at the end of the file.
   * @param line Line to insert.
   * @exception CPUException if pos > lines
   */
  void insert(const std::string& line) {
    this->insert(this->lines(), line);
  }

  /**
   * Insert the lines of a File in the file.
   * @param pos Position of the new line.
   * @param file File to insert.
   * @exception CPUException if pos > lines
   */
  void insert(size_type pos, const File& file);

  /**
   * Insert the lines of a File at the end of the file.
   * @param file File to insert.
   * @exception CPUException if pos > lines
   */
  void insert(const File& file) {
    this->insert(this->lines(), file);
  }

  /**
   * Remove n lines from the file.
   * @param pos Position of the first line to remove.
   * @param n Number of lines to remove.
   * @exception CPUException if pos > lines or pos + n > lines
   */
  void remove(size_type pos, size_type n);


  /**
   * Load from a file.
   *
   * Before the load, all the lines of the File are removed.
   * @param filename File to open.
   * @exception IOError File can't be opened
   */
  void load(std::string filename);

  /**
   * Save to a file.
   * @param filename File name where to save.
   * @exception IOError File can't be saved
   */
  void save(std::string filename) const;


  /**
   * Get a line.
   * @param pos Position of the line.
   * @exception CPUException if pos > lines
   */
  std::string& operator [](size_type pos) { return this->get_line(pos); }

  /**
   * Get a line.
   * @param pos Position of the line.
   * @exception CPUException if pos > lines
   */
  const std::string& operator [](size_type pos) const
  { return this->get_line(pos); }


  /**
   * Load lines.
   *
   * Before the load, all the lines of the File are removed.
   * @param is From where to get the file
   * @param file Where to load the lines
   * @return Reference to where to get the file
   */
  friend std::istream& operator >>(std::istream& is, File& file);

  /**
   * Print the lines.
   * @param os Where to print the file
   * @param file File to print
   * @return Reference to where to print
   */
  friend std::ostream& operator <<(std::ostream& os, const File& file);

private:
  std::vector<std::string> lines_;
};

}
}

#endif // SIMPLEWORLD_CPU_FILE_HPP
