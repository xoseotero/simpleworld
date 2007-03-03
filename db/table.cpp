/**
 * @file db/table.cpp
 * Base class for the tables.
 *
 * begin:     Tue, 13 Feb 2007 11:20:17 +0100
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2007 Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  License: See COPYING file that comes with this distribution
 */

#include "table.h"

namespace SimpleWorld
{
namespace DB
{

Table::Table(DB* db, ID id) throw (DBError, IDNotFound)
  : db_(db), id_(id)
{
  this->update();
}

}
}
