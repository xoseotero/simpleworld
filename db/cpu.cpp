/**
 * @file db/cpu.cpp
 * CPU of a bug.
 *
 * begin:     Mon, 01 Jan 2007 09:02:47 +0100
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2007, Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  License: See COPYING file that comes with this distribution
 */

#include "cpu.h"

namespace SimpleWorld
{
namespace DB
{

CPU::CPU(DB* db, ID bug_id)
  : Table(db, bug_id)
{
}

CPU::~CPU()
{
  this->db_->free_cpu(this->id_);
}


void CPU::update()
{
  static sqlite3x::sqlite3_command sql(*this->db_,
				       "\
SELECT r0, r1, r2, r3, r4, r5, r6, r7,\n\
       r8, r9, r10, r11, r12, pc, sp, etp\n\
FROM CPU\n\
WHERE bug_id = ?;");
  sql.bind(1, this->id_);

  try {
    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw IDNotFound(__FILE__, __LINE__);

    this->r0 = cursor.getint(0);
    this->r1 = cursor.getint(1);
    this->r2 = cursor.getint(2);
    this->r3 = cursor.getint(3);
    this->r4 = cursor.getint(4);
    this->r5 = cursor.getint(5);
    this->r6 = cursor.getint(6);
    this->r7 = cursor.getint(7);
    this->r8 = cursor.getint(8);
    this->r9 = cursor.getint(9);
    this->r10 = cursor.getint(10);
    this->r11 = cursor.getint(11);
    this->r12 = cursor.getint(12);
    this->pc = cursor.getint(13);
    this->sp = cursor.getint(14);
    this->etp = cursor.getint(15);
  } catch (sqlite3x::database_error) {
    throw DBError(__FILE__, __LINE__);
  }
}

void CPU::update_db()
{
  static sqlite3x::sqlite3_command sql(*this->db_,
				       "\
UPDATE CPU\n\
SET r0 = ?, r1 = ?, r2 = ?, r3 = ?,\n\
    r4 = ?, r5 = ?, r6 = ?, r7 = ?,\n\
    r8 = ?, r9 = ?, r10 = ?, r11 = ?,\n\
    r12 = ?, pc = ?, sp = ?, etp = ?\n\
WHERE bug_id = ?;");
  sql.bind(1, static_cast<int>(this->r0));
  sql.bind(2, static_cast<int>(this->r1));
  sql.bind(3, static_cast<int>(this->r2));
  sql.bind(4, static_cast<int>(this->r3));
  sql.bind(5, static_cast<int>(this->r4));
  sql.bind(6, static_cast<int>(this->r5));
  sql.bind(7, static_cast<int>(this->r6));
  sql.bind(8, static_cast<int>(this->r7));
  sql.bind(9, static_cast<int>(this->r8));
  sql.bind(10, static_cast<int>(this->r9));
  sql.bind(11, static_cast<int>(this->r10));
  sql.bind(12, static_cast<int>(this->r11));
  sql.bind(13, static_cast<int>(this->r12));
  sql.bind(14, static_cast<int>(this->pc));
  sql.bind(15, static_cast<int>(this->sp));
  sql.bind(16, static_cast<int>(this->etp));
  sql.bind(17, this->id_);

  try {
    sql.executenonquery();
  } catch (sqlite3x::database_error) {
    throw IDNotFound(__FILE__, __LINE__);
  }
}

}
}
