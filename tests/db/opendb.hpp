/**
 * @file test/db/opendb.hpp
 * Helper function to open the database.
 *
 *  Copyright (C) 2010  Xosé Otero <xoseotero@gmail.com>
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

#ifndef TEST_DB_OPENDB_HPP
#define TEST_DB_OPENDB_HPP

#include <string>

#include <simpleworld/db/db.hpp>

/**
 * Helper function to open the database.
 * If the database doesn't exists it's created with default values.
 * @param filename the name of the file to open.
 * @return the database.
 * @exception DBException if the database can't be opened/created.
 */
simpleworld::db::DB open_db(std::string filename);

#endif // TEST_DB_OPENDB_HPP
