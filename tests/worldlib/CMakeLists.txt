add_definitions("-DINCLUDE_DIR=\"${CMAKE_SOURCE_DIR}/swl/\"")
add_definitions("-DTESTOUTPUT=\"${CMAKE_BINARY_DIR}/tests/worldlib/\"")

# Only if UNIT_TESTS is set
if(UNIT_TESTS)
  add_executable(events_swl_test events_test.cpp)
  target_link_libraries(events_swl_test
    simpleworld_cpu common
    ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

  add_test("worldlib/events.swl" events_swl_test)
endif(UNIT_TESTS)
