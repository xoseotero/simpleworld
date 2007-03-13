/**
 * @file db/cpu.h
 * CPU of a bug.
 *
 * begin:     Mon, 01 Jan 2007 08:46:13 +0100
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2007, Xosé Otero <xoseotero@users.sourceforge.net>
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

#ifndef __DB_CPU_H__
#define __DB_CPU_H__

#include <string>

#include <db/types.hpp>
#include <db/db.hpp>
#include <db/table.hpp>

namespace SimpleWorld
{
namespace DB
{

/**
 * CPU of a bug.
 */     
class CPU: public Table
{
  friend class DB;
protected:
  /**
   * Constructor.
   * @param db database.
   * @param bug_id id of the bug.
   * @exception DBError if there is a error in the database.
   * @exception IDNotFound if the ID is not found in the table.
   */
  CPU(DB* db, ID bug_id);

public:
  /**
   * Destructor.
   */
  ~CPU();


  // Data
  Word r0;
  Word r1;
  Word r2;
  Word r3;
  Word r4;
  Word r5;
  Word r6;
  Word r7;
  Word r8;
  Word r9;
  Word r10;
  Word r11;
  Word r12;
  Word pc;
  Word sp;
  Word etp;


  /**
   * Update the data of the class with the database.
   * @exception DBError if there is a error in the database.
   * @execption IDNotFound if the ID is not found in the table.
   */
  void update();

  /**
   * Update the database with the data of the class.
   * @exception DBError if there is a error in the database.
   */
  void update_db();
};

}
}

#endif // __DB_CPU_H__
