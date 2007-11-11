SET(SIMPLEWORLD_SRCS
  element.cpp
  movement.cpp
  world.cpp
  operations_world.cpp
  bug.cpp
  egg.cpp
  food.cpp
  simpleworld.cpp)

ADD_LIBRARY(simpleworld STATIC ${SIMPLEWORLD_SRCS})

INSTALL(TARGETS simpleworld
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static)
