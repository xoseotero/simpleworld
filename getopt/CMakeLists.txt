# Use extern getopt()/getopt_long() if found
IF(getopt_USE_INTERNAL)
  SET(getopt_SRCS
    getopt.c
    getopt1.c)
  ADD_LIBRARY(getopt STATIC ${getopt_SRCS})
ENDIF(getopt_USE_INTERNAL)
