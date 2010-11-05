add_definitions("-DTESTDATA=\"${CMAKE_SOURCE_DIR}/tests/db/\"")
add_definitions("-DTESTOUTPUT=\"${CMAKE_BINARY_DIR}/tests/db/\"")

# Only if UNIT_TESTS is set
if(UNIT_TESTS)
  add_executable(db_blob_test blob_test.cpp)
  target_link_libraries(db_blob_test simpleworld_cpu simpleworld_db simpleworld
    ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_REGEX_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${SQLite3x_LIB} ${SQLite3_LIB})

  add_executable(db_environment_test environment_test.cpp)
  target_link_libraries(db_environment_test simpleworld_cpu simpleworld_db simpleworld
    ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_REGEX_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${SQLite3x_LIB} ${SQLite3_LIB})

  add_executable(db_world_test world_test.cpp)
  target_link_libraries(db_world_test simpleworld_cpu simpleworld_db simpleworld
    ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_REGEX_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${SQLite3x_LIB} ${SQLite3_LIB})

  add_executable(db_food_test food_test.cpp)
  target_link_libraries(db_food_test simpleworld_cpu simpleworld_db simpleworld
    ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_REGEX_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${SQLite3x_LIB} ${SQLite3_LIB})

  add_executable(db_mutation_test mutation_test.cpp)
  target_link_libraries(db_mutation_test simpleworld_cpu simpleworld_db simpleworld
    ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_REGEX_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${SQLite3x_LIB} ${SQLite3_LIB})

  add_executable(db_bug_test bug_test.cpp)
  target_link_libraries(db_bug_test simpleworld_cpu simpleworld_db simpleworld
    ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_REGEX_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${SQLite3x_LIB} ${SQLite3_LIB})

  add_executable(db_egg_test egg_test.cpp)
  target_link_libraries(db_egg_test simpleworld_cpu simpleworld_db simpleworld
    ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_REGEX_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${SQLite3x_LIB} ${SQLite3_LIB})

  add_executable(db_alivebug_test alivebug_test.cpp)
  target_link_libraries(db_alivebug_test simpleworld_cpu simpleworld_db simpleworld
    ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_REGEX_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${SQLite3x_LIB} ${SQLite3_LIB})

  add_executable(db_deadbug_test deadbug_test.cpp)
  target_link_libraries(db_deadbug_test simpleworld_cpu simpleworld_db simpleworld
    ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_REGEX_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${SQLite3x_LIB} ${SQLite3_LIB})

  add_executable(db_db_test db_test.cpp)
  target_link_libraries(db_db_test simpleworld_cpu simpleworld_db simpleworld
    ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_REGEX_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${SQLite3x_LIB} ${SQLite3_LIB})

  add_test("db::Blob" db_blob_test)
  add_test("db::Environment" db_environment_test)
  add_test("db::World" db_world_test)
  add_test("db::Food" db_food_test)
  add_test("db::Mutation" db_mutation_test)
  add_test("db::Bug" db_bug_test)
  add_test("db::Egg" db_egg_test)
  add_test("db::AliveBug" db_alivebug_test)
  add_test("db::DeadBug" db_deadbug_test)
  add_test("db::DB" db_db_test)
endif(UNIT_TESTS)
