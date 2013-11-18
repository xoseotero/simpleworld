/**
* @file simpleworld/db/default.cpp
* Default values of the Environment.
*
*  Copyright (C) 2011-2013  Xosé Otero <xoseotero@gmail.com>
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

#include "default.hpp"


#define DEFAULT_SIZE_X 16
#define DEFAULT_SIZE_Y 16
#define DEFAULT_TIME_ROT 64
#define DEFAULT_SIZE_ROT 64
#define DEFAULT_MUTATIONS_PROBABILITY 0.001
#define DEFAULT_TIME_BIRTH 2
#define DEFAULT_TIME_MUTATE 1024
#define DEFAULT_TIME_LAZINESS 64
#define DEFAULT_ENERGY_LAZINESS 64
#define DEFAULT_ATTACK_MULTIPLER 2.5
#define DEFAULT_TIME_NOTHING 1
#define DEFAULT_TIME_MYSELF 1
#define DEFAULT_TIME_DETECT 1
#define DEFAULT_TIME_INFO 1
#define DEFAULT_TIME_MOVE 2
#define DEFAULT_TIME_TURN 2
#define DEFAULT_TIME_ATTACK 2
#define DEFAULT_TIME_EAT 2
#define DEFAULT_TIME_EGG 2
#define DEFAULT_ENERGY_NOTHING 0
#define DEFAULT_ENERGY_MYSELF 1
#define DEFAULT_ENERGY_DETECT 1
#define DEFAULT_ENERGY_INFO 1
#define DEFAULT_ENERGY_MOVE 2
#define DEFAULT_ENERGY_TURN 2
#define DEFAULT_ENERGY_ATTACK 5
#define DEFAULT_ENERGY_EAT 3
#define DEFAULT_ENERGY_EGG 10


namespace simpleworld
{
namespace db
{

DefaultEnvironment default_environment = { DEFAULT_SIZE_X,
                                           DEFAULT_SIZE_Y,

                                           DEFAULT_TIME_ROT,
                                           DEFAULT_SIZE_ROT,

                                           DEFAULT_MUTATIONS_PROBABILITY,
                                           DEFAULT_TIME_BIRTH,
                                           DEFAULT_TIME_MUTATE,

                                           DEFAULT_TIME_LAZINESS,
                                           DEFAULT_ENERGY_LAZINESS,

                                           DEFAULT_ATTACK_MULTIPLER,

                                           DEFAULT_TIME_NOTHING,
                                           DEFAULT_TIME_MYSELF,
                                           DEFAULT_TIME_DETECT,
                                           DEFAULT_TIME_INFO,
                                           DEFAULT_TIME_MOVE,
                                           DEFAULT_TIME_TURN,
                                           DEFAULT_TIME_ATTACK,
                                           DEFAULT_TIME_EAT,
                                           DEFAULT_TIME_EGG,

                                           DEFAULT_ENERGY_NOTHING,
                                           DEFAULT_ENERGY_MYSELF,
                                           DEFAULT_ENERGY_DETECT,
                                           DEFAULT_ENERGY_INFO,
                                           DEFAULT_ENERGY_MOVE,
                                           DEFAULT_ENERGY_TURN,
                                           DEFAULT_ENERGY_ATTACK,
                                           DEFAULT_ENERGY_EAT,
                                           DEFAULT_ENERGY_EGG };

}
}
