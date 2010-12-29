set(DB_SRCS
  table.cpp
  blob.cpp
  environment.cpp
  world.cpp
  spawn.cpp
  resource.cpp
  food.cpp
  mutation.cpp
  bug.cpp
  egg.cpp
  alivebug.cpp
  deadbug.cpp
  transaction.cpp
  db.cpp)
add_library(simpleworld_db SHARED ${DB_SRCS})

install(TARGETS simpleworld_db
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static)
