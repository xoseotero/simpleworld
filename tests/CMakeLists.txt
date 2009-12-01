# Only if SW_TEST is set
IF(SW_TEST)
  ADD_EXECUTABLE(word_test word_test.cpp)
  TARGET_LINK_LIBRARIES(word_test sw_cpu
    boost_program_options boost_filesystem boost_regex
    boost_unit_test_framework)

  ADD_EXECUTABLE(memory_test memory_test.cpp)
  TARGET_LINK_LIBRARIES(memory_test sw_cpu
    boost_program_options boost_filesystem boost_regex
    boost_unit_test_framework)

  ADD_EXECUTABLE(instruction_test instruction_test.cpp)
  TARGET_LINK_LIBRARIES(instruction_test sw_cpu
    boost_program_options boost_filesystem boost_regex
    boost_unit_test_framework)

  ADD_TEST(word.h word_test)
  ADD_TEST(memory.h memory_test)
  ADD_TEST(instruction.h instruction_test)
ENDIF(SW_TEST)
