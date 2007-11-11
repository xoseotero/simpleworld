/**
 * @file db/code.cpp
 * Code of a bug.
 *
 * begin:     Thu, 01 Mar 2007 13:00:43 +0100
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

#include <cstdio>
#include <cstring>
#include <cassert>

#include <sqlite3x/sqlite3x.hpp>

#include <xyssl/md5.h>

#include <cpu/types.hpp>
#include "code.hpp"
#include "common.hpp"

namespace SimpleWorld
{
namespace DB
{

Code::Code(DB* db, ID bug_id)
  : Table(db, bug_id), code(&this->changed)
{
  this->update();
}

Code::Code(DB* db)
  : Table(db), code(&this->changed)
{
}


/**
 * Search the mutations of the code.
 * @param db database.
 * @param mutations pointer to the mutations.
 * @param bug_id ID of the bug.
 */
static void init_mutations(std::vector<Mutation>* mutations,
                           DB* db, ID bug_id)
{
  mutations->clear();

  sqlite3x::sqlite3_command sql(*db);

  try {
    sql.prepare("\
SELECT id\n\
FROM Mutation\n\
WHERE bug_id = ?;");
    sql.bind(1, bug_id);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    while (cursor.step())
      mutations->push_back(Mutation(db, cursor.getint64(0)));
  } catch (const sqlite3x::database_error& e) {
    throw DBError(__FILE__, __LINE__, e.what());
  }
}

/**
 * Propagate the update() to all the mutations.
 * @param mutations pointer to the mutations.
 */
static void update_mutations(std::vector<Mutation>* mutations)
{
  std::vector<Mutation>::iterator iter = mutations->begin();
  while (iter != mutations->end()) {
    (*iter).update();
    ++iter;
  }
}

/**
 * Propagate the update_db() to all the mutations.
 * @param mutations pointer to the mutations.
 */
static void update_db_mutations(std::vector<Mutation>* mutations)
{
  std::vector<Mutation>::iterator iter = mutations->begin();
  while (iter != mutations->end()) {
    (*iter).update_db();
    ++iter;
  }
}

/**
 * Propagate the insert() to all the mutations.
 * @param mutations pointer to the mutations.
 * @param bug_id ID of the bug.
 */
static void insert_mutations(std::vector<Mutation>* mutations, ID bug_id)
{
  std::vector<Mutation>::iterator iter = mutations->begin();
  while (iter != mutations->end()) {
    (*iter).insert(bug_id);
    ++iter;
  }
}

// It's not needed to propagate the remove() because the database do it
///**
// * Propagate the remove() to all the mutations.
// * @param mutations pointer to the mutations.
// */
//static void update_remove(std::vector<Mutation>* mutations)
//{
//  std::vector<Mutation>::iterator iter = mutations->begin();
//  while (iter != mutations->end()) {
//    (*iter).remove();
//    ++iter;
//  }
//}


void Code::update()
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
SELECT size, md5, code\n\
FROM Code\n\
WHERE bug_id = ?;");
    sql.bind(1, this->id_);

    sqlite3x::sqlite3_cursor cursor(sql.executecursor());
    if (! cursor.step())
      throw IDNotFound(__FILE__, __LINE__);

    this->size = cursor.getint(0);
    this->md5 = cursor.getstring(1);
    this->code = get_memory(&cursor, 2);
    assert(this->code.size() == this->size);

    sqlite3x::sqlite3_command sql_mutations(*this->db_,
                                            "\
SELECT id FROM Mutation\n\
WHERE bug_id = ?;");
    sqlite3x::sqlite3_cursor cursor_mutations(sql_mutations.executecursor());
    while (cursor_mutations.step())
      this->mutations.push_back(Mutation(this->db_,
                                         cursor_mutations.getint64(0)));
  } catch (const sqlite3x::database_error& e) {
    throw DBError(__FILE__, __LINE__, e.what());
  }


  init_mutations(&this->mutations, this->db_, this->id_);
  update_mutations(&this->mutations);

  Table::update();
}

void Code::update_db(bool force)
{
  if (this->changed or force) {
    this->update_md5(force);

    sqlite3x::sqlite3_command sql(*this->db_);

    try {
      sql.prepare("\
UPDATE Code\n\
SET size = ?, md5 = ?, code = ?\n\
WHERE bug_id = ?;");
      sql.bind(1, static_cast<int>(this->size));
      sql.bind(2, this->md5);
      bind_memory(&sql, 3, this->code);
      sql.bind(4, this->id_);

      sql.executenonquery();
    } catch (const sqlite3x::database_error& e) {
      throw DBError(__FILE__, __LINE__, e.what());
    }
  }


  update_db_mutations(&this->mutations);

  Table::update_db(force);
}

void Code::insert(ID bug_id)
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
INSERT INTO Code(bug_id, size, md5, code)\n\
VALUES(?, ?, ?, ?);");
    sql.bind(1, bug_id);
    sql.bind(2, static_cast<int>(this->size));
    sql.bind(3, this->md5);
    bind_memory(&sql, 4, this->code);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw DBError(__FILE__, __LINE__, e.what());
  }


  insert_mutations(&this->mutations, bug_id);

  Table::insert(bug_id);
}

void Code::remove()
{
  sqlite3x::sqlite3_command sql(*this->db_);

  try {
    sql.prepare("\
DELETE FROM Code\n\
WHERE id = ?;");
    sql.bind(1, this->id_);

    sql.executenonquery();
  } catch (const sqlite3x::database_error& e) {
    throw DBError(__FILE__, __LINE__, e.what());
  }


  // It's not needed to propagate the remove() because the database do it
  //remove_mutations(&this->mutations);

  Table::remove();
}


/**
 * Get the MD5 digest: 16 bytes
 * @param code Code to hash.
 * @param digest where to return the MD5 digest.
 */
static void md5_digest(const CPU::Memory& code, unsigned char digest[16])
{
  md5_context ctx;
  md5_starts(&ctx);

  CPU::Address i;
  CPU::Word word;
  for (i = 0; i < code.size(); i += sizeof(CPU::Word)) {
    word = code.get_word(i, false);
    md5_update(&ctx, reinterpret_cast<unsigned char*>(&word),
	       sizeof(CPU::Word));
  }

  md5_finish(&ctx, digest);
}

/**
 * Get the MD5 hexadecimal representation: 32 bytes
 * @param digest MD5 hash.
 * @return the MD5 hexadecimal representation.
 */
static std::string md5_hex(unsigned char digest[16])
{
  std::string md5;
  char hex[3];
  int i;
  for (i = 0; i < 16; i++) {
    std::sprintf(hex, "%02x", digest[i]);
    md5 += hex;
  }

  return md5;
}

void Code::update_md5(bool force)
{
  if (not this->changed and not force)
    return;

  unsigned char digest[16];
  md5_digest(this->code, digest);

  this->md5 = md5_hex(digest);

  this->changed = true;
}

}
}
