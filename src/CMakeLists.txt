# Two targets share the same name:
# a) the library named simpleworld
# b) the executable named simpleworld
#
# By default, cmake disallow the use of two targets with the same name, so,
# there are two possible solutions:
# 1) set a explicit LIBRARY in the target_link_libraries
# target_link_libraries(simpleworld LIBRARY simpleworld)
# 2) use different names and rename the output
# set_target_properties(simpleworld_exe OUTPUT_NAME simpleworld)
#
# The second option was used

set(SIMPLEWORLD_EXE_SRCS
  table.cpp
  create.cpp
  run.cpp
  vacuum.cpp
  info.cpp
  spawn.cpp
  food.cpp
  egg.cpp
  env.cpp
  simpleworld.cpp)

add_executable(simpleworld_exe ${SIMPLEWORLD_EXE_SRCS})
target_link_libraries(simpleworld_exe
  simpleworld_cpu
  simpleworld_db
  simpleworld
  common
  ${getopt_LIB}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_REGEX_LIBRARY}
  ${SQLite3x_LIB}
  ${SQLite3_LIB})
set_target_properties(simpleworld_exe PROPERTIES
  OUTPUT_NAME simpleworld
  INSTALL_RPATH_USE_LINK_PATH ON
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

install(TARGETS simpleworld_exe
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static)


add_subdirectory(common)
add_subdirectory(swlc)
add_subdirectory(swld)
add_subdirectory(swcpu)
