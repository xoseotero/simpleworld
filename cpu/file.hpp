/**
 * @file cpu/file.h
 * A file as a array of lines.
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

#ifndef __CPU_FILE_H__
#define __CPU_FILE_H__

#include <vector>
#include <string>
#include <fstream>

#include <simple/types.hpp>
#include <cpu/exception.hpp>

namespace SimpleWorld
{
namespace CPU
{

/**
 * File exception.
 * It's raised if the file don't exist or can't be opened.
 */
class FileAccessError: public std::runtime_error, public CPUException
{
public:
  /**
   * Constructor.
   * @param file File where the exception is raised.
   * @param line Line where the exception is raised.
   * @param filename Name of the file.
   * @param what Reason of the exception.
   */
  FileAccessError(std::string file, Uint32 line, std::string filename,
                  const std::string& what = "Error accessing file")
    throw ()
    : runtime_error(what), CPUException(file, line), filename(filename)
  {}

  /**
   * Destructor.
   */
  ~FileAccessError() throw () {}


  std::string filename;		/**< Name of the file. */
};

/**
 * File exception.
 * It's raised if you try to access a wrond line.
 */
class LineOutOfRange: public std::out_of_range, public CPUException
{
public:
  /**
   * Constructor.
   * @param file File where the exception is raised.
   * @param line Line where the exception is raised.
   * @param fileline Line out of range.
   * @param what Reason of the exception.
   */
  LineOutOfRange(std::string file, Uint32 line,
		 std::vector<std::string>::size_type fileline,
                 const std::string& what = "Line out of range") throw()
    : out_of_range(what), CPUException(file, line), fileline(fileline)
  {}

  /**
   * Destructor.
   */
  ~LineOutOfRange() throw () {}


  std::vector<std::string>::size_type fileline; /**< Line out of range. */
};


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
   * @exception FileAccessError File can't be opened
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
   * @exception LineOutOfRange pos > lines
   */
  std::string& get_line(size_type pos);

  /**
   * Get a line.
   * @param pos Position of the line.
   * @exception LineOutOfRange pos > lines
   */
  const std::string& get_line(size_type pos) const;


  /**
   * Insert a new line in the file.
   * @param pos Position of the new line.
   * @param line Line to insert.
   * @exception LineOutOfRange pos > lines
   */
  void insert(size_type pos, const std::string& line);

  /**
   * Insert the lines of a File in the file.
   * @param pos Position of the new line.
   * @param file File to insert.
   * @exception LineOutOfRange pos > lines
   */
  void insert(size_type pos, const File& file);

  /**
   * Remove n lines from the file.
   * @param pos Position of the first line to remove.
   * @param n Number of lines to remove.
   * @exception LineOutOfRange pos > lines or pos + n > lines
   */
  void remove(size_type pos, size_type n);


  /**
   * Load from a file.
   *
   * Before the load, all the lines of the File are removed.
   * @param filename File to open.
   * @exception FileAccessError File can't be opened
   */
  void load(std::string filename);

  /**
   * Save to a file.
   * @param filename File name where to save.
   * @exception FileAccessError File can't be saved
   */
  void save(std::string filename) const;


  /**
   * Get a line.
   * @param pos Position of the line.
   * @exception LineOutOfRange pos > lines
   */
  std::string& operator [](size_type pos);

  /**
   * Get a line.
   * @param pos Position of the line.
   * @exception LineOutOfRange pos > lines
   */
  const std::string& operator [](size_type pos) const;


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

#endif // __CPU_FILE_H__
