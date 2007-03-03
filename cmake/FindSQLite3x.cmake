# - Find the SQLite3x includes and libraries.
# The following variables are set if SQLite3x is found.  If SQLite3x is not
# found, SQLite3x_FOUND is set to false.
#  SQLite3x_FOUND        - True when the SQLite3x include directory is found.
#  SQLite3x_INCLUDE_DIRS - the path to where the sqlite3x include files are.
#  SQLite3x_LIBRARIES    - The sqlite3x library.
FIND_PATH(SQLite3x_INCLUDE_DIRS sqlite3x.hpp /usr/include /usr/local/include)
FIND_LIBRARY(SQLite3x_LIBRARIES NAMES sqlite3x PATHS /usr/lib /usr/local/lib)

IF(SQLite3x_INCLUDE_DIRS AND SQLite3x_LIBRARIES)
  SET(SQLite3x_FOUND 1)
  IF(NOT SQLite3x_FIND_QUIETLY)
    MESSAGE(STATUS "Found libsqlite3x: ${SQLite3_LIBRARIES}")
  ENDIF(NOT SQLite3x_FIND_QUIETLY)
ENDIF(SQLite3x_INCLUDE_DIRS AND SQLite3x_LIBRARIES)

MARK_AS_ADVANCED(SQLite3x_INCLUDE_DIRS SQLite3x_LIBRARIES)
