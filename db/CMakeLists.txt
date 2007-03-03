SET(DB_SRCS
  table.cpp
  bug.cpp
  hierarchy.cpp
  code.cpp
  cpu.cpp
  mutations.cpp
  environment.cpp
  db.cpp)
ADD_LIBRARY(sw_db SHARED ${DB_SRCS})

INSTALL(TARGETS sw_db
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static)
