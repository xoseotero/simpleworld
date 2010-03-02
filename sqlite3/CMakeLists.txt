# Use extern SQLite3 library if found
if(SQLite3_USE_INTERNAL)
  set(SQLite3_SRCS sqlite3.c)
  add_library(sqlite3 STATIC ${SQLite3_SRCS})

  # Simple World doesn't use extensions
  add_definitions(-DSQLITE_OMIT_LOAD_EXTENSION)

  # Simple World doesn't use threads
  # If threads are needed, there should be added pthread support for UNIXes.
  add_definitions(-DSQLITE_THREADSAFE=0)
endif(SQLite3_USE_INTERNAL)
