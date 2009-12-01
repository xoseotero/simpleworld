/**
 * @file src/simpleworld/table.hpp
 * Show a SQL query as a table.
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

#include <string>

#include <sqlite3x.hpp>

/**
 * Show a SQL query as a table with one column per line.
 * @param showHeader True to show column names in List or Column mode.
 * @param nullvalue The text to print when a NULL comes back from the database.
 */
void show_query_column(bool showHeader, int colWidth, std::string nullvalue,
                       sqlite3x::sqlite3_cursor cursor);

/**
 * Show a SQL query as a table with one record per line.
 * @param showHeader True to show column names in List or Column mode.
 * @param colWidth Requested width of each column when in column mode.
 * @param nullvalue The text to print when a NULL comes back from the database.
 */
void show_query_line(bool showHeader, std::string nullvalue,
                     sqlite3x::sqlite3_cursor cursor);
