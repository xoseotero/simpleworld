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

  ADD_TEST(word.hpp word_test)
  ADD_TEST(memory.hpp memory_test)
  ADD_TEST(instruction.hpp instruction_test)
  ADD_TEST(isa.hpp isa_test)
  ADD_TEST(file.hpp file_test)
  ADD_TEST(source.hpp source_test)
  ADD_TEST(object.hpp object_test)
ENDIF(RUN_TESTS)
