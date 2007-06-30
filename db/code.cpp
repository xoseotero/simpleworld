/**
 * @file db/code.cpp
 * Code of a bug.
 *
 * begin:     Thu, 01 Mar 2007 13:00:43 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2007 Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  License: See COPYING file that comes with this distribution
 */

#include "code.hpp"

#include <cstring>

#include <boost/scoped_array.hpp>

#include <sqlite3x.hpp>

namespace SimpleWorld
{
namespace DB
{

Code::Code(DB* db, ID bug_id)
  : Table(db, bug_id)
{
}

Code::~Code()
{
  this->db_->free_code(this->id_);
}


void Code::update()
{
  static sqlite3x::sqlite3_command sql(*this->db_,
				       "\
SELECT size, md5, code\n\
FROM Code\n\
WHERE bug_id = ?;");
  sql.bind(1, this->id_);

  try {
    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw IDNotFound(__FILE__, __LINE__);

    this->size = cursor.getint(0);
    this->md5 = cursor.getstring(1);

    int size;
    Word const *blob = static_cast<const Word*>(cursor.getblob(2, size));
    int i;
    for (i = 0; i < size; i += 4)
      this->code.push_back(blob[i]);
  } catch (sqlite3x::database_error) {
    throw DBError(__FILE__, __LINE__);
  }
}

void Code::update_db()
{
  static sqlite3x::sqlite3_command sql(*this->db_,
				       "\
UPDATE Code\n\
SET size = ?, md5 = ?, code = ?\n\
WHERE bug_id = ?;");
  sql.bind(1, static_cast<int>(this->size));
  sql.bind(2, this->md5);

  boost::scoped_array<Word> blob(new Word[this->size]);
  int i;
  for (i = 0; i < (this->size / 4); i++)
    blob[i] = this->code[i];
  sql.bind(3, blob.get(), this->size);
  
  sql.bind(4, this->id_);

  try {
    sql.executenonquery();
  } catch (sqlite3x::database_error) {
    throw DBError(__FILE__, __LINE__);
  }
}

}
}
