cmake_minimum_required (VERSION 2.6)
set_property (GLOBAL PROPERTY USE_FOLDERS ON)
project (libastron-cxx)

### Define extra windows defines ###
if (WIN32)
	add_definitions (-D_WIN32_WINDOWS)
endif()


### Use RelWithDebInfo by default ###
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
	set (CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

### Add GNU Make compiler flags for each build type ###
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall")
	set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g")

	### Release Flags -- warnings are errors, release code should not have warnings ###
	set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -Wall -Werror")
	
	### Debug flags, Wall
	set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
endif()


### Linux requires pthreads ###
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set (CMAKE_CXX_FLAGS "-pthread")
endif()


### BOOST Dependency -- required by yaml-cpp and networking ###
set (Boost_USE_STATIC_LIBS ON CACHE BOOL "If true, will try to find static Boost first instead of dynamic.")
find_package (Boost COMPONENTS system)
if (NOT Boost_FOUND)
	set (Boost_USE_STATIC_LIBS OFF CACHE BOOL "If true, will try to find static Boost first instead of dynamic.")
	find_package (Boost COMPONENTS system)
endif()
if (Boost_FOUND)
	include_directories (${Boost_INCLUDE_DIR})
	link_directories (${Boost_LIBRARY_DIRS})
	message ("  ${Boost_LIBRARIES}\n")
else ()
	message (FATAL_ERROR "You need boost to build this, set the BOOST_ROOT or BOOSTROOT env variables, or pass them to cmake")
endif()

add_definitions(
	-DBOOST_ALL_NO_LIB
	-D_SCL_SECURE_NO_WARNINGS
)

# Enable c++11 mode for g++
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	execute_process (COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
	message (STATUS "GCC Version: ${GCC_VERSION}")
	if(GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
		add_definitions (-std=c++11) # Enable the new C++ standard
	else()
		add_definitions (-std=c++0x) # Enable the new C++ standard
	endif()
endif()


### START BUILD OPTIONS ###
# Build client library
set (BUILD_CLIENT ON CACHE BOOL "If on, will build the libastron-client library.")

# Build internal library
set (BUILD_INTERNAL ON CACHE BOOL "If on, will build the libastron-internal library.")

# Build unittest executables
if(CMAKE_BUILD_TYPE MATCHES DEBUG)
    set(BUILD_TESTS ON CACHE BOOL "If true, will build some example executables which can be tested with the python testsuite.")
else()
	set(BUILD_TESTS OFF CACHE BOOL "If true, will build some example executables which can be tested with the python testsuite.")
endif()

# Use 32-bit datagram length tags
set (USE_32BIT_DATAGRAMS OFF CACHE BOOL "If on, datagrams and dclass fields will use 32-bit length tags instead of 16-bit.")
if (USE_32BIT_DATAGRAMS)
	add_definitions(-DASTRON_32BIT_DATAGRAMS)
	add_definitions(-DDCPARSER_32BIT_LENGTH_TAG)
endif()

### END BUILD OPTIONS ###


include_directories (src)

add_library (dcparser
	src/dcparser/p3dcparser_composite1.cxx 
	src/dcparser/p3dcparser_composite2.cxx 
	src/dcparser/dcLexer.cxx
	src/dcparser/dcParser.cxx
)

add_library (astron-cxx
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
	src/objects/ObjectRepository.cpp
)
add_dependencies (astron-cxx dcparser)
target_link_libraries (astron-cxx dcparser ${Boost_LIBRARIES})

if(BUILD_CLIENT)
	add_library (astron-cxx-client
		src/client/ClientConnection.h
		src/client/ClientConnection.cpp
		src/client/ClientMessages.h
		src/client/ClientRepository.h
	)
	add_dependencies (astron-cxx-client astron-cxx)
	target_link_libraries (astron-cxx-client astron-cxx)
endif()

if(BUILD_INTERNAL)
	add_library (astron-cxx-internal
		src/internal/InternalConnection.h
		src/internal/InternalConnection.cpp
		src/internal/InternalMessages.h
		src/internal/InternalRepository.h
		src/internal/InternalRepository.cpp
		src/internal/Shard.h
	)
	add_dependencies (astron-cxx-internal astron-cxx)
	target_link_libraries (astron-cxx-internal astron-cxx)
endif()

if(BUILD_TESTS)
	add_executable (unittest-core src/unittest/main-core.cpp)
	add_dependencies (unittest-core astron-cxx)
	target_link_libraries (unittest-core astron-cxx)

	if(BUILD_CLIENT)
		add_executable (unittest-client src/unittest/main-internal.cpp)
		add_dependencies (unittest-client astron-cxx-internal)
		target_link_libraries (unittest-client astron-cxx-internal)
	endif()

	if(BUILD_INTERNAL)
		add_executable (unittest-internal src/unittest/main-internal.cpp)
		add_dependencies (unittest-internal astron-cxx-internal)
		target_link_libraries (unittest-internal astron-cxx-internal)
	endif()
endif()
