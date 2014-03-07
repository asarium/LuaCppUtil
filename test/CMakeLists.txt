
if(MSVC)
	set(gtest_force_shared_crt ON)
endif(MSVC)

add_subdirectory(gtest-1.7.0)

set_target_properties(gtest
	PROPERTIES
		FOLDER "test/gtest"
)
set_target_properties(gtest_main
	PROPERTIES
		FOLDER "test/gtest"
)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(TEST_SRCS
	Function.cpp
	Convert.cpp
	Table.cpp
	Reference.cpp
	Value.cpp
	Util.cpp
	TestUtil.hpp
)

add_executable(gtests ${TEST_SRCS})
target_link_libraries(gtests gtest_main gtest luacpputil)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(test gtests COMMENT "Running tests")

set_target_properties(gtests
	PROPERTIES
		FOLDER "test"
)
set_target_properties(test
	PROPERTIES
		FOLDER "test"
)
