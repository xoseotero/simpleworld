add_definitions("-DTESTDATA=\"${CMAKE_SOURCE_DIR}/tests/simpleworld/\"")
add_definitions("-DTESTOUTPUT=\"${CMAKE_BINARY_DIR}/tests/simpleworld/\"")

# Only if UNIT_TESTS is set
if(UNIT_TESTS)
  add_executable(ints_test ints_test.cpp)
  target_link_libraries(ints_test simpleworld_cpu simpleworld_db simpleworld
    ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_REGEX_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${SQLite3x_LIB} ${SQLite3_LIB})

  add_executable(world_test world_test.cpp)
  target_link_libraries(world_test simpleworld_cpu simpleworld_db simpleworld
    ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_REGEX_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${SQLite3x_LIB} ${SQLite3_LIB})

  add_executable(movement_test movement_test.cpp)
  target_link_libraries(movement_test simpleworld_cpu simpleworld_db
    simpleworld
    ${BOOST_PROGRAM_OPTIONS_LIBRARY} ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_REGEX_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${SQLite3x_LIB} ${SQLite3_LIB})

  add_test("ints.hpp" ints_test)
  add_test("World" world_test)
  add_test("movement.hpp" movement_test)
endif()
