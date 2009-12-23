/**
 * @file src/table.cpp
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
 */
void show_query_column(bool showHeader, int colWidth, std::string nullvalue,
                       sqlite3x::sqlite3_cursor cursor)
{
  int cnt = 0;         /**< Number of records displayed so far */
  int actualWidth[100];/**< Actual width of each column */
  for (int i = 0; i < 100; i++)
    actualWidth[i] = 0;
  int i;

  while (cursor.step()) {
    if(cnt++ == 0) {
      for(i = 0; i < cursor.colcount(); i++) {
        int w = colWidth, n;
        if(w <= 0) {
          w = cursor.getcolname(i).size();
          if(w < 10)
            w = 10;
          if (cursor.isnull(i))
            n = nullvalue.size();
          else
            n = cursor.getstring(i).size();
          if(w < n)
            w = n;
        }
        if(i < ArraySize(actualWidth))
          actualWidth[i] = w;
        if(showHeader)
          std::printf("%-*.*s%s", w, w, cursor.getcolname(i).c_str(),
                      i == cursor.colcount() - 1 ? "\n": "  ");
      }
      if(showHeader)
        for(i = 0; i < cursor.colcount(); i++) {
          int w;
          if(i < ArraySize(actualWidth))
            w = actualWidth[i];
          else
            w = 10;
          std::printf("%-*.*s%s", w, w,
                      "-----------------------------------"
                      "----------------------------------------------------------",
                      i == cursor.colcount() - 1 ? "\n": "  ");
        }
    }
    for(i = 0; i < cursor.colcount(); i++) {
      int w;
      if(i < ArraySize(actualWidth))
        w = actualWidth[i];
      else
        w = 10;
      std::printf("%-*.*s%s", w, w,
                  cursor.isnull(i) ?
                  nullvalue.c_str() : cursor.getstring(i).c_str(),
                  i == cursor.colcount() - 1 ? "\n": "  ");
    }
  }
}


/**
 * Create a table with one record per line.
 * @param showHeader True to show column names in List or Column mode.
 * @param nullvalue The text to print when a NULL comes back from the database.
 */
void show_query_line(bool showHeader, std::string nullvalue,
                     sqlite3x::sqlite3_cursor cursor)
{
  int cnt = 0;         /**< Number of records displayed so far */
  int i;

  while (cursor.step()) {
    int w = 5;
    for(i = 0; i < cursor.colcount(); i++) {
      int len = cursor.getcolname(i).size();
      if(len > w)
        w = len;
    }
    if(cnt++ > 0)
      std::printf("\n");
    for(i = 0; i < cursor.colcount(); i++) {
      std::printf("%*s = %s\n", w, cursor.getcolname(i).c_str(),
                  cursor.isnull(i) ?
                  nullvalue.c_str() : cursor.getstring(i).c_str());
    }
  }
}
