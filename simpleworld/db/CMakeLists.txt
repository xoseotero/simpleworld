SET(DB_SRCS
  common.cpp
  memory.cpp
  table.cpp
  environment.cpp
  food.cpp
  mutation.cpp
  code.cpp
  bugelement.cpp
  alivebug.cpp
  cpu.cpp
  bug.cpp
  egg.cpp
  deadbug.cpp
  db.cpp)
ADD_LIBRARY(simpleworld_db SHARED ${DB_SRCS})

INSTALL(TARGETS simpleworld_db
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static)
