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

  ADD_TEST(word.h word_test)
  ADD_TEST(memory.h memory_test)
  ADD_TEST(instruction.h instruction_test)
ENDIF(RUN_TESTS)
