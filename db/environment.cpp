/**
 * @file db/environment.cpp
 * Information about the environment of the world.
 *
 * begin:     Fri, 26 Jan 2007 06:29:35 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2007 Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  License: See COPYING file that comes with this distribution
 */

#include "environment.hpp"

namespace SimpleWorld
{
namespace DB
{

Environment::Environment(DB* db, Time time)
  : Table(db, time)
{
}

Environment::~Environment()
{
  this->db_->free_environment(this->id_);
}


void Environment::update()
{
  static sqlite3x::sqlite3_command sql(*this->db_,
				       "\
SELECT time, order_world, mutations_percent,\n\
       energy_detect, energy_move, energy_push, energy_take,\n\
       energy_attack, energy_defend, energy_msg, energy_sex\n\
FROM Environment\n\
WHERE time >= ?\n\
ORDER BY time\n\
LIMIT 1;");
  sql.bind(1, this->id_);

  try {
    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw IDNotFound(__FILE__, __LINE__);

    this->id_ = this->time = cursor.getint(0);
    this->order_world = cursor.getint(1);
    this->mutations_percent = cursor.getint(2);
    this->energy_detect = cursor.getint(3);
    this->energy_move = cursor.getint(4);
    this->energy_push = cursor.getint(5);
    this->energy_take = cursor.getint(6);
    this->energy_attack = cursor.getint(7);
    this->energy_defend = cursor.getint(8);
    this->energy_msg = cursor.getint(9);
    this->energy_sex = cursor.getint(10);
  } catch (sqlite3x::database_error) {
    throw DBError(__FILE__, __LINE__);
  }
}

void Environment::update_db()
{
  static sqlite3x::sqlite3_command sql(*this->db_,
				       "\
UPDATE Environment\n\
SET time = ?, order_world = ?, mutations_percent = ?,\n\
    energy_detect = ?, energy_move = ?,\n\
    energy_push = ?, energy_take = ?,\n\
    energy_attack = ?, energy_defend = ?\n\
    energy_msg = ?, energy_sex = ?\n\
WHERE time = ?;");
  sql.bind(1, static_cast<int>(this->time));
  sql.bind(2, static_cast<int>(this->order_world));
  sql.bind(3, static_cast<int>(this->mutations_percent));
  sql.bind(4, static_cast<int>(this->energy_detect));
  sql.bind(5, static_cast<int>(this->energy_move));
  sql.bind(6, static_cast<int>(this->energy_push));
  sql.bind(7, static_cast<int>(this->energy_take));
  sql.bind(8, static_cast<int>(this->energy_attack));
  sql.bind(9, static_cast<int>(this->energy_defend));
  sql.bind(10, static_cast<int>(this->energy_msg));
  sql.bind(11, static_cast<int>(this->energy_sex));
  sql.bind(12, this->id_);

  try {
    sql.executenonquery();
  } catch (sqlite3x::database_error) {
    throw DBError(__FILE__, __LINE__);
  }

  this->id_ = this->time;
}

}
}
