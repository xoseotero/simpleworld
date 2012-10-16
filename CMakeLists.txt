project(SimpleWorld)
set(SIMPLEWORLD_VERSION "\"1.0b7-git\"")

cmake_minimum_required(VERSION 2.6)

include_directories("${CMAKE_SOURCE_DIR}")


# include specific modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(${CMAKE_ROOT}/Modules/TestBigEndian.cmake)
include(${CMAKE_ROOT}/Modules/CheckTypeSize.cmake)
include(${CMAKE_ROOT}/Modules/TestForSTDNamespace.cmake)
include(${CMAKE_ROOT}/Modules/TestForANSIStreamHeaders.cmake)
include(${CMAKE_ROOT}/Modules/CheckIncludeFileCXX.cmake)
include(${CMAKE_ROOT}/Modules/TestCXXAcceptsFlag.cmake)


# Check if the endianness of the system
test_big_endian(BIG_ENDIAN)
if(BIG_ENDIAN)
  set(IS_BIG_ENDIAN 1)
else()
  set(IS_LITTLE_ENDIAN 1)
endif()


# Test for standard C++ features in the compiler
if(CMAKE_NO_ANSI_STREAM_HEADERS OR CMAKE_NO_STD_NAMESPACE)
  message(FATAL_ERROR "The compiler not support standard C++ features")
endif()


# Check for GCC cxxabi.h, for demangle the name of the clases returned by
# typeid(*).name() in this compiler
check_include_file_cxx("cxxabi.h" HAVE_CXXABI_H)


# Boost is needed
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREAD OFF)
find_package(Boost COMPONENTS filesystem regex unit_test_framework)
if(NOT Boost_FOUND)
  message(FATAL_ERROR "\tBoost not found")
endif()

include_directories(${Boost_INCLUDE_DIRS})


# getopt()/getopt_long()
option(getopt_USE_INTERNAL "Use internal getopt()/getopt_long()" OFF)

if(NOT getopt_USE_INTERNAL)
  check_include_file_cxx(getopt.h HAVE_GETOPT_H)
endif()

if(NOT HAVE_GETOPT_H OR getopt_USE_INTERNAL)
  set(getopt_USE_INTERNAL 1)

  include_directories("${CMAKE_SOURCE_DIR}/getopt")
  set(getopt_LIB "getopt")
else()
  set(getopt_USE_INTERNAL 0)

  set(getopt_LIB "")
endif()


# SQLite3
option(SQLite3_USE_INTERNAL "Use internal SQLite3 library" OFF)

if(NOT SQLite3_USE_INTERNAL)
  find_package(SQLite3)
endif()

if(NOT SQLite3_FOUND OR SQLite3_USE_INTERNAL)
  set(SQLite3_USE_INTERNAL 1)

  include_directories("${CMAKE_SOURCE_DIR}/sqlite3")
  set(SQLite3_LIB "sqlite3")
else()
  set(SQLite3_USE_INTERNAL 0)

  include_directories(${SQLite3_INCLUDE_DIRS})
  set(SQLite3_LIB ${SQLite3_LIBRARIES})
endif()


check_cxx_accepts_flag("-Wall" CXX_ACCEPTS_WALL)
if(CXX_ACCEPTS_WALL)
  add_definitions("-Wall")
endif()


option(DEBUG_MODE "Build the project using debugging code" OFF)
if(DEBUG_MODE)
  set(CMAKE_BUILD_TYPE Debug)
  add_definitions("-DDEBUG")
else()
  # disable BOOST_ASSERT without affecting the definition of the
  # standard assert
  add_definitions("-DBOOST_DISABLE_ASSERTS")
endif()

option(UNIT_TESTS "Compile the unit tests" OFF)
if(UNIT_TESTS)
  enable_testing()
endif()


# Doxyfile
configure_file(${SimpleWorld_SOURCE_DIR}/Doxyfile.cmake
  ${SimpleWorld_SOURCE_DIR}/Doxyfile)
# simpleworld/config.hpp
configure_file(${SimpleWorld_SOURCE_DIR}/simpleworld/config.hpp.cmake
  ${SimpleWorld_SOURCE_DIR}/simpleworld/config.hpp)
# src/common/info.hpp
configure_file(${SimpleWorld_SOURCE_DIR}/src/common/info.hpp.cmake
  ${SimpleWorld_SOURCE_DIR}/src/common/info.hpp)


# Information about the state of the dependencies
message("Libraries used:")
if(Boost_FOUND)
  message("	boost:    external library used")
endif()

if(getopt_USE_INTERNAL)
  message("	getopt:   internal library used")
else()
  message("	getopt:   external library used")
endif()

if(SQLite3_USE_INTERNAL)
  message("	SQlite3:  internal library used")
else()
  message("	SQlite3:  external library used")
endif()


add_subdirectory(getopt)
add_subdirectory(sqlite3)
add_subdirectory(simpleworld)
add_subdirectory(src)
add_subdirectory(tests)
