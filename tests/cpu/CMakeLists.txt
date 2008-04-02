ADD_DEFINITIONS("-DTESTDATA=\\\"${CMAKE_SOURCE_DIR}/tests/cpu/\\\"")
ADD_DEFINITIONS("-DTESTOUTPUT=\\\"${CMAKE_BINARY_DIR}/tests/cpu/\\\"")

# Only if RUN_TESTS is set
IF(RUN_TESTS)
  ADD_EXECUTABLE(word_test word_test.cpp)
  TARGET_LINK_LIBRARIES(word_test simpleworld_cpu
    ${Boost_program_options_LIBRARY} ${Boost_filesystem_LIBRARY}
    ${Boost_regex_LIBRARY} ${Boost_unit_test_framework_LIBRARY})

  ADD_EXECUTABLE(memory_test memory_test.cpp)
  TARGET_LINK_LIBRARIES(memory_test simpleworld_cpu
    ${Boost_program_options_LIBRARY} ${Boost_filesystem_LIBRARY}
    ${Boost_regex_LIBRARY} ${Boost_unit_test_framework_LIBRARY})

  ADD_EXECUTABLE(instruction_test instruction_test.cpp)
  TARGET_LINK_LIBRARIES(instruction_test simpleworld_cpu
    ${Boost_program_options_LIBRARY} ${Boost_filesystem_LIBRARY}
    ${Boost_regex_LIBRARY} ${Boost_unit_test_framework_LIBRARY})

  ADD_EXECUTABLE(cs_test cs_test.cpp)
  TARGET_LINK_LIBRARIES(cs_test simpleworld_cpu
    ${Boost_program_options_LIBRARY} ${Boost_filesystem_LIBRARY}
    ${Boost_regex_LIBRARY} ${Boost_unit_test_framework_LIBRARY})

  ADD_EXECUTABLE(isa_test isa_test.cpp)
  TARGET_LINK_LIBRARIES(isa_test simpleworld_cpu
    ${Boost_program_options_LIBRARY} ${Boost_filesystem_LIBRARY}
    ${Boost_regex_LIBRARY} ${Boost_unit_test_framework_LIBRARY})

  ADD_EXECUTABLE(file_test file_test.cpp)
  TARGET_LINK_LIBRARIES(file_test simpleworld_cpu
    ${Boost_program_options_LIBRARY} ${Boost_filesystem_LIBRARY}
    ${Boost_regex_LIBRARY} ${Boost_unit_test_framework_LIBRARY})

  ADD_EXECUTABLE(source_test source_test.cpp)
  TARGET_LINK_LIBRARIES(source_test simpleworld_cpu
    ${Boost_program_options_LIBRARY} ${Boost_filesystem_LIBRARY}
    ${Boost_regex_LIBRARY} ${Boost_unit_test_framework_LIBRARY})

  ADD_EXECUTABLE(object_test object_test.cpp)
  TARGET_LINK_LIBRARIES(object_test simpleworld_cpu
    ${Boost_program_options_LIBRARY} ${Boost_filesystem_LIBRARY}
    ${Boost_regex_LIBRARY} ${Boost_unit_test_framework_LIBRARY})

  ADD_EXECUTABLE(cpu_test cpu_test.cpp)
  TARGET_LINK_LIBRARIES(cpu_test simpleworld_cpu
    ${Boost_program_options_LIBRARY} ${Boost_filesystem_LIBRARY}
    ${Boost_regex_LIBRARY} ${Boost_unit_test_framework_LIBRARY})

  ADD_TEST("cpu/word.hpp" word_test)
  ADD_TEST("CPU::Memory" memory_test)
  ADD_TEST("CPU::Instruction" instruction_test)
  ADD_TEST("CPU::CS" cs_test)
  ADD_TEST("CPU::ISA" isa_test)
  ADD_TEST("CPU::File" file_test)
  ADD_TEST("CPU::Source" source_test)
  ADD_TEST("CPU::object" object_test)
  ADD_TEST("CPU::CPU" cpu_test)
ENDIF(RUN_TESTS)
