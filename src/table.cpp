/**
 * @file src/table.cpp
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

// This code is heavily based on the work done by D. Richard Hipp in the
// callback() function found in the file shell.c of SQLite3
// (http://sqlite.org/).
// As SQLite3 is in the public domain and, there isn't any copyright text
// that can be added here. Instead, its header is added.

/*
** 2001 September 15
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** This file contains code to implement the "sqlite" command line
** utility for accessing SQLite databases.
**
** $Id$
*/

#include <cstdio>

#include "table.hpp"


/*
** Number of elements in an array
*/
#define ArraySize(X)  (sizeof(X)/sizeof(X[0]))


/**
 * Create a table with one column per line.
 * @param showHeader True to show column names in List or Column mode.
 * @param colWidth Requested width of each column when in column mode.
 * @param nullvalue The text to print when a NULL comes back from the database.
 * @param stmt Prepared statement.
 */
void show_query_column(bool showHeader, int colWidth, std::string nullvalue,
                       sqlite3_stmt* stmt)
{
  int cnt = 0;         /**< Number of records displayed so far */
  int actualWidth[100];/**< Actual width of each column */
  for (int i = 0; i < 100; i++)
    actualWidth[i] = 0;
  int i;

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    if(cnt++ == 0) {
      for(i = 0; i < sqlite3_column_count(stmt); i++) {
        int w = colWidth, n;
        if(w <= 0) {
          w = sqlite3_column_bytes(stmt, i);
          if(w < 10)
            w = 10;
          if (sqlite3_column_type(stmt, i) == SQLITE_NULL)
            n = nullvalue.size();
          else
            n = sqlite3_column_bytes(stmt, i);
          if(w < n)
            w = n;
        }
        if(i < ArraySize(actualWidth))
          actualWidth[i] = w;
        if(showHeader)
          std::printf("%-*.*s%s", w, w, sqlite3_column_name(stmt, i),
                      i == sqlite3_column_count(stmt) - 1 ? "\n": "  ");
      }
      if(showHeader)
        for(i = 0; i < sqlite3_column_count(stmt); i++) {
          int w;
          if(i < ArraySize(actualWidth))
            w = actualWidth[i];
          else
            w = 10;
          std::printf("%-*.*s%s", w, w,
                      "-----------------------------------"
                      "----------------------------------------------------------",
                      i == sqlite3_column_count(stmt) - 1 ? "\n": "  ");
        }
    }
    for(i = 0; i < sqlite3_column_count(stmt); i++) {
      int w;
      if(i < ArraySize(actualWidth))
        w = actualWidth[i];
      else
        w = 10;
      std::printf("%-*.*s%s", w, w,
                  sqlite3_column_type(stmt, i) == SQLITE_NULL ?
                  nullvalue.c_str() : reinterpret_cast<const char*>(sqlite3_column_text(stmt, i)),
                  i == sqlite3_column_count(stmt) - 1 ? "\n": "  ");
    }
  }
}


/**
 * Create a table with one record per line.
 * @param showHeader True to show column names in List or Column mode.
 * @param nullvalue The text to print when a NULL comes back from the database.
 * @param stmt Prepared statement.
 */
void show_query_line(bool showHeader, std::string nullvalue,
                     sqlite3_stmt* stmt)
{
  int cnt = 0;         /**< Number of records displayed so far */
  int i;

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    int w = 5;
    for(i = 0; i < sqlite3_column_count(stmt); i++) {
      int len = sqlite3_column_bytes(stmt, i);
      if(len > w)
        w = len;
    }
    if(cnt++ > 0)
      std::printf("\n");
    for(i = 0; i < sqlite3_column_count(stmt); i++) {
      std::printf("%*s = %s\n", w, sqlite3_column_name(stmt, i),
                  sqlite3_column_type(stmt, i) == SQLITE_NULL ?
                  nullvalue.c_str() : reinterpret_cast<const char*>(sqlite3_column_text(stmt, i)));
    }
  }
}
