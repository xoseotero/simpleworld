PROJECT(SimpleWorld)

SUBDIRS(getopt
  sqlite3
  sqlite3x
  xyssl
  simpleworld
  src
  tests)

INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}")


# include specific modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

INCLUDE(${CMAKE_ROOT}/Modules/TestBigEndian.cmake)
INCLUDE(${CMAKE_ROOT}/Modules/CheckTypeSize.cmake)
INCLUDE(${CMAKE_ROOT}/Modules/TestForSTDNamespace.cmake)
INCLUDE(${CMAKE_ROOT}/Modules/TestForANSIStreamHeaders.cmake)
INCLUDE(${CMAKE_ROOT}/Modules/CheckIncludeFileCXX.cmake)
INCLUDE(${CMAKE_ROOT}/Modules/TestCXXAcceptsFlag.cmake)


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


# Test for standard C++ features in the compiler
IF(CMAKE_NO_ANSI_STREAM_HEADERS OR CMAKE_NO_STD_NAMESPACE)
  MESSAGE(FATAL_ERROR "The compiler not support standard C++ features")
ENDIF(CMAKE_NO_ANSI_STREAM_HEADERS OR CMAKE_NO_STD_NAMESPACE)


# Boost is needed
SET(Boost_LIB_TYPE "st")
#SET(Boost_LIB_SUFFIX "gcc41-mt-1_34")
FIND_PACKAGE(Boost REQUIRED filesystem regex unit_test_framework)
IF(NOT Boost_FOUND)
  MESSAGE(FATAL_ERROR "\tBoost not found")
ENDIF(NOT Boost_FOUND)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})


# getopt()/getopt_long()
OPTION(getopt_USE_INTERNAL "Use internal getopt()/getopt_long()" OFF)

IF(NOT getopt_USE_INTERNAL)
  CHECK_INCLUDE_FILE_CXX(getopt.h HAVE_GETOPT_H)
ENDIF(NOT getopt_USE_INTERNAL)

IF(NOT HAVE_GETOPT_H OR getopt_USE_INTERNAL)
  SET(getopt_USE_INTERNAL 1)

  INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/getopt")
  SET(getopt_LIB "getopt")
ELSE(NOT HAVE_GETOPT_H OR getopt_USE_INTERNAL)
  SET(getopt_USE_INTERNAL 0)

  SET(getopt_LIB "")
ENDIF(NOT HAVE_GETOPT_H OR getopt_USE_INTERNAL)


# SQLite3
OPTION(SQLite3_USE_INTERNAL "Use internal SQLite3 library" OFF)

IF(NOT SQLite3_USE_INTERNAL)
  FIND_PACKAGE(SQLite3)
ENDIF(NOT SQLite3_USE_INTERNAL)

IF(NOT SQLite3_FOUND OR SQLite3_USE_INTERNAL)
  SET(SQLite3_USE_INTERNAL 1)

  INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/sqlite3")
  SET(SQLite3_LIB "sqlite3")
ELSE(NOT SQLite3_FOUND OR SQLite3_USE_INTERNAL)
  SET(SQLite3_USE_INTERNAL 0)

  INCLUDE_DIRECTORIES(${SQLite3_INCLUDE_DIRS})
  SET(SQLite3_LIB ${SQLite3_LIBRARIES})
ENDIF(NOT SQLite3_FOUND OR SQLite3_USE_INTERNAL)


# SQLite3x
OPTION(SQLite3x_USE_INTERNAL "Use internal SQLite3x library" OFF)

IF(NOT SQLite3x_USE_INTERNAL)
  FIND_PACKAGE(SQLite3x)
ENDIF(NOT SQLite3x_USE_INTERNAL)

IF(NOT SQLite3x_FOUND OR SQLite3x_USE_INTERNAL)
  SET(SQLite3x_USE_INTERNAL 1)

  INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/sqlite3x")
  SET(SQLite3x_LIB "sqlite3x")
ELSE(NOT SQLite3x_FOUND OR SQLite3x_USE_INTERNAL)
  SET(SQLite3x_USE_INTERNAL 0)

  INCLUDE_DIRECTORIES(${SQLite3x_INCLUDE_DIRS})
  SET(SQLite3x_LIB ${SQLite3x_LIBRARIES})
ENDIF(NOT SQLite3x_FOUND OR SQLite3x_USE_INTERNAL)


# XySSL
OPTION(XySSL_USE_INTERNAL "Use internal XySSL library" OFF)

IF(NOT XySSL_USE_INTERNAL)
  FIND_PACKAGE(XySSL)
ENDIF(NOT XySSL_USE_INTERNAL)

IF(NOT XySSL_FOUND OR XySSL_USE_INTERNAL)
  SET(XySSL_USE_INTERNAL 1)

  INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/xyssl")
  SET(XySSL_LIB "xyssl")
ELSE(NOT XySSL_FOUND OR XySSL_USE_INTERNAL)
  SET(XySSL_USE_INTERNAL 0)

  INCLUDE_DIRECTORIES(${XySSL_INCLUDE_DIRS})
  SET(XySSL_LIB ${XySSL_LIBRARIES})
ENDIF(NOT XySSL_FOUND OR XySSL_USE_INTERNAL)


CHECK_CXX_ACCEPTS_FLAG("-Wall" CXX_ACCEPTS_WALL)
IF(CXX_ACCEPTS_WALL)
  ADD_DEFINITIONS("-Wall")
ENDIF(CXX_ACCEPTS_WALL)


OPTION(DEBUG_MODE "Build the project using debugging code" OFF)
IF(DEBUG_MODE)
  SET(CMAKE_BUILD_TYPE Debug)
  ADD_DEFINITIONS("-DDEBUG")
ELSE(DEBUG_MODE)
  SET(CMAKE_BUILD_TYPE Release)
  # disable BOOST_ASSERT without affecting the definition of the
  # standard assert
  ADD_DEFINITIONS("-DBOOST_DISABLE_ASSERTS")
ENDIF(DEBUG_MODE)

OPTION(RUN_TESTS "Execute the tests" OFF)
IF(RUN_TESTS)
  ENABLE_TESTING()
ENDIF(RUN_TESTS)


# config.hpp
CONFIGURE_FILE(${SimpleWorld_SOURCE_DIR}/simpleworld/config.hpp.cmake
  ${SimpleWorld_SOURCE_DIR}/simpleworld/config.hpp)


# Information about the state of the dependencies
MESSAGE("Libraries used:")
IF(Boost_FOUND)
  MESSAGE("\tboost: external library used")
ENDIF(Boost_FOUND)

IF(getopt_USE_INTERNAL)
  MESSAGE("\tgetopt:\tinternal library used")
ELSE(getopt_USE_INTERNAL)
  MESSAGE("\tgetopt:\texternal library used")
ENDIF(getopt_USE_INTERNAL)

IF(SQLite3_USE_INTERNAL)
  MESSAGE("\tSQlite3:\tinternal library used")
ELSE(SQLite3_USE_INTERNAL)
  MESSAGE("\tSQlite3:\texternal library used")
ENDIF(SQLite3_USE_INTERNAL)

IF(SQLite3x_USE_INTERNAL)
  MESSAGE("\tSQLite3x:\tinternal library used")
ELSE(SQLite3x_USE_INTERNAL)
  MESSAGE("\tSQLite3x:\texternal library used")
ENDIF(SQLite3x_USE_INTERNAL)

IF(XySSL_USE_INTERNAL)
  MESSAGE("\tXySSL:\tinternal library used")
ELSE(XySSL_USE_INTERNAL)
  MESSAGE("\tXySSL:\tinternal library used")
ENDIF(XySSL_USE_INTERNAL)
