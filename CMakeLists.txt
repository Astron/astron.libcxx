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


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	execute_process (COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
	message (STATUS "GCC Version: ${GCC_VERSION}")
	if(GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
		add_definitions (-std=c++11) # Enable the new C++ standard
	else()
		add_definitions (-std=c++0x) # Enable the new C++ standard
	endif()
endif()


include_directories (src)
add_library (libastron-cxx
	${CLIENTAGENT_FILES}
)

add_library (dcparser
	src/dcparser/p3dcparser_composite1.cxx 
	src/dcparser/p3dcparser_composite2.cxx 
	src/dcparser/dcLexer.cxx
	src/dcparser/dcParser.cxx
)

add_library (libastron-cxx
	src/messages/messages.h
	src/messages/Datagram.h
	src/messages/DatagramIterator.h
	src/connection/Connection.h
	src/connection/NetworkClient.h
	src/connection/NetworkClient.cpp
)

add_dependencies (libastron-cxx dcparser)
target_link_libraries (libastron-cxx dcparser ${Boost_LIBRARIES})
