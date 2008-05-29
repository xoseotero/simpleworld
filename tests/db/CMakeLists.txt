add_definitions("-DTESTDATA=\"${CMAKE_SOURCE_DIR}/tests/db/\"")
add_definitions("-DTESTOUTPUT=\"${CMAKE_BINARY_DIR}/tests/db/\"")

# Only if UNIT_TESTS is set
if(UNIT_TESTS)
  add_executable(db_test db_test.cpp)
  target_link_libraries(db_test simpleworld_cpu simpleworld_db simpleworld
    ${Boost_program_options_LIBRARY} ${Boost_filesystem_LIBRARY}
    ${Boost_regex_LIBRARY} ${Boost_unit_test_framework_LIBRARY}
    ${SQLite3x_LIB} ${SQLite3_LIB})

  add_test("db::DB" db_test)
endif(UNIT_TESTS)
