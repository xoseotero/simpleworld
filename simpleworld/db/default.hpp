/**
* @file simpleworld/db/default.hpp
* Default values of the Environment.
*
*  Copyright (C) 2011  Xosé Otero <xoseotero@gmail.com>
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

#ifndef SIMPLEWORLD_DB_DEFAULT_HPP
#define SIMPLEWORLD_DB_DEFAULT_HPP

#include <simpleworld/types.hpp>

namespace simpleworld
{
namespace db
{

/**
 * Default values of the Environment.
 */
struct DefaultEnvironment
{
  Coord size_x;
  Coord size_y;

  Time time_rot;
  Energy size_rot;

  double mutations_probability;
  Time time_birth;
  Time time_mutate;

  Time time_laziness;
  Energy energy_laziness;

  double attack_multiplier;

  Time time_nothing;
  Time time_myself;
  Time time_detect;
  Time time_info;
  Time time_move;
  Time time_turn;
  Time time_attack;
  Time time_eat;
  Time time_egg;

  Energy energy_nothing;
  Energy energy_myself;
  Energy energy_detect;
  Energy energy_info;
  Energy energy_move;
  Energy energy_turn;
  Energy energy_attack;
  Energy energy_eat;
  Energy energy_egg;
};

extern DefaultEnvironment default_environment;

}
}

#endif // SIMPLEWORLD_DB_DEFAULT_HPP
