/**
 * @file src/swupdate.cpp
 * Update the Simple World database.
 *
 * begin:     Fri, 01 Feb 2008 06:12:28 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2008  Xosé Otero <xoseotero@users.sourceforge.net>
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

#include <iostream>
#include <string>
#include <cstdio>
#include <string.h>
#include <cstdlib>

#include <getopt.h>

#include <boost/format.hpp>

#include <sqlite3x.hpp>


// information about he program.
const char* program_short_name = "swupdate";
const char* program_name = "Simple World database udpater";
const char* program_version = "1.0b1-svn";
const char* program_years = "2008";
const char* program_author = "Xosé Otero";
const char* program_author_email = "xoseotero@users.sourceforge.net";
const char* program_mailbugs = "simpleworld-list@lists.sourceforge.net";


/**
 * Show the usage of the program.
 * @param error a text to show as error.
 */
void usage(std::string error)
{
  std::cerr << boost::format(\
"%1%: %2%\n\
Try `%1% --help' for more information.")
    % program_short_name
    % error
    << std::endl;

  std::exit(1);
}

/**
 * Show the help of the program.
 */
void help()
{
  std::cout << boost::format(\
"Usage: %1% [OPTION]... [DATABASE]\n\
Update the Simple World database.\n\
\n\
Mandatory arguments to long options are mandatory for short options too.\n\
  -h, --help                 display this help and exit\n\
  -v, --version              output version information and exit\n\
\n\
Exit status is 0 if OK, 1 if minor problems, 2 if serious trouble.\n\
\n\
Report bugs to <%2%>.")
    % program_short_name
    % program_mailbugs
    << std::endl;
  std::exit(0);
}

/**
 * Show the version of the program.
 */
void version()
{
  std::cout << boost::format(\
"%1% (%2%) %3%\n\
\n\
Copyright (C) %4%, %5% <%6%>.\n\
This is free software. You may redistribute copies of it under the terms of\n\
the GNU General Public License <http://www.gnu.org/licenses/gpl.html>.\n\
There is NO WARRANTY, to the extent permitted by law.")
    % program_short_name
    % program_name
    % program_version
    % program_years
    % program_author
    % program_author_email
    << std::endl;

  std::exit(0);
}


// information from the command line
static std::string database_path;


/**
 * Parse the command line.
 * @param argc number of parameters.
 * @param argv parameters.
 */
void parse_cmd(int argc, char* argv[])
{
  struct option long_options[] = {
    {"version", no_argument, NULL, 'v'},
    {"help", no_argument, NULL, 'h'},

    {NULL, 0, NULL, 0}
  };

  // avoid that getopt prints any message
  opterr = 0;

  while (true) {
    /* getopt_long stores the option index here. */
    int option_index = 0;

    int c = getopt_long(argc, argv, "vh", long_options, &option_index);

    /* Detect the end of the options. */
    if (c == -1)
      break;

    switch (c)
    {
    case 'v':
      version();

      break;

    case 'h':
      help();

      break;

    case '?':
      if (optind <= 1)
        optind++;
      usage(boost::str(boost::format("unrecognized option `%1%'")
                       % argv[optind - 1]));

      break;

    default:
      abort();
    }
  }

  if (argc == optind)
    usage("a database file is needed");
  else if ((optind + 1) < argc)
    usage("too many database files");

  database_path = argv[optind];
}


/**
 * Update the database from version 1 to version 2.
 * @param database database to update.
 */
static void update_1(sqlite3x::sqlite3_connection* connection)
{
  connection->executenonquery("BEGIN TRANSACTION");

  // first change:
  // Environment table has a new primary key (id)
  // a temporary table must be used because a new primary key can't be added
  // all the indexes or triggers are dropped automatically
  connection->executenonquery("\
CREATE TABLE Environment_temp\n\
(\n\
  id INTEGER NOT NULL,\n\
\n\
  time INTEGER NOT NULL,\n\
\n\
  -- the rest of the rows can't change\n\
  size_x INTEGER NOT NULL,\n\
  size_y INTEGER NOT NULL,\n\
\n\
  energy_developed INTEGER NOT NULL,\n\
\n\
  mutations_probability REAL NOT NULL,  -- Values from 0 to 1\n\
\n\
  time_birth INTEGER NOT NULL,\n\
\n\
  energy_nothing INTEGER NOT NULL,\n\
  energy_myself INTEGER NOT NULL,\n\
  energy_detect INTEGER NOT NULL,\n\
  energy_info INTEGER NOT NULL,\n\
  energy_move INTEGER NOT NULL,\n\
  energy_turn INTEGER NOT NULL,\n\
  energy_attack INTEGER NOT NULL,\n\
  energy_eat INTEGER NOT NULL,\n\
  energy_egg INTEGER NOT NULL,\n\
\n\
  PRIMARY KEY(id),\n\
  CHECK(time >= 0),\n\
  CHECK(size_x > 0 AND size_y > 0),\n\
  CHECK(energy_developed >= 0),\n\
  CHECK(mutations_probability >= 0 AND mutations_probability <= 1),\n\
  CHECK(time_birth >= 0),\n\
  CHECK(energy_nothing >= 0 AND energy_myself >= 0 AND\n\
        energy_detect >= 0 AND energy_info >= 0 AND\n\
        energy_move >= 0 AND energy_turn >= 0 AND\n\
        energy_attack >= 0 AND energy_eat >= 0 AND\n\
        energy_egg >= 0)\n\
);");

  // copy the data to the new Environment table
  connection->executenonquery("\
INSERT INTO Environment_temp(time, size_x, size_y,\n\
                             energy_developed, mutations_probability,\n\
                             time_birth,\n\
                             energy_nothing, energy_myself, energy_detect,\n\
                             energy_info, energy_move, energy_turn,\n\
                             energy_attack, energy_eat, energy_egg)\n\
SELECT time, size_x, size_y,\n\
       energy_developed, mutations_probability, time_birth,\n\
       energy_nothing, energy_myself, energy_detect,\n\
       energy_info, energy_move, energy_turn,\n\
       energy_attack, energy_eat, energy_egg\n\
FROM Environment;");
  connection->executenonquery("DROP TABLE Environment;");
  connection->executenonquery("\
ALTER TABLE Environment_temp\n\
RENAME TO Environment;");


  // create the indexes and triggers
  connection->executenonquery("\
CREATE INDEX Environment_index ON Environment(time);");

  connection->executenonquery("\
CREATE TRIGGER Environment_insert_time\n\
BEFORE INSERT\n\
ON Environment\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'There is a older time')\n\
  WHERE (SELECT max(time)\n\
         FROM Environment) > NEW.time;\n\
END;");

  connection->executenonquery("\
CREATE TRIGGER Environment_update_time\n\
BEFORE UPDATE\n\
ON Environment\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'There is a older time')\n\
  WHERE (SELECT max(time)\n\
         FROM Environment) >= NEW.time;\n\
END;");

  connection->executenonquery("\
CREATE TRIGGER Environment_insert\n\
BEFORE INSERT\n\
ON Environment\n\
FOR EACH ROW\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, \"The size of the World can't change\")\n\
  WHERE (SELECT count(id)\n\
         FROM Environment) > 0\n\
        AND\n\
        (SELECT id\n\
         FROM Environment\n\
         WHERE size_x = NEW.size_x AND size_y = NEW.size_y) IS NULL;\n\
END;");

  connection->executenonquery("\
CREATE TRIGGER Environment_update\n\
BEFORE UPDATE\n\
ON Environment\n\
FOR EACH ROW\n\
WHEN OLD.size_x != NEW.size_x OR\n\
     OLD.size_y != NEW.size_y OR\n\
     OLD.energy_developed != NEW.energy_developed OR\n\
     OLD.mutations_probability != NEW.mutations_probability OR\n\
     OLD.time_birth != NEW.time_birth OR\n\
     OLD.energy_nothing != NEW.energy_nothing OR\n\
     OLD.energy_myself != NEW.energy_myself OR\n\
     OLD.energy_detect != NEW.energy_detect OR\n\
     OLD.energy_info != NEW.energy_info OR\n\
     OLD.energy_move != NEW.energy_move OR\n\
     OLD.energy_turn != NEW.energy_turn OR\n\
     OLD.energy_attack != NEW.energy_attack OR\n\
     OLD.energy_eat != NEW.energy_eat OR\n\
     OLD.energy_egg != NEW.energy_egg\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'changed invalid col');\n\
END;");


  // second change:
  // Code table loss the md5 field
  // a temporary table must be used because a field can't be removed
  // all the indexes or triggers are dropped automatically
  connection->executenonquery("\
CREATE TABLE Code_temp\n\
(\n\
  bug_id INTEGER NOT NULL,\n\
\n\
  size INTEGER NOT NULL,\n\
\n\
  code BLOB NOT NULL,\n\
  PRIMARY KEY(bug_id),\n\
  FOREIGN KEY(bug_id) REFERENCES Bug(id),\n\
  CHECK(size >= 0 AND (size % 4 == 0)),\n\
  CHECK(length(code) % 4 == 0),\n\
  CHECK(size == length(code))\n\
);");

  // this trigger must be manually dropped because it was connected to Bug by
  // error
  connection->executenonquery("DROP TRIGGER Code_delete_trigger;");

  // copy the data to the new Code table
  connection->executenonquery("\
INSERT INTO Code_temp(bug_id, size, code)\n\
SELECT bug_id, size, code\n\
FROM Code;");
  connection->executenonquery("DROP TABLE Code;");
  connection->executenonquery("\
ALTER TABLE Code_temp\n\
RENAME TO Code;");

  // create the indexes and triggers
  connection->executenonquery("\
CREATE TRIGGER Code_update_id_trigger\n\
BEFORE UPDATE\n\
ON Code\n\
FOR EACH ROW\n\
WHEN OLD.bug_id != NEW.bug_id\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id changed');\n\
END;");

  connection->executenonquery("\
CREATE TRIGGER Code_insert_valid_trigger\n\
BEFORE INSERT\n\
ON Code\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id of bug not found')\n\
  WHERE (SELECT id\n\
         FROM Bug\n\
         WHERE id=NEW.bug_id AND dead IS NULL) IS NULL;\n\
END;");

  connection->executenonquery("\
CREATE TRIGGER Code_update_valid_trigger\n\
BEFORE UPDATE\n\
ON Code\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id of bug not found')\n\
  WHERE (SELECT id\n\
         FROM Bug\n\
         WHERE id=NEW.bug_id AND dead IS NULL) IS NULL;\n\
END;");

  connection->executenonquery("\
CREATE TRIGGER Code_insert_uniq_trigger\n\
BEFORE INSERT\n\
ON Code\n\
FOR EACH ROW BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id of bug already in table')\n\
  WHERE (SELECT count(bug_id)\n\
         FROM Code\n\
         WHERE bug_id=NEW.bug_id) != 0;\n\
END;");

  connection->executenonquery("\
CREATE TRIGGER Code_update_uniq_trigger\n\
BEFORE UPDATE\n\
ON Code\n\
FOR EACH ROW\n\
WHEN OLD.bug_id != NEW.bug_id\n\
BEGIN\n\
  SELECT RAISE(ROLLBACK, 'id of bug already in table')\n\
  WHERE (SELECT count(bug_id)\n\
         FROM Code\n\
         WHERE bug_id=NEW.bug_id) != 0;\n\
END;");

  connection->executenonquery("\
CREATE TRIGGER Code_delete_trigger\n\
BEFORE DELETE\n\
ON Bug\n\
FOR EACH ROW BEGIN\n\
  DELETE FROM Mutation WHERE bug_id = OLD.id;\n\
END;");


  // third change
  // remove unnecesary indexes
  connection->executenonquery("DROP INDEX IF EXISTS Bug_index;");
  connection->executenonquery("DROP INDEX IF EXISTS Code_index;");
  connection->executenonquery("DROP INDEX IF EXISTS CPU_index;");
  connection->executenonquery("DROP INDEX IF EXISTS Food_index;");


  // all the changes have been done
  connection->executenonquery("PRAGMA user_version = 2;");

  connection->executenonquery("COMMIT;");
}

/**
 * Update the database to the lattest version.
 * @param database database to update.
 */
void update(const std::string& database)
{
  sqlite3x::sqlite3_connection connection(database);
  int version = connection.executeint("PRAGMA user_version;");
  if (version == 0) {
    std::cerr << "This is not a Simple World database" << std::endl;
    std::exit(1);
  }

  if (version < 2)
    update_1(&connection);

  connection.executenonquery("VACUUM;");
}


int main(int argc, char* argv[])
try {
  std::srand(std::time(NULL));

  parse_cmd(argc, argv);
  update(database_path);

  std::exit(EXIT_SUCCESS);
}
catch (const std::exception& e) {
  std::cout << boost::format("Exception thrown: %1%") % e.what() << std::endl;
}
catch (...) {
  std::cout << "Unknown exception thrown" << std::endl;
}
