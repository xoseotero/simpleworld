/**
 * @file db/db.h
 * Simple World Data Base management.
 *
 * begin:     Wed, 24 Jan 2007 17:13:02 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2007 Xosé Otero <xoseotero@users.sourceforge.net>
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

#ifndef __DB_DB_H__
#define __DB_DB_H__

#include <vector>
#include <map>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <sqlite3x.hpp>

#include <simple/types.hpp>
#include <db/types.hpp>
#include <db/exception.hpp>

namespace SimpleWorld
{
/**
 * Namespace for Simple World Data Base.
 */
namespace DB
{

/**
 * DB exception.
 * It's raised if the a error in the database happen.
 */
class DBError: public std::runtime_error, public DBException
{
  /**
   * Constructor.
   * @param file File where the exception is raised.
   * @param line Line where the exception is raised.
   * @param what Reason of the exception.
   */
public:
  DBError(std::string file = "", Uint32 line = 0,
          const std::string& what = "Database Error") throw()
    : runtime_error(what), DBException(file, line)
  {}

  /**
   * Destructor.
   */
  ~DBError() throw () {}
};

/**
 * DB exception.
 * It's raised if the version of the database is not supported.
 */
class VersionNotSupported: public std::runtime_error, public DBException
{
  /**
   * Constructor.
   * @param file File where the exception is raised.
   * @param line Line where the exception is raised.
   * @param what Reason of the exception.
   */
public:
  VersionNotSupported(std::string file = "", Uint32 line = 0,
		      const std::string& what =
		      "Database version not supported.")
    throw()
    : runtime_error(what), DBException(file, line)
  {}

  /**
   * Destructor.
   */
  ~VersionNotSupported() throw () {}
};

/**
 * Table exception.
 * It's raised if the ID is not found in the table.
 */
class IDNotFound: public std::runtime_error, public DBException
{
  /**
   * Constructor.
   * @param file File where the exception is raised.
   * @param line Line where the exception is raised.
   * @param what Reason of the exception.
   */
public:
  IDNotFound(std::string file = "", Uint32 line = 0,
	     const std::string& what = "ID not found") throw()
    : runtime_error(what), DBException(file, line)
  {}

  /**
   * Destructor.
   */
  ~IDNotFound() throw () {}
};


class Bug;
class Hierarchy;
class Code;
class CPU;
class Mutations;
class Environment;


/**
 * Simple World Data Base management.
 */
class DB: public sqlite3x::sqlite3_connection
{
public:
  /**
   * Constructor.
   * @param filename File name of the database.
   * @exception SQLiteError The database can't be opened.
   * @exception DBError if there is a error in the database.
   * @exception VersionNotSupported if the database version is not supported.
   */
  DB(std::string filename);


  /**
   * Version of the data base.
   * @return The version.
   */
  Uint8 version() const { return this->version_; }


  // Bug management

  /**
   * List of all the bugs, ordered by it's birth.
   * @return the list of bugs.
   * @exception DBError if there is a error in the database.
   */
  std::vector<ID> bugs();

  /**
   * List of the alive bugs, ordered by its world_order.
   * @return the list of bugs.
   * @exception DBError if there is a error in the database.
   */
  std::vector<ID> alive_bugs();

  /**
   * Get a bug.
   * @param id id of the bug
   * @return the Bug.
   * @exception DBError if there is a error in the database.
   * @exception IDNotFound if the ID is not found in the table.
   */
  boost::shared_ptr<Bug> bug(ID id);

  /**
   * Free the memory used by the bug.
   * @param id id of the bug.
   * @exception IDNotFound if the ID is not found in the table.
   */
  void free_bug(ID id);

  /**
   * Add a new bug to the database.
   * @param order order of the bug in the world.
   * @param energy energy of the bug.
   * @param x position of the bug (x coord)
   * @param y position of the bug (y coord)
   * @param orientation orientation of the bug (0 <= orientation <= 3)
   * @param birth birthday
   * @return the ID of the created bug.
   * @exception DBError if there is a error in the database.
   */
  ID add_bug(SortOrder order, Energy energy,
	     Position x, Position y, Orientation orientation,
	     Time birth);

  /**
   * Delete a bug from the database.
   * @param bug_id id of the bug.
   * @exception IDNotFound if the ID is not found in the table.
   */
  void del_bug(ID id);


  // Hierarchy management

  /**
   * Get the hierarchy of a bug.
   * @param bug_id id of the bug.
   * @return the Hierarchy.
   * @exception DBError if there is a error in the database.
   * @exception IDNotFound if the ID is not found in the table.
   */
  boost::shared_ptr<Hierarchy> hierarchy(ID bug_id);

  /**
   * Free the memory used by the hierarchy.
   * @param bug_id id of the bug.
   * @exception IDNotFound if the ID is not found in the table.
   */
  void free_hierarchy(ID bug_id);


  // Code management

  /**
   * Get the code of a bug.
   * @param bug_id id of the bug.
   * @return the Code.
   * @exception DBError if there is a error in the database.
   * @exception IDNotFound if the ID is not found in the table.
   */
  boost::shared_ptr<Code> code(ID bug_id);

  /**
   * Free the memory used by the code.
   * @param bug_id id of the bug.
   * @exception IDNotFound if the ID is not found in the table.
   */
  void free_code(ID bug_id);


  // CPU management

  /**
   * Get the CPU of a bug.
   * @param bug_id id of the bug.
   * @return the Code.
   * @exception DBError if there is a error in the database.
   * @exception IDNotFound if the ID is not found in the table.
   */
  boost::shared_ptr<CPU> cpu(ID bug_id);

  /**
   * Free the memory used by the CPU.
   * @param bug_id id of the bug.
   * @exception IDNotFound if the ID is not found in the table.
   */
  void free_cpu(ID bug_id);


  // Mutations management

  /**
   * Get the mutations of a bug.
   * @param bug_id id of the bug.
   * @return the Code.
   * @exception DBError if there is a error in the database.
   * @exception IDNotFound if the ID is not found in the table.
   */
  boost::shared_ptr<Mutations> mutations(ID bug_id);

  /**
   * Free the memory used by the mutations.
   * @param bug_id id of the bug.
   * @exception IDNotFound if the ID is not found in the table.
   */
  void free_mutations(ID bug_id);


  // Environment management

  /**
   * List of all the environments (changes), ordered by it's time.
   * @return the list of environments.
   * @exception DBError if there is a error in the database.
   */
  std::vector<Time> environments();

  /**
   * Get the last environment (change).
   * @return the environment.
   * @exception DBError if there is a error in the database.
   * @exception IDNotFound if there is any environments.
   */
  Time last_environment();

  /**
   * Get a environment.
   * @param time time of the environment (the real time can be >= than this).
   * @return the Environment.
   * @exception DBError if there is a error in the database.
   * @exception IDNotFound if the ID is not found in the table.
   */
  boost::shared_ptr<Environment> environment(Time time);

  /**
   * Free the memory used by the environment.
   * @param time time of the environment.
   * @exception IDNotFound if the ID is not found in the table.
   */
  void free_environment(Time time);

  /**
   * Add a new environment to the database.
   *
   * order_world will be 0.
   * @param time time.
   * @param mutations_percent percent of mutations.
   * @param energy_detect energy needed for detect.
   * @param energy_move energy needed for move.
   * @param energy_push energy needed for push something.
   * @param energy_take energy needed for take something.
   * @param energy_attack energy needed for attack.
   * @param energy_defend energy needed for defend.
   * @param energy_msg energy needed for a message.
   * @param energy_sex energy needed for sex.
   * @return the ID of the created bug.
   * @exception DBError if there is a error in the database.
   */
  Time add_environment(World::Time time, Uint8 mutations_percent,
		       Energy energy_detect, Energy energy_move,
		       Energy energy_push, Energy energy_take,
		       Energy energy_attack, Energy energy_defend,
		       Energy energy_msg, Energy energy_sex);

  /**
   * Delete a environment from the database.
   * @param time time of the environment.
   * @exception IDNotFound if the ID is not found in the table.
   */
  void del_environment(Time time);

protected:
  /**
   * This function is called when open() succeeds. Subclasses
   * which wish to do custom db initialization or sanity checks
   * may do them here.
   */
  void on_open();

  /**
   * Create the tables.
   * @exception DBError The tables can't be created.
   */
  void create_tables();

private:
  Uint8 version_;

  std::map<ID, boost::weak_ptr<Bug> > bug_map_;
  std::map<ID, boost::weak_ptr<Hierarchy> > hierarchy_map_;
  std::map<ID, boost::weak_ptr<Code> > code_map_;
  std::map<ID, boost::weak_ptr<CPU> > cpu_map_;
  std::map<ID, boost::weak_ptr<Mutations> > mutations_map_;
  std::map<Time, boost::weak_ptr<Environment> > environment_map_;
};

}
}

#endif // __DB_DB_H__
