set(CPU_SRCS
  word.cpp
  memory.cpp memory_file.cpp
  operations_management.cpp operations_move.cpp operations_branch.cpp
  operations_function.cpp operations_arithmetic.cpp operations_sign.cpp
  operations_logic.cpp operations_shift.cpp
  instruction.cpp isa.cpp
  cpu.cpp
  file.cpp source.cpp
  object.cpp)
add_library(simpleworld_cpu SHARED ${CPU_SRCS})

install(TARGETS simpleworld_cpu
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
