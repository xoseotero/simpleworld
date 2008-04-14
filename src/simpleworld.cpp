/**
 * @file src/simpleworld.cpp
 * Simple World
 *
 * begin:     Tue, 02 Oct 2007 08:03:37 +0200
 * last:      $Date$
 *
 *  Copyright (C) 2006-2008  Xosé Otero <xoseotero@users.sourceforge.net>
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
#include <ctime>

#include <getopt.h>

#include <boost/format.hpp>

#include <simpleworld/ints.hpp>
#include <simpleworld/exception.hpp>
#include <simpleworld/worlderror.hpp>
#include <simpleworld/types.hpp>
#include <simpleworld/simpleworld.hpp>
#include <simpleworld/world.hpp>
#include <simpleworld/cpu/types.hpp>
#include <simpleworld/cpu/memory_file.hpp>
#include <simpleworld/db/types.hpp>
#include <simpleworld/db/exception.hpp>
#include <simpleworld/db/environment.hpp>
#include <simpleworld/db/egg.hpp>
#include <simpleworld/db/bug.hpp>
#include <simpleworld/db/deadbug.hpp>
#include <simpleworld/db/food.hpp>
namespace sw = simpleworld;
namespace cpu = simpleworld::cpu;
namespace db = simpleworld::db;

#include "printexc.hpp"

#define DEFAULT_ENERGY 64

// information about he program.
const char* program_short_name = "simpleworld";
const char* program_name = "Simple World";
const char* program_version = "1.0a4";
const char* program_years = "2006-2008";
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
The simulation of a world and the bugs that live on it.\n\
\n\
Mandatory arguments to long options are mandatory for short options too.\n\
  -c, --create               create a void world\n\
  -r, --run=CYCLES           run CYCLES cycles in the world\n\
  -e, --egg=PATH             add a egg to the world\n\
  -f, --food                 add food to the world\n\
  -i, --info                 show information about the world\n\
  -I, --extrainfo            show information about the world and its\n\
                               elements (eggs, bugs and food)\n\
  -C, --code=ID              save the code of Bug[ID] as ID.swo\n\
\n\
      --position=X,Y         position of the element added\n\
                               if not set, a random position is calculated\n\
      --orientation=ORIENTATION\n\
                             orientation of the element added\n\
                               valid values are: north, east, south and west\n\
                               if not set, a random orientation is\n\
                               calculated\n\
      --energy=ENERGY        energy of the element added\n\
                               if not set, the default value is %2%\n\
\n\
  -h, --help                 display this help and exit\n\
  -v, --version              output version information and exit\n\
\n\
Exit status is 0 if OK, 1 if minor problems, 2 if serious trouble.\n\
\n\
Report bugs to <%3%>.")
    % program_short_name
    % DEFAULT_ENERGY
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
static sw::Time cycles = 0;
static std::string egg_path;
static db::ID bug_id;
static sw::Position position;
static sw::Orientation orientation;
static sw::Energy energy = DEFAULT_ENERGY;

// if --position was used
static bool position_set = false;
// if --orientation was used
static bool orientation_set = false;

// commands to do
static bool create_flag = false;
static bool run_flag = false;
static bool egg_flag = false;
static bool food_flag = false;
static bool info_flag = false;
static bool extrainfo_flag = false;
static bool code_flag = false;

/**
 * Parse the command line.
 * @param argc number of parameters.
 * @param argv parameters.
 */
void parse_cmd(int argc, char* argv[])
{
  struct option long_options[] = {
    {"create", no_argument, NULL, 'c'},
    {"run", required_argument, NULL, 'r'},
    {"egg", required_argument, NULL, 'e'},
    {"food", no_argument, NULL, 'f'},
    {"info", no_argument, NULL, 'i'},
    {"extrainfo", no_argument, NULL, 'I'},
    {"code", required_argument, NULL, 'C'},

    {"position", required_argument, NULL, 'p'},
    {"orientation", required_argument, NULL, 'o'},
    {"energy", required_argument, NULL, 'n'},

    {"version", no_argument, NULL, 'v'},
    {"help", no_argument, NULL, 'h'},

    {NULL, 0, NULL, 0}
  };

  // avoid that getopt prints any message
  opterr = 0;

  while (true) {
    /* getopt_long stores the option index here. */
    int option_index = 0;

    int c = getopt_long(argc, argv, "cr:e:fiIC:nvh", long_options,
                        &option_index);

    /* Detect the end of the options. */
    if (c == -1)
      break;

    switch (c)
    {
    case 'c':
      create_flag = true;

      break;

    case 'r':
      run_flag = true;
      {
        char* endptr;
        cycles = std::strtoul(optarg, &endptr, 10);
        if (*endptr != '\0')
          usage(boost::str(boost::format("invalid argument `%1%'")
                           % optarg));
      }

      break;

    case 'e':
      egg_flag = true;
      egg_path = optarg;

      break;

    case 'f':
      food_flag = true;

      break;

    case 'i':
      info_flag = true;

      break;

    case 'I':
      extrainfo_flag = true;

      break;

    case 'C':
      code_flag = true;
      {
        char* endptr;
        bug_id = std::strtoul(optarg, &endptr, 10);
        if (*endptr != '\0')
          usage(boost::str(boost::format("invalid argument `%1%'")
                           % optarg));
      }

      break;

    case 'p':
      if (sscanf(optarg, "%u,%u", &position.x, &position.y) != 2)
        usage(boost::str(boost::format("Invalid value for --position (%1%)")
                         % optarg));
      position_set = true;

      break;

    case 'o':
      if (strcasecmp(optarg, "north") == 0)
        orientation = sw::OrientationNorth;
      else if (strcasecmp(optarg, "east") == 0)
        orientation = sw::OrientationEast;
      else if (strcasecmp(optarg, "south") == 0)
        orientation = sw::OrientationSouth;
      else if (strcasecmp(optarg, "west") == 0)
        orientation = sw::OrientationWest;
      else
        usage(boost::str(boost::format("Invalid value for --orientation (%1%)")
                         % optarg));

      orientation_set = true;

      break;

    case 'n':
      {
        char* endptr;
        energy = std::strtoul(optarg, &endptr, 10);
        if (*endptr != '\0')
          usage(boost::str(boost::format("invalid argument `%1%'")
                           % optarg));
      }

      break;

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

  short commands = create_flag + run_flag + egg_flag + food_flag +
    info_flag + extrainfo_flag + code_flag;
  if (commands == 0)
    usage("a command must be passed");
  else if (commands > 1)
    usage("too many commands passed");

  if (argc == optind)
    usage("a database file is needed");
  else if ((optind + 1) < argc)
    usage("too many database files");

  database_path = argv[optind];
}


/**
 * Show the information about the World.
 * @param environment environment of the World.
 */
void show_world_info(const db::Environment& environment,
                     const sw::World& world)
{
  std::cout << boost::format("\
Size:\t%1% x %2%\n\
Number of elements:\t%3%\n\
Time:\t%4%\n\
\n\
Probability of mutations:\t%5%\n\
Time needed to birth:\t%6%\n\
Multiplier to the attack:\t%7%\n\
\n\
Energy used for doing action NOTHING:\t%8%\n\
Energy used for doing action MYSELF:\t%9%\n\
Energy used for doing action DETECT:\t%10%\n\
Energy used for doing action INFO:\t%11%\n\
Energy used for doing action MOVE:\t%12%\n\
Energy used for doing action TURN:\t%13%\n\
Energy used for doing action ATTACK:\t%14%\n\
Energy used for doing action EAT:\t%15%\n\
Energy used for doing action EGG:\t%16%")
    % environment.size.x
    % environment.size.y
    % world.num_elements()
    % environment.time

    % environment.mutations_probability
    % environment.time_birth
    % environment.attack_multiplier

    % environment.energy_nothing
    % environment.energy_myself
    % environment.energy_detect
    % environment.energy_info
    % environment.energy_move
    % environment.energy_turn
    % environment.energy_attack
    % environment.energy_eat
    % environment.energy_egg
    << std::endl;
}

/**
 * Show the elements of the World.
 * @param world World where the elements live.
 */
void show_world_elements(const sw::World& world)
{
  // a new line to separate the normal info from the extra info
  std::cout << std::endl;

  bool element_found = false;

  sw::Position i = {0, 0};
  sw::Position size = world.size();
  for (i.x = 0; i.x < size.x; i.x++)
    for (i.y = 0; i.y < size.y; i.y++) {
      if (not world.used(i))
        continue;


      sw::Element* element = world[i];

      switch (element->type) {
      case sw::ElementEgg:
        {
          db::Egg* egg = dynamic_cast<db::Egg*>(element);

          std::cout << boost::format("\
Egg[%1%] at (%2%, %3%):\n\
\tOrientation:\t%4%\n\
\tEnergy:\t%5%\n\
\n\
\tBirth:\t%6%\n\
\tFather:\t%7%")
            % egg->id()

            % egg->position.x
            % egg->position.y
            % egg->orientation

            % egg->energy

            % egg->birth
            % egg->father_id

            << std::endl;
        }

        element_found = true;

        break;

      case sw::ElementBug:
        {
          db::Bug* bug = dynamic_cast<db::Bug*>(element);

          std::cout << boost::format("\
Bug[%1%] at (%2%, %3%):\n\
\tOrientation:\t%4%\n\
\tEnergy:\t%5%\n\
\n\
\tBirth:\t%6%\n\
\tFather:\t%7%")
            % bug->id()

            % bug->position.x
            % bug->position.y
            % bug->orientation

            % bug->energy

            % bug->birth
            % bug->father_id

            << std::endl;
        }

        element_found = true;

        break;

      case sw::ElementFood:
        {
          db::Food* food = dynamic_cast<db::Food*>(element);

          std::cout << boost::format("\
Food[%1%] at (%2%, %3%):\n\
\tSize:\t%4%")
            % food->id()

            % food->position.x
            % food->position.y

            % food->size
            << std::endl;
        }

        element_found = true;

        break;

      default:
        std::cout << boost::format("\
Unknown element at (%1%, %2%)")
          % element->position.x
          % element->position.y
          << std::endl;

        element_found = true;

        break;
      }
    }

  if (not element_found)
    std::cout << "The World is empty." << std::endl;
}


sw::Position random_position(const sw::Position& max)
{
  return (sw::Position) {
    float(max.x) * (std::rand() / (RAND_MAX + 1.0)),
    float(max.y) * (std::rand() / (RAND_MAX + 1.0))
  };
}

sw::Orientation random_orientation()
{
  return static_cast<sw::Orientation>(float(4) *
                                      (std::rand() / (RAND_MAX + 1.0)));
}


int main(int argc, char* argv[])
try {
  std::srand(std::time(NULL));

  parse_cmd(argc, argv);

  sw::SimpleWorld simpleworld(database_path);
  const db::Environment& environment = simpleworld.env();
  const sw::World& world = simpleworld.world();

  if (run_flag) {
    simpleworld.run(cycles);
  } else if (egg_flag or food_flag) {
    if (world.num_elements() == (world.size().x * world.size().y)) {
      std::cerr << "The World is full" << std::endl;

      std::exit(1);
    }

    // calculate a random position
    if (not position_set)
      position = random_position(world.size());

    // calculate a random orientation
    if (egg_flag and not orientation_set)
      orientation = random_orientation();

    try {
      if (food_flag) {
        simpleworld.add_food(position, energy);

        std::cout << boost::format("\
Food[%1%] added at (%2%, %3%) with a size of %4%")
          % dynamic_cast<db::Food*>(world[position])->id()
          % position.x
          % position.y
          % energy
          << std::endl;
      } else {
        simpleworld.add_egg(energy, position, orientation,
                            cpu::MemoryFile(egg_path));

        std::cout << boost::format("\
Egg[%1%] added at (%2%, %3%) with a orientation of %4% and a energy of %5%")
          % dynamic_cast<db::Egg*>(world[position])->id()
          % position.x
          % position.y
          % orientation
          % energy
          << std::endl;
      }
    } catch (const sw::WorldError& e) {
      if (world.used(position))
        std::cerr << boost::format("\
Position (%1%, %2%) is already used")
          % position.x
          % position.y
          << std::endl;
      else
        std::cerr << boost::format("\
(%1%, %2%) is outside of the World")
          % position.x
          % position.y
          << std::endl;
      std::exit(1);
    }
  } else if (info_flag or extrainfo_flag) {
    show_world_info(environment, world);

    if (extrainfo_flag)
      show_world_elements(world);
  } else if (code_flag) {
    // the bug can be in several stages: egg, bug or dead bug
    bool bug_found = false;

    try {
      // is the bug a egg?
      db::Egg egg(&simpleworld, bug_id);
      bug_found = true;
      cpu::MemoryFile code(egg.code.code);
      code.save_file(boost::str(boost::format("%1%.swo") % bug_id));
    } catch (const db::DBException& e) {
    }

    try {
      // is the bug a alive bug?
      db::Bug bug(&simpleworld, bug_id);
      bug_found = true;
      cpu::MemoryFile code(bug.code.code);
      code.save_file(boost::str(boost::format("%1%.swo") % bug_id));
    } catch (const db::DBException& e) {
    }

    try {
      // is the bug a dead bug?
      db::DeadBug deadbug(&simpleworld, bug_id);
      bug_found = true;
      cpu::MemoryFile code(deadbug.code.code);
      code.save_file(boost::str(boost::format("%1%.swo") % bug_id));
    } catch (const db::DBException& e) {
    }

    if (not bug_found)
      // no, it was a error
      std::cerr << boost::format("\
Bug[%1%] not found")
        % bug_id
        << std::endl;
  }

  std::exit(EXIT_SUCCESS);
}
catch (const sw::Exception& e) {
  std::cout << e << std::endl;
}
catch (const std::exception& e) {
  std::cout << boost::format("Exception thrown: %1%") % e.what() << std::endl;
}
catch (...) {
  std::cout << "Unknown exception thrown" << std::endl;
}
