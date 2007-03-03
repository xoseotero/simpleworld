/**
 * @file db/mutations.cpp
 * Mutations of a bug.
 *
 * begin:     Thu, 01 Mar 2007 16:59:31 +0100
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2007 Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  License: See COPYING file that comes with this distribution
 */

#include "mutations.h"

namespace SimpleWorld
{
namespace DB
{

Mutations::Mutations(DB* db, ID bug_id) throw (DBError, IDNotFound)
  : Table(db, bug_id)
{
}

Mutations::~Mutations() throw ()
{
  this->db_->free_mutations(this->id_);
}


void Mutations::update() throw (DBError, IDNotFound)
{
  static sqlite3x::sqlite3_command sql(*this->db_,
				       "\
SELECT position, original, mutated\n\
FROM Mutation\n\
WHERE bug_id = ?;");
  sql.bind(1, this->id_);

  try {
    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    Mutation mutation;
    while (cursor.step()) {
      mutation.position = cursor.getint(0);
      mutation.original = cursor.getint(1);
      mutation.mutated = cursor.getint(2);
      if (cursor.isnull(1))
	mutation.type = Mutation::addition;
      else if (cursor.isnull(2))
	mutation.type = Mutation::deletion;
      else
	mutation.type = Mutation::mutation;

      this->mutations.push_back(mutation);
    }

    if (this->mutations.empty())
      throw IDNotFound(__FILE__, __LINE__);
  } catch (sqlite3x::database_error) {
    throw DBError(__FILE__, __LINE__);
  }
}

void Mutations::update_db() throw (DBError)
{
  static sqlite3x::sqlite3_command sql(*this->db_,
				       "\
UPDATE Mutation\n\
SET position = ?, original = ?, mutated = ?\n\
WHERE bug_id = ?;");
  sql.bind(4, this->id_);

  std::vector<Mutation>::iterator iter;
  for (iter = this->mutations.begin();
       iter != this->mutations.end();
       ++iter) {
    sql.bind(1, static_cast<int>((*iter).position));
    if ((*iter).type == Mutation::addition)
      sql.bind(2);
    else
      sql.bind(2, static_cast<int>((*iter).original));
    if ((*iter).type == Mutation::deletion)
      sql.bind(3);
    else
      sql.bind(3, static_cast<int>((*iter).mutated));

    try {
      sql.executenonquery();
    } catch (sqlite3x::database_error) {
      throw DBError(__FILE__, __LINE__);
    }
  }
}

}
}
