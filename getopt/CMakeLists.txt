# Use extern getopt()/getopt_long() if found
if(getopt_USE_INTERNAL)
  set(getopt_SRCS
    getopt.c
    getopt1.c)
  add_library(getopt STATIC ${getopt_SRCS})
endif()
