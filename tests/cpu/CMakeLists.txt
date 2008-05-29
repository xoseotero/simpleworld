add_definitions("-DTESTDATA=\"${CMAKE_SOURCE_DIR}/tests/cpu/\"")
add_definitions("-DTESTOUTPUT=\"${CMAKE_BINARY_DIR}/tests/cpu/\"")

# Only if UNIT_TESTS is set
if(UNIT_TESTS)
  add_executable(word_test word_test.cpp)
  target_link_libraries(word_test simpleworld_cpu
    ${Boost_program_options_LIBRARY} ${Boost_filesystem_LIBRARY}
    ${Boost_regex_LIBRARY} ${Boost_unit_test_framework_LIBRARY})

  add_executable(memory_test memory_test.cpp)
  target_link_libraries(memory_test simpleworld_cpu
    ${Boost_program_options_LIBRARY} ${Boost_filesystem_LIBRARY}
    ${Boost_regex_LIBRARY} ${Boost_unit_test_framework_LIBRARY})

  add_executable(instruction_test instruction_test.cpp)
  target_link_libraries(instruction_test simpleworld_cpu
    ${Boost_program_options_LIBRARY} ${Boost_filesystem_LIBRARY}
    ${Boost_regex_LIBRARY} ${Boost_unit_test_framework_LIBRARY})

  add_executable(cs_test cs_test.cpp)
  target_link_libraries(cs_test simpleworld_cpu
    ${Boost_program_options_LIBRARY} ${Boost_filesystem_LIBRARY}
    ${Boost_regex_LIBRARY} ${Boost_unit_test_framework_LIBRARY})

  add_executable(isa_test isa_test.cpp)
  target_link_libraries(isa_test simpleworld_cpu
    ${Boost_program_options_LIBRARY} ${Boost_filesystem_LIBRARY}
    ${Boost_regex_LIBRARY} ${Boost_unit_test_framework_LIBRARY})

  add_executable(file_test file_test.cpp)
  target_link_libraries(file_test simpleworld_cpu
    ${Boost_program_options_LIBRARY} ${Boost_filesystem_LIBRARY}
    ${Boost_regex_LIBRARY} ${Boost_unit_test_framework_LIBRARY})

  add_executable(source_test source_test.cpp)
  target_link_libraries(source_test simpleworld_cpu
    ${Boost_program_options_LIBRARY} ${Boost_filesystem_LIBRARY}
    ${Boost_regex_LIBRARY} ${Boost_unit_test_framework_LIBRARY})

  add_executable(object_test object_test.cpp)
  target_link_libraries(object_test simpleworld_cpu
    ${Boost_program_options_LIBRARY} ${Boost_filesystem_LIBRARY}
    ${Boost_regex_LIBRARY} ${Boost_unit_test_framework_LIBRARY})

  add_executable(cpu_test cpu_test.cpp)
  target_link_libraries(cpu_test simpleworld_cpu
    ${Boost_program_options_LIBRARY} ${Boost_filesystem_LIBRARY}
    ${Boost_regex_LIBRARY} ${Boost_unit_test_framework_LIBRARY})

  add_test("cpu/word.hpp" word_test)
  add_test("cpu::Memory" memory_test)
  add_test("cpu::Instruction" instruction_test)
  add_test("cpu::CS" cs_test)
  add_test("cpu::ISA" isa_test)
  add_test("cpu::File" file_test)
  add_test("cpu::Source" source_test)
  add_test("cpu::object" object_test)
  add_test("cpu::CPU" cpu_test)
endif(UNIT_TESTS)
