SET(CPU_SRCS
  word.cpp
  memory.cpp
  instruction.cpp
  operation.cpp
  operations_management.cpp operations_move.cpp operations_branch.cpp
  operations_function.cpp operations_arithmetic.cpp operations_logic.cpp
  operations_shift.cpp
  cpu.cpp cpu_file.cpp
  file.cpp source.cpp
  object.cpp)
ADD_LIBRARY(sw_cpu SHARED ${CPU_SRCS})

INSTALL(TARGETS sw_cpu
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static)
