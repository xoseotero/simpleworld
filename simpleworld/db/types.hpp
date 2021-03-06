/**
 * @file simpleworld/db/types.hpp
 * Definition of the types used in the database.
 *
 *  Copyright (C) 2007  Xosé Otero <xoseotero@gmail.com>
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

#ifndef SIMPLEWORLD_DB_TYPES_HPP
#define SIMPLEWORLD_DB_TYPES_HPP

#include <sqlite3.h>

namespace simpleworld
{
namespace db
{

typedef sqlite3_int64 ID;

}
}

#endif // SIMPLEWORLD_DB_TYPES_HPP
