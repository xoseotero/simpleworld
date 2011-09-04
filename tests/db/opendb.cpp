#include "opendb.hpp"
#include <simpleworld/db/exception.hpp>
namespace sw = simpleworld;
namespace db = simpleworld::db;

/**
 * Helper function to open the database.
 * If the database doesn't exists it's created with default values.
 * @param filename the name of the file to open.
 * @return the database.
 * @exception DBException if the database can't be opened/created.
 */
db::DB open_db(std::string filename)
{
  try {
    return db::DB(filename);
  } catch (const db::DBException& e) {
    db::DB::create(filename);
    return db::DB(filename);
  }
}
