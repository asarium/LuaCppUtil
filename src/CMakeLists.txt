
set(SOURCES
	LuaFunction.cpp
	LuaTable.cpp
	LuaConvert.cpp
	LuaReference.cpp
	LuaValue.cpp
	LuaUtil.cpp
)

SET(HEADERS
	${INCLUDE_DIR}/LuaCpp/LuaFunction.hpp
	${INCLUDE_DIR}/LuaCpp/LuaTable.hpp
	${INCLUDE_DIR}/LuaCpp/LuaConvert.hpp
	${INCLUDE_DIR}/LuaCpp/LuaReference.hpp
	${INCLUDE_DIR}/LuaCpp/LuaValue.hpp
	${INCLUDE_DIR}/LuaCpp/LuaException.hpp
	${INCLUDE_DIR}/LuaCpp/LuaHeaders.hpp
	${INCLUDE_DIR}/LuaCpp/LuaUtil.hpp
)

add_library(luacpputil STATIC ${SOURCES} ${HEADERS})

find_package(Lua51 REQUIRED)
target_link_libraries(luacpputil ${LUA_LIBRARIES})
target_include_directories(luacpputil PUBLIC ${LUA_INCLUDE_DIR})

find_package(Boost REQUIRED)

target_include_directories(luacpputil PUBLIC ${Boost_INCLUDE_DIRS})

target_include_directories(luacpputil PUBLIC ${INCLUDE_DIR})


	