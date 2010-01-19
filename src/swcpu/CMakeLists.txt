set(SWCPU_SRCS
  memoryfile.cpp
  cpu.cpp
  swcpu.cpp)

add_executable(swcpu ${SWCPU_SRCS})
target_link_libraries(swcpu
  simpleworld_cpu
  common
  ${getopt_LIB}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_REGEX_LIBRARY})
set_target_properties(swcpu PROPERTIES
  INSTALL_RPATH_USE_LINK_PATH ON
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

install(TARGETS swcpu
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static)
