# Use extern XySSL library if found
IF(NOT XySSL_FOUND)
  SET(XySSL_SRCS
    md5.c
    sha1.c
    sha2.c)
  ADD_LIBRARY(xyssl STATIC ${XySSL_SRCS})
ENDIF(NOT XySSL_FOUND)
