# - Find the XySSL includes and library.
# The following variables are set if XySSL is found.  If XySSL is not
# found, XySSL_FOUND is set to false.
#  XySSL_FOUND        - True when the XySSL include directory is found.
#  XySSL_INCLUDE_DIRS - the path to where the XySSL include files are.
#  XySSL_LIBRARIES    - The XySSL library.
MESSAGE(STATUS "Looking for XySSL")

FIND_PATH(XySSL_INCLUDE_DIRS xyssl/md5.h /usr/include /usr/local/include)
FIND_LIBRARY(XySSL_LIBRARIES NAMES xyssl PATHS /usr/lib /usr/local/lib)

IF(XySSL_INCLUDE_DIRS AND XySSL_LIBRARIES)
  SET(XySSL_FOUND 1)
  IF(NOT XySSL_FIND_QUIETLY)
    MESSAGE(STATUS "Looking for XySSL - found")
  ENDIF(NOT XySSL_FIND_QUIETLY)
ELSE(XySSL_INCLUDE_DIRS AND XySSL_LIBRARIES)
  IF(NOT XySSL_FIND_QUIETLY)
    MESSAGE(STATUS "Looking for XySSL - not found")
  ENDIF(NOT XySSL_FIND_QUIETLY)
ENDIF(XySSL_INCLUDE_DIRS AND XySSL_LIBRARIES)

MARK_AS_ADVANCED(XySSL_INCLUDE_DIRS XySSL_LIBRARIES)
