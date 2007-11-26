SET(CPU_SRCS
  word.cpp
  memory.cpp memory_file.cpp
  operations_management.cpp operations_move.cpp operations_branch.cpp
  operations_function.cpp operations_arithmetic.cpp operations_logic.cpp
  operations_shift.cpp
  instruction.cpp isa.cpp
  cpu.cpp
  file.cpp source.cpp
  object.cpp)
ADD_LIBRARY(simpleworld_cpu SHARED ${CPU_SRCS})

INSTALL(TARGETS simpleworld_cpu
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static)
