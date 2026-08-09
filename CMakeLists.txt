cmake_minimum_required(VERSION 3.20)
project(marco VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE MARCO_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE MARCO_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

add_library(marco STATIC ${MARCO_SOURCES} ${MARCO_HEADERS})

target_include_directories(marco
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(marco PRIVATE
	-Wall -Wextra -Wpedantic
)

option(MARCO_BUILD_EXAMPLES "Build marco example executables" ON)
option(MARCO_BUILD_TESTS "Build marco tests" ON)

if(MARCO_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()

if(MARCO_BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()