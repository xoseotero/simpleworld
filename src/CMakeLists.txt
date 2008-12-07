set(common_SRCS
  printexc.cpp
  fakecpu.cpp)
add_library(common STATIC ${common_SRCS})


add_subdirectory(swlc)
add_subdirectory(swld)
add_subdirectory(swcpu)
add_subdirectory(simpleworld)
