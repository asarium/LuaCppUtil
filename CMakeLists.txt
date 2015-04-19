
cmake_minimum_required(VERSION 2.8.11)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

macro(set_policy policy value)
	if (POLICY ${policy})
		cmake_policy(SET ${policy} ${value})
	endif ()
endmacro(set_policy)

# Set a few policies
set_policy(CMP0051 OLD)
set_policy(CMP0053 NEW)
set_policy(CMP0054 NEW)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(LUACPP_BUILD_TESTS "Build tests" OFF)

PROJECT(LuaCppUtil)

include(toolchain)

SET(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
add_subdirectory(src)

if (LUACPP_BUILD_TESTS)
	add_subdirectory(test)
endif(LUACPP_BUILD_TESTS)
