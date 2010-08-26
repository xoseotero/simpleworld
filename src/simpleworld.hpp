/**
 * @file src/simpleworld.hpp
 * Simple World.
 *
 *  Copyright (C) 2008  Xosé Otero <xoseotero@gmail.com>
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

#ifndef SRC_SIMPLEWORLD_HPP
#define SRC_SIMPLEWORLD_HPP

// information about the program.
extern const char* program_short_name;
extern const char* program_name;
extern const char* program_version;
extern const char* program_years;
extern const char* program_author;
extern const char* program_author_email;
extern const char* program_mailbugs;


/**
 * Simple World create command.
 * @param argc number of parameters.
 * @param argv array of parameters.
 */
void sw_create(int argc, char* argv[]);

/**
 * Simple World run command.
 * @param argc number of parameters.
 * @param argv array of parameters.
 */
void sw_run(int argc, char* argv[]);

/**
 * Simple World vacuum command.
 * @param argc number of parameters.
 * @param argv array of parameters.
 */
void sw_vacuum(int argc, char* argv[]);

/**
 * Simple World info command.
 * @param argc number of parameters.
 * @param argv array of parameters.
 */
void sw_info(int argc, char* argv[]);

/**
 * Simple World env command.
 * @param argc number of parameters.
 * @param argv array of parameters.
 */
void sw_env(int argc, char* argv[]);

/**
 * Simple World egg command.
 * @param argc number of parameters.
 * @param argv array of parameters.
 */
void sw_egg(int argc, char* argv[]);

/**
 * Simple World food command.
 * @param argc number of parameters.
 * @param argv array of parameters.
 */
void sw_food(int argc, char* argv[]);

#endif // SRC_SIMPLEWORLD_HPP
