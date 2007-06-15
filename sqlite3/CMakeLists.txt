# Use extern SQLite3 library if found
IF(SQLite3_USE_INTERNAL)
  SET(SQLite3_SRCS sqlite3.c)
  ADD_LIBRARY(sqlite3 STATIC ${SQLite3_SRCS})
ENDIF(SQLite3_USE_INTERNAL)
