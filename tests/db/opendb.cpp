#include "opendb.hpp"
#include <simpleworld/db/exception.hpp>
namespace sw = simpleworld;
namespace db = simpleworld::db;

// Default values
#define DEFAULT_SIZE sw::Position(16, 16)
#define DEFAULT_ROT 64
#define DEFAULT_SROT 64
#define DEFAULT_MUTATIONS 0.01
#define DEFAULT_BIRTH 2
#define DEFAULT_OLD 1024
#define DEFAULT_LAZINESS 64
#define DEFAULT_ELAZINESS 64
#define DEFAULT_MULTIPLIER 2.5
#define DEFAULT_NOTHING 0
#define DEFAULT_MYSELF 1
#define DEFAULT_DETECT 1
#define DEFAULT_INFO 1
#define DEFAULT_MOVE 2
#define DEFAULT_TURN 1
#define DEFAULT_ATTACK 3
#define DEFAULT_EAT 3
#define DEFAULT_EGG 4

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
    db::DB::create(filename, 0, DEFAULT_SIZE.x, DEFAULT_SIZE.y,
		   DEFAULT_ROT, DEFAULT_SROT, DEFAULT_MUTATIONS,
		   DEFAULT_BIRTH, DEFAULT_OLD, DEFAULT_LAZINESS,
		   DEFAULT_ELAZINESS, DEFAULT_MULTIPLIER, DEFAULT_NOTHING,
		   DEFAULT_MYSELF, DEFAULT_DETECT, DEFAULT_INFO,
		   DEFAULT_MOVE, DEFAULT_TURN, DEFAULT_ATTACK,
                   DEFAULT_EAT, DEFAULT_EGG);
    return db::DB(filename);
  }
}
