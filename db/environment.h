/**
 * @file db/environment.h
 * Information about the environment of the world.
 *
 * begin:     Thu, 25 Jan 2007 03:53:57 +0100
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2007 Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __DB_ENVIRONMENT_H__
#define __DB_ENVIRONMENT_H__

#include <simple/types.h>
#include <db/types.h>
#include <db/table.h>

namespace SimpleWorld
{
namespace DB
{

/*
 * Information about the environment of the world.
 */
class Environment: public Table
{
  friend class DB;

protected:
  /**
   * Constructor.
   * @param db database.
   * @param time time of the environment (the real time can be >= than this).
   * @exception DBError if there is a error in the database.
   * @exception IDNotFound if the time is not found in the table.
   */
  Environment(DB* db, Time time) throw (DBError, IDNotFound);

public:
  /**
   * Destructor.
  */
  ~Environment() throw ();


  // Data
  Time time;

  SortOrder order_world;

  Uint8 mutations_percent;

  Energy energy_detect;
  Energy energy_move;
  Energy energy_push;
  Energy energy_take;
  Energy energy_attack;
  Energy energy_defend;
  Energy energy_msg;
  Energy energy_sex;


  /**
   * Update the data of the class with the database.
   * @exception DBError if there is a error in the database.
   * @execption IDNotFound if the ID is not found in the table.
   */
  void update() throw (DBError, IDNotFound);

  /**
   * Update the database with the data of the class.
   * @exception DBError if there is a error in the database.
   */
  void update_db() throw (DBError);
};

}
}

#endif // __DB_ENVIRONMENT_H__
