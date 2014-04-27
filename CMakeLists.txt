cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR) # Required for FindBoost
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
project(libastron-cxx)
include_directories(src)

## Setup testing
# If python is available we can run python-based unittests
find_package(PythonLibs)
find_package(PythonInterp)
if(PYTHONINTERP_FOUND)
  enable_testing()
  if(PYTHON_VERSION_MAJOR EQUAL 2)
    set(PYTHON2_EXECUTABLE "${PYTHON_EXECUTABLE}")
  else()
    set(PYTHON2_EXECUTABLE python2)
  endif()
endif()


## Configure the build type
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  # Use RelWithDebInfo by default
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Build type.")
endif()
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
  RelWithDebInfo Release Debug)


## Operating-system specific build flags
# Linux requires pthreads
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(CMAKE_CXX_FLAGS "-pthread")
endif()

# Windows needs an extra define
if(WIN32)
  add_definitions(-D_WIN32_WINDOWS)
endif()


## Compiler specific build flags
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
   CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -Wall -Wextra")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO
    "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g -Wall")

  # warnings are errors, release code should not have warnings
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -Wall -Werror")

  # debug flags, wall, wextra
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -Wall -Wextra")
endif()

if(CMAKE_GENERATOR STREQUAL Xcode)
  set(CMAKE_CXX_FLAGS_RELEASE
    "${CMAKE_CXX_FLAGS_RELEASE} -std=c++0x -stdlib=libc++")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO
    "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -std=c++0x -stdlib=libc++")
  set(CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS_DEBUG} -std=c++0x -stdlib=libc++")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_definitions(-std=c++11)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  execute_process(COMMAND "${CMAKE_C_COMPILER}" -dumpversion OUTPUT_VARIABLE GCC_VERSION)
  message(STATUS "GCC Version: ${GCC_VERSION}")
  if(GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
    add_definitions(-std=c++11) # Enable the new C++ standard
  else()
    add_definitions(-std=c++0x) # Enable the new C++ standard
  endif()
endif()


## Find the Boost library
option(Boost_USE_STATIC_LIBS "If true, will try to find static Boost first instead of dynamic." ON)
find_package(Boost COMPONENTS system)
if(NOT Boost_FOUND)
  unset(Boost_USE_STATIC_LIBS CACHE)
  option(Boost_USE_STATIC_LIBS "If true, will try to find static Boost first instead of dynamic." OFF)
  find_package(Boost COMPONENTS system)
  if(NOT Boost_FOUND)
    unset(Boost_USE_STATIC_LIBS CACHE)
  endif()
endif()
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIR})
  link_directories(${Boost_LIBRARY_DIRS})
  message("  ${Boost_LIBRARIES}\n")
else()
  message(FATAL_ERROR "You need boost to build this, set the BOOST_ROOT or BOOSTROOT env variables, or pass them to cmake")
endif()
add_definitions(-DBOOST_ALL_NO_LIB -D_SCL_SECURE_NO_WARNINGS)


## Setup all of the CMake build options
# Some option-defaults are changed by the build type
if(CMAKE_BUILD_TYPE MATCHES DEBUG)
  set(IS_DEBUG_BUILD ON)
else()
  set(IS_DEBUG_BUILD OFF)
endif()

# Build client library
option(BUILD_CLIENT "If on, will build the libastron-client library." ON)

# Build internal library
option(BUILD_INTERNAL "If on, will build the libastron-internal library." ON)

# Build binary unittests
option(BUILD_TESTS "If on, will build some example executables which can be tested with the python testsuite." ${IS_DEBUG_BUILD})

# Use 32-bit datagram length tags
option(USE_32BIT_DATAGRAMS "If on, datagrams and dclass fields will use 32-bit length tags instead of 16-bit." OFF)


## Configure the build from the select option
if(USE_32BIT_DATAGRAMS)
  add_definitions(-DASTRON_32BIT_DATAGRAMS)
  add_definitions(-DDCPARSER_32BIT_LENGTH_TAG)
endif()

add_library(dcparser
  src/dcparser/p3dcparser_composite1.cxx
  src/dcparser/p3dcparser_composite2.cxx
  src/dcparser/dcLexer.cxx
  src/dcparser/dcParser.cxx)

add_library(astron-cxx
  src/util/types.h
  src/util/Datagram.h
  src/util/DatagramIterator.h
  src/util/Connection.h
  src/util/Connection.cpp
  src/objects/DistributedObject.h
  src/objects/DistributedObject.cpp
  src/objects/ObjectFactory.h
  src/objects/ObjectFactory.cpp
  src/objects/ObjectRepository.h
  src/objects/ObjectRepository.cpp)
add_dependencies(astron-cxx dcparser)
target_link_libraries(astron-cxx dcparser ${Boost_LIBRARIES})

if(BUILD_CLIENT)
  add_library(astron-cxx-client
    src/client/ClientConnection.h
    src/client/ClientConnection.cpp
    src/client/ClientMessages.h
    src/client/ClientRepository.h)
  add_dependencies(astron-cxx-client astron-cxx)
  target_link_libraries(astron-cxx-client astron-cxx)
endif()

if(BUILD_INTERNAL)
  add_library(astron-cxx-internal
    src/internal/InternalConnection.h
    src/internal/InternalConnection.cpp
    src/internal/InternalMessages.h
    src/internal/InternalRepository.h
    src/internal/InternalRepository.cpp
    src/internal/Shard.h)
  add_dependencies(astron-cxx-internal astron-cxx)
  target_link_libraries(astron-cxx-internal astron-cxx)
endif()

if(BUILD_TESTS)
  add_executable(unittest-core src/unittest/main-core.cpp)
  add_dependencies(unittest-core astron-cxx)
  target_link_libraries(unittest-core astron-cxx)

  if(BUILD_CLIENT)
    add_executable(unittest-client src/unittest/main-internal.cpp)
    add_dependencies(unittest-client astron-cxx-internal)
    target_link_libraries(unittest-client astron-cxx-internal)
  endif()

  if(BUILD_INTERNAL)
    add_executable(unittest-internal src/unittest/main-internal.cpp)
    add_dependencies(unittest-internal astron-cxx-internal)
    target_link_libraries(unittest-internal astron-cxx-internal)
  endif()
endif()
