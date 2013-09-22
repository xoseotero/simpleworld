set(SIMPLEWORLD_SRCS
  element.cpp
  movement.cpp
  world.cpp
  operations_world.cpp
  dbmemory.cpp
  isa.cpp
  cpu.cpp
  bug.cpp
  egg.cpp
  food.cpp
  mutation.cpp
  simpleworld.cpp)

add_library(simpleworld STATIC ${SIMPLEWORLD_SRCS})

install(TARGETS simpleworld
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)


add_subdirectory(cpu)
add_subdirectory(db)
