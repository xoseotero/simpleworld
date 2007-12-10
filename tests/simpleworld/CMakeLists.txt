ADD_DEFINITIONS("-DTESTDATA=\\\"${CMAKE_SOURCE_DIR}/tests/simpleworld/\\\"")
ADD_DEFINITIONS("-DTESTOUTPUT=\\\"${CMAKE_BINARY_DIR}/tests/simpleworld/\\\"")

# Only if RUN_TESTS is set
IF(RUN_TESTS)
  ADD_EXECUTABLE(world_test world_test.cpp)
  TARGET_LINK_LIBRARIES(world_test simpleworld_cpu simpleworld_db simpleworld
    ${Boost_program_options_LIBRARY} ${Boost_filesystem_LIBRARY}
    ${Boost_regex_LIBRARY} ${Boost_unit_test_framework_LIBRARY}
    ${SQLite3x_LIB} ${SQLite3_LIB}
    ${XySSL_LIB} ${OpenSSL_LIB})

  ADD_TEST("World" world_test)
ENDIF(RUN_TESTS)
