# - Find the SQLite3 includes and libraries.
# The following variables are set if SQLite3 is found.  If SQLite3 is not
# found, SQLite3_FOUND is set to false.
#  SQLite3_FOUND        - True when the SQLite3 include directory is found.
#  SQLite3_INCLUDE_DIRS - the path to where the sqlite3 include files are.
#  SQLite3_LIBRARIES    - The sqlite3 library.
FIND_PATH(SQLite3_INCLUDE_DIRS sqlite3.h /usr/include /usr/local/include)
FIND_LIBRARY(SQLite3_LIBRARIES NAMES sqlite3 PATHS /usr/lib /usr/local/lib)

IF(SQLite3_INCLUDE_DIRS AND SQLite3_LIBRARIES)
  SET(SQLite3_FOUND 1)
  IF(NOT SQLite3_FIND_QUIETLY)
    MESSAGE(STATUS "Found libsqlite3: ${SQLite3_LIBRARIES}")
  ENDIF(NOT SQLite3_FIND_QUIETLY)
ENDIF(SQLite3_INCLUDE_DIRS AND SQLite3_LIBRARIES)

MARK_AS_ADVANCED(SQLite3_INCLUDE_DIRS SQLite3_LIBRARIES)
