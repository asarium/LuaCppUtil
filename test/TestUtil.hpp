
#ifndef TEST_UTIL_H
#define TEST_UTIL_H
#pragma once

#include <gtest/gtest.h>

#include <LuaCpp/LuaHeaders.hpp>

class LuaStateTest : public testing::Test
{
public:
	lua_State* L;

	virtual void SetUp() override
	{
		L = luaL_newstate();
		luaL_openlibs(L);
	}

	virtual void TearDown() override
	{
		ASSERT_EQ(0, lua_gettop(L));

		lua_close(L);
	}
};

#endif // TEST_UTIL_H
