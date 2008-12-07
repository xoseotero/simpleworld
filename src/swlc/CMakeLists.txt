add_executable(swlc swlc.cpp)
target_link_libraries(swlc
  simpleworld_cpu
  common
  ${getopt_LIB}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_REGEX_LIBRARY})
set_target_properties(swlc PROPERTIES
  INSTALL_RPATH_USE_LINK_PATH ON
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

install(TARGETS swlc
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static)
