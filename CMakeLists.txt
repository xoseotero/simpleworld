PROJECT(SimpleWorld)
INCLUDE_DIRECTORIES(.)
SUBDIRS(simple cpu src tests)
LINK_DIRECTORIES(cpu)


INCLUDE(${CMAKE_ROOT}/Modules/TestForANSIStreamHeaders.cmake)
INCLUDE(${CMAKE_ROOT}/Modules/TestForSTDNamespace.cmake)
INCLUDE(${CMAKE_ROOT}/Modules/CheckIncludeFileCXX.cmake)

# include specific modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")


# Boost is needed
FIND_PACKAGE(Boost REQUIRED)
IF(NOT Boost_FOUND)
  MESSAGE(FATAL_ERROR "Boost not found")
ENDIF(NOT Boost_FOUND)

SET(CMAKE_REQUIRED_INCLUDES "${Boost_INCLUDE_DIRS}")
# Boost Filesystem Library is needed
CHECK_INCLUDE_FILE_CXX("boost/filesystem/path.hpp" HAVE_BOOST_PATH)
IF(NOT HAVE_BOOST_PATH)
  MESSAGE(FATAL_ERROR "Boost Filesystem Library not found")
ENDIF(NOT HAVE_BOOST_PATH)

# Boost Regular Expressions Library is needed
CHECK_INCLUDE_FILE_CXX("boost/regex.hpp" HAVE_BOOST_REGEX)
IF(NOT HAVE_BOOST_REGEX)
  MESSAGE(FATAL_ERROR "Boost Regular Expressions Library not found")
ENDIF(NOT HAVE_BOOST_REGEX)

IF(SW_TEST)
  # Boost Unit Test Frameword is needed
  CHECK_INCLUDE_FILE_CXX("boost/test/unit_test.hpp" HAVE_BOOST_UTF)
  IF(NOT HAVE_BOOST_UTF)
    MESSAGE(FATAL_ERROR "Boost Unit Test Framework not found")
  ENDIF(NOT HAVE_BOOST_UTF)
ENDIF(SW_TEST)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

# SQLite3
FIND_PACKAGE(SQLite3 REQUIRED)
IF(NOT SQLite3_FOUND)
  MESSAGE(FATAL_ERROR "SQLite3 not found")
ENDIF(NOT SQLite3_FOUND)

INCLUDE_DIRECTORIES(${SQLite3_INCLUDE_DIRS})
LINK_DIRECTORIES(${SQLite3_LIBRARY_DIRS})


OPTION(SW_DEBUG "Build the project using debugging code" OFF)
IF(SW_DEBUG)
  ADD_DEFINITIONS(-DDEBUG)
ENDIF(SW_DEBUG)

OPTION(SW_TEST "Execute the tests" ON)
IF(SW_TEST)
  ENABLE_TESTING()
ENDIF(SW_TEST)
