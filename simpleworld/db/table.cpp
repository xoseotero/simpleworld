/**
 * @file simpleworld/db/table.cpp
 * Base class for the tables.
 *
 * begin:     Tue, 13 Feb 2007 11:20:17 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2007  Xosé Otero <xoseotero@users.sourceforge.net>
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

#include "table.hpp"

namespace SimpleWorld
{
namespace DB
{

Table::Table(DB* db, ID id)
  : db_(db), id_(id)
{
  this->update();
}

Table::Table(DB* db)
  : db_(db)
{
}

Table::~Table()
{
}


void Table::update()
{
  this->changed = false;
}

void Table::update_db(bool force)
{
  this->changed = false;
}

void Table::insert()
{
  this->changed = false;
}

void Table::insert(ID id)
{
  this->id_ = id;

  this->changed = false;
}

void Table::remove()
{
  this->changed = false;
}

}
}