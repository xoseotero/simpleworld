add_definitions("-DTESTDATA=\"${CMAKE_SOURCE_DIR}/tests/simpleworld/\"")
add_definitions("-DTESTOUTPUT=\"${CMAKE_BINARY_DIR}/tests/simpleworld/\"")

# Only if UNIT_TESTS is set
if(UNIT_TESTS)
  add_executable(world_test world_test.cpp)
  target_link_libraries(world_test simpleworld_cpu simpleworld_db simpleworld
    ${Boost_program_options_LIBRARY} ${Boost_filesystem_LIBRARY}
    ${Boost_regex_LIBRARY} ${Boost_unit_test_framework_LIBRARY}
    ${SQLite3x_LIB} ${SQLite3_LIB})

  add_executable(movement_test movement_test.cpp)
  target_link_libraries(movement_test simpleworld_cpu simpleworld_db
    simpleworld
    ${Boost_program_options_LIBRARY} ${Boost_filesystem_LIBRARY}
    ${Boost_regex_LIBRARY} ${Boost_unit_test_framework_LIBRARY}
    ${SQLite3x_LIB} ${SQLite3_LIB})

  add_test("World" world_test)
  add_test("movement.hpp" movement_test)
endif(UNIT_TESTS)
