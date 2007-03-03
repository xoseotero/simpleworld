# Use extern SQLite3x library if found
IF(SQLite3x_USE_INTERNAL)
  SET(SQLite3x_SRCS
    sqlite3x_exception.cpp
    sqlite3x_connection.cpp
    sqlite3x_transaction.cpp
    sqlite3x_command.cpp
    sqlite3x_cursor.cpp)
  ADD_LIBRARY(sqlite3x STATIC ${SQLite3x_SRCS})
ENDIF(SQLite3x_USE_INTERNAL)
