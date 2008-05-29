ADD_DEFINITIONS("-DTESTDATA=\"${CMAKE_SOURCE_DIR}/tests/db/\"")
ADD_DEFINITIONS("-DTESTOUTPUT=\"${CMAKE_BINARY_DIR}/tests/db/\"")

# Only if UNIT_TESTS is set
IF(UNIT_TESTS)
  ADD_EXECUTABLE(db_test db_test.cpp)
  TARGET_LINK_LIBRARIES(db_test simpleworld_cpu simpleworld_db simpleworld
    ${Boost_program_options_LIBRARY} ${Boost_filesystem_LIBRARY}
    ${Boost_regex_LIBRARY} ${Boost_unit_test_framework_LIBRARY}
    ${SQLite3x_LIB} ${SQLite3_LIB}
    ${XySSL_LIB} ${OpenSSL_LIB})

  ADD_TEST("db::DB" db_test)
ENDIF(UNIT_TESTS)
