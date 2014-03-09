
cmake_minimum_required(VERSION 2.8.11)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(LUACPP_BUILD_TESTS "Build tests" OFF)

PROJECT(LuaCppUtil)

include(toolchain)

SET(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
add_subdirectory(src)

if (LUACPP_BUILD_TESTS)
	add_subdirectory(test)
endif(LUACPP_BUILD_TESTS)
