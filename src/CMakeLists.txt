ADD_EXECUTABLE(simpleworld main.cpp)
TARGET_LINK_LIBRARIES(simpleworld sw_cpu boost_filesystem boost_regex)
SET_TARGET_PROPERTIES(simpleworld PROPERTIES
  INSTALL_RPATH_USE_LINK_PATH ON
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

ADD_EXECUTABLE(swlc swlc.cpp)
TARGET_LINK_LIBRARIES(swlc sw_cpu
  boost_program_options boost_filesystem boost_regex)
SET_TARGET_PROPERTIES(swlc PROPERTIES
  INSTALL_RPATH_USE_LINK_PATH ON
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

ADD_EXECUTABLE(swld swld.cpp)
TARGET_LINK_LIBRARIES(swld sw_cpu
  boost_program_options boost_filesystem boost_regex)
SET_TARGET_PROPERTIES(swld PROPERTIES
  INSTALL_RPATH_USE_LINK_PATH ON
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

ADD_EXECUTABLE(swcpu swcpu.cpp)
TARGET_LINK_LIBRARIES(swcpu sw_cpu
  boost_program_options boost_filesystem boost_regex)
SET_TARGET_PROPERTIES(swcpu PROPERTIES
  INSTALL_RPATH_USE_LINK_PATH ON
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

INSTALL(TARGETS simpleworld swlc swld swcpu
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static)
