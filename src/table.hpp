/**
 * @file src/table.hpp
 * Show a SQL query as a table.
 *
 *  Copyright (C) 2008-2010  Xosé Otero <xoseotero@gmail.com>
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

#ifndef SRC_TABLE_HPP
#define SRC_TABLE_HPP

#include <string>

#include <sqlite3.h>

/**
 * Show a SQL query as a table with one column per line.
 * @param showHeader True to show column names in List or Column mode.
 * @param colWidth Requested width of each column when in column mode.
 * @param nullvalue The text to print when a NULL comes back from the database.
 * @param stmt Prepared statement.
 */
void show_query_column(bool showHeader, int colWidth, std::string nullvalue,
                       sqlite3_stmt* stmt);

/**
 * Show a SQL query as a table with one record per line.
 * @param showHeader True to show column names in List or Column mode.
 * @param nullvalue The text to print when a NULL comes back from the database.
 * @param stmt Prepared statement.
 */
void show_query_line(bool showHeader, std::string nullvalue, 
                     sqlite3_stmt* stmt);

#endif // SRC_TABLE_HPP
