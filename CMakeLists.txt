cmake_minimum_required (VERSION 2.6)
set_property (GLOBAL PROPERTY USE_FOLDERS ON)
project (LibAstron++)

### Configure CMake to use our extra Find modules ###
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")


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


### Debug Flags -- compile in spam/debug log messages ###
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG_MESSAGES")


### OS Specific Settings ###
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")		### Setup library for Windows


else ()											### Setup library for Unix, Linux, OS X, etc


endif()


if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")		### Linux requires pthreads
	set (CMAKE_CXX_FLAGS "-pthread")
endif()




set (BUILD_TESTS OFF CACHE BOOL "If set to true, test files will be compiled in")

if(BUILD_TESTS)
	set (TEST_FILES
#tests
	)
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	execute_process (COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
	message (STATUS "GCC Version: ${GCC_VERSION}")
	if(GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
		add_definitions (-std=c++11) # Enable the new C++ standard
	else()
		add_definitions (-std=c++0x) # Enable the new C++ standard
	endif()
endif()


set (BUILD_CLIENTAGENT ON CACHE BOOL "If on, Client Agent will be built into binary")
if(BUILD_CLIENTAGENT)
	set (CLIENTAGENT_FILES
		src/AstronConnection.h
		src/AstronConnection.cpp
	)
endif()



include_directories (src)
include_directories (dependencies/asio/asio/include)
add_library (libastron++
	${CLIENTAGENT_FILES}
)

#source_group ("Header Files" FILES src/core/config.h src/core/global.h src/core/logger.h src/messagedirector/messagedirector.h)

add_library (dcparser 
	src/dcparser/p3dcparser_composite1.cxx 
	src/dcparser/p3dcparser_composite2.cxx 
	src/dcparser/dcLexer.cxx
	src/dcparser/dcParser.cxx
)

add_dependencies (libastron++ dcparser)
target_link_libraries (libastron++ dcparser)
