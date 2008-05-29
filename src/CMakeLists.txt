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

set(common_SRCS
  printexc.cpp
  fakecpu.cpp)
add_library(common STATIC ${common_SRCS})


add_executable(simpleworld_exe simpleworld.cpp)
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

add_executable(swlc swlc.cpp)
target_link_libraries(swlc
  simpleworld_cpu
  common
  ${getopt_LIB}
  ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_REGEX_LIBRARY})
set_target_properties(swlc PROPERTIES
  INSTALL_RPATH_USE_LINK_PATH ON
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

add_executable(swld swld.cpp)
target_link_libraries(swld
  simpleworld_cpu
  common
  ${getopt_LIB}
  ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_REGEX_LIBRARY})
set_target_properties(swld PROPERTIES
  INSTALL_RPATH_USE_LINK_PATH ON
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

add_executable(swcpu swcpu.cpp)
target_link_libraries(swcpu
  simpleworld_cpu
  common
  ${getopt_LIB}
  ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_REGEX_LIBRARY})
set_target_properties(swcpu PROPERTIES
  INSTALL_RPATH_USE_LINK_PATH ON
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")


install(TARGETS simpleworld_exe swlc swld swcpu
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static)
