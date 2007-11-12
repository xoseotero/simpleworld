INCLUDE(${CMAKE_ROOT}/Modules/TestBigEndian.cmake)
INCLUDE(${CMAKE_ROOT}/Modules/CheckTypeSize.cmake)


# Check if the endianness of the system
TEST_BIG_ENDIAN(BIG_ENDIAN)
IF(BIG_ENDIAN)
  SET(IS_BIG_ENDIAN 1)
ELSE(BIG_ENDIAN)
  SET(IS_LITTLE_ENDIAN 1)
ENDIF(BIG_ENDIAN)

# long long is a C99 64bits type not present in the C++ 98 standard
# g++ has long long
CHECK_TYPE_SIZE("long long" LONG_LONG)
# MSVC has __int64
CHECK_TYPE_SIZE("__int64" _INT64)
IF(NOT HAVE__INT64 AND NOT HAVE_LONG_LONG)
  MESSAGE(FATAL_ERROR "64bits integer not found")
ENDIF(NOT HAVE__INT64 AND NOT HAVE_LONG_LONG)


CONFIGURE_FILE(${SimpleWorld_SOURCE_DIR}/simple/config.hpp.cmake
  ${SimpleWorld_SOURCE_DIR}/simple/config.hpp)
