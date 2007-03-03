# Use extern XySSL library if found
IF(XySSL_USE_INTERNAL)
  SET(XySSL_SRCS
    md5.c
    sha1.c
    sha2.c)
  ADD_LIBRARY(xyssl STATIC ${XySSL_SRCS})
ENDIF(XySSL_USE_INTERNAL)
