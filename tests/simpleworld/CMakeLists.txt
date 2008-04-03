ADD_DEFINITIONS("-DTESTDATA=\\\"${CMAKE_SOURCE_DIR}/tests/simpleworld/\\\"")
ADD_DEFINITIONS("-DTESTOUTPUT=\\\"${CMAKE_BINARY_DIR}/tests/simpleworld/\\\"")

# Only if UNIT_TESTS is set
IF(UNIT_TESTS)
  ADD_EXECUTABLE(world_test world_test.cpp)
  TARGET_LINK_LIBRARIES(world_test simpleworld_cpu simpleworld_db simpleworld
    ${Boost_program_options_LIBRARY} ${Boost_filesystem_LIBRARY}
    ${Boost_regex_LIBRARY} ${Boost_unit_test_framework_LIBRARY}
    ${SQLite3x_LIB} ${SQLite3_LIB}
    ${XySSL_LIB} ${OpenSSL_LIB})

  ADD_EXECUTABLE(movement_test movement_test.cpp)
  TARGET_LINK_LIBRARIES(movement_test simpleworld_cpu simpleworld_db
    simpleworld
    ${Boost_program_options_LIBRARY} ${Boost_filesystem_LIBRARY}
    ${Boost_regex_LIBRARY} ${Boost_unit_test_framework_LIBRARY}
    ${SQLite3x_LIB} ${SQLite3_LIB}
    ${XySSL_LIB} ${OpenSSL_LIB})

  ADD_TEST("World" world_test)
  ADD_TEST("movement.hpp" movement_test)
ENDIF(UNIT_TESTS)
