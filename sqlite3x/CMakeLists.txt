# Use extern SQLite3x library if found
if(SQLite3x_USE_INTERNAL)
  set(SQLite3x_SRCS
    sqlite3x_exception.cpp
    sqlite3x_connection.cpp
    sqlite3x_transaction.cpp
    sqlite3x_command.cpp
    sqlite3x_cursor.cpp)
  add_library(sqlite3x STATIC ${SQLite3x_SRCS})
endif(SQLite3x_USE_INTERNAL)
