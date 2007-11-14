# Two targets share the same name:
# a) the library named simpleworld
# b) the executable named simpleworld
#
# By default, cmake disallow the use of two targets with the same name, so,
# there are two possible solutions:
# 1) set a explicit LIBRARY in the TARGET_LINK_LIBRARIES
# TARGET_LINK_LIBRARIES(simpleworld LIBRARY simpleworld)
# 2) use different names and rename the output
# SET_TARGET_PROPERTIES(simpleworld_exe OUTPUT_NAME simpleworld)
#
# The second option was used

SET(fakecpu_SRCS
  fakecpu.cpp)
ADD_LIBRARY(fakecpu STATIC ${fakecpu_SRCS})


ADD_EXECUTABLE(simpleworld_exe simpleworld.cpp)
TARGET_LINK_LIBRARIES(simpleworld_exe
  simpleworld_cpu
  simpleworld_db
  simpleworld
  ${getopt_LIB}
  ${Boost_filesystem_LIBRARY}
  ${Boost_regex_LIBRARY}
  ${SQLite3x_LIB}
  ${SQLite3_LIB}
  ${XySSL_LIB}
  ${OpenSSL_LIB})
SET_TARGET_PROPERTIES(simpleworld_exe PROPERTIES
  OUTPUT_NAME simpleworld
  INSTALL_RPATH_USE_LINK_PATH ON
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

ADD_EXECUTABLE(swlc swlc.cpp)
TARGET_LINK_LIBRARIES(swlc simpleworld_cpu fakecpu
  ${getopt_LIB}
  ${Boost_program_options_LIBRARY} ${Boost_filesystem_LIBRARY}
  ${Boost_regex_LIBRARY})
SET_TARGET_PROPERTIES(swlc PROPERTIES
  INSTALL_RPATH_USE_LINK_PATH ON
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

ADD_EXECUTABLE(swld swld.cpp)
TARGET_LINK_LIBRARIES(swld simpleworld_cpu fakecpu
  ${getopt_LIB}
  ${Boost_program_options_LIBRARY} ${Boost_filesystem_LIBRARY} ${Boost_regex_LIBRARY})
SET_TARGET_PROPERTIES(swld PROPERTIES
  INSTALL_RPATH_USE_LINK_PATH ON
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

ADD_EXECUTABLE(swcpu swcpu.cpp)
TARGET_LINK_LIBRARIES(swcpu simpleworld_cpu fakecpu
  ${getopt_LIB}
  ${Boost_program_options_LIBRARY} ${Boost_filesystem_LIBRARY}
  ${Boost_regex_LIBRARY})
SET_TARGET_PROPERTIES(swcpu PROPERTIES
  INSTALL_RPATH_USE_LINK_PATH ON
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

INSTALL(TARGETS simpleworld_exe swlc swld swcpu
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static)
