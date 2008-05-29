add_definitions("-DTESTDATA=\"${CMAKE_SOURCE_DIR}/tests/db/\"")
add_definitions("-DTESTOUTPUT=\"${CMAKE_BINARY_DIR}/tests/db/\"")

# Only if UNIT_TESTS is set
if(UNIT_TESTS)
  add_executable(db_test db_test.cpp)
  target_link_libraries(db_test simpleworld_cpu simpleworld_db simpleworld
    ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_REGEX_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${SQLite3x_LIB} ${SQLite3_LIB})

  add_test("db::DB" db_test)
endif(UNIT_TESTS)
