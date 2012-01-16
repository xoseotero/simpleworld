add_definitions("-DINCLUDE_DIR=\"${CMAKE_SOURCE_DIR}/swl/\"")
add_definitions("-DTESTOUTPUT=\"${CMAKE_BINARY_DIR}/tests/stdlib/\"")

# Only if UNIT_TESTS is set
if(UNIT_TESTS)
  add_executable(def_swl_test def_test.cpp)
  target_link_libraries(def_swl_test
    simpleworld_cpu common
    ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

  add_executable(error_swl_test error_test.cpp)
  target_link_libraries(error_swl_test
    simpleworld_cpu common
    ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

  add_executable(world_swl_test world_test.cpp)
  target_link_libraries(world_swl_test
    simpleworld_cpu common
    ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

  add_executable(bits_swl_test bits_test.cpp)
  target_link_libraries(bits_swl_test
    simpleworld_cpu common
    ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

  add_executable(random_swl_test random_test.cpp)
  target_link_libraries(random_swl_test
    simpleworld_cpu common
    ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

  add_executable(int_swl_test int_test.cpp)
  target_link_libraries(int_swl_test
    simpleworld_cpu common
    ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

  add_executable(time_swl_test time_test.cpp)
  target_link_libraries(time_swl_test
    simpleworld_cpu common
    ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

  add_executable(address_swl_test address_test.cpp)
  target_link_libraries(address_swl_test
    simpleworld_cpu common
    ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

  add_executable(math_swl_test math_test.cpp)
  target_link_libraries(math_swl_test
    simpleworld_cpu common
    ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

  add_executable(mem_swl_test mem_test.cpp)
  target_link_libraries(mem_swl_test
    simpleworld_cpu common
    ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

  add_executable(jmp_swl_test jmp_test.cpp)
  target_link_libraries(jmp_swl_test
    simpleworld_cpu common
    ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

  add_executable(alloc_swl_test alloc_test.cpp)
  target_link_libraries(alloc_swl_test
    simpleworld_cpu common
    ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

  add_executable(init_swl_test init_test.cpp)
  target_link_libraries(init_swl_test
    simpleworld_cpu common
    ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

  add_executable(array_swl_test array_test.cpp)
  target_link_libraries(array_swl_test
    simpleworld_cpu common
    ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

  add_executable(node_swl_test node_test.cpp)
  target_link_libraries(node_swl_test
    simpleworld_cpu common
    ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

  add_executable(stack_swl_test stack_test.cpp)
  target_link_libraries(stack_swl_test
    simpleworld_cpu common
    ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

  add_executable(queue_swl_test queue_test.cpp)
  target_link_libraries(queue_swl_test
    simpleworld_cpu common
    ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

  add_executable(list_swl_test list_test.cpp)
  target_link_libraries(list_swl_test
    simpleworld_cpu common
    ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

  add_executable(set_swl_test set_test.cpp)
  target_link_libraries(set_swl_test
    simpleworld_cpu common
    ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

  add_executable(map_swl_test map_test.cpp)
  target_link_libraries(map_swl_test
    simpleworld_cpu common
    ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

  add_executable(multimap_swl_test multimap_test.cpp)
  target_link_libraries(multimap_swl_test
    simpleworld_cpu common
    ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

  add_test("stdlib/def.swl" def_swl_test)
  add_test("stdlib/error.swl" error_swl_test)
  add_test("stdlib/world.swl" world_swl_test)
  add_test("stdlib/bits.swl" bits_swl_test)
  add_test("stdlib/random.swl" random_swl_test)
  add_test("stdlib/int.swl" int_swl_test)
  add_test("stdlib/time.swl" time_swl_test)
  add_test("stdlib/address.swl" address_swl_test)
  add_test("stdlib/math.swl" math_swl_test)
  add_test("stdlib/mem.swl" mem_swl_test)
  add_test("stdlib/jmp.swl" jmp_swl_test)
  add_test("stdlib/alloc.swl" alloc_swl_test)
  add_test("stdlib/init.swl" init_swl_test)
  add_test("stdlib/array.swl" array_swl_test)
  add_test("stdlib/node.swl" node_swl_test)
  add_test("stdlib/stack.swl" stack_swl_test)
  add_test("stdlib/queue.swl" queue_swl_test)
  add_test("stdlib/list.swl" list_swl_test)
  add_test("stdlib/set.swl" set_swl_test)
  add_test("stdlib/map.swl" map_swl_test)
  add_test("stdlib/multimap.swl" multimap_swl_test)
endif(UNIT_TESTS)
