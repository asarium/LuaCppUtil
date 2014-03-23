
#include "TestUtil.hpp"

#include "LuaCpp/LuaConvert.hpp"
#include <LuaCpp/LuaTable.hpp>
#include <LuaCpp/LuaFunction.hpp>

using namespace luacpp;
using namespace convert;

namespace
{
	int testCFunction(lua_State* state)
	{
		return 0;
	}
}

class LuaConvertTest : public LuaStateTest
{
};

TEST_F(LuaConvertTest, PushDouble)
{
	pushValue(L, 1.0);

	ASSERT_TRUE(lua_isnumber(L, -1) == 1);
	ASSERT_DOUBLE_EQ(1.0, lua_tonumber(L, -1));

	lua_pop(L, 1);
}

TEST_F(LuaConvertTest, PushFloat)
{
	pushValue(L, 1.0f);

	ASSERT_TRUE(lua_isnumber(L, -1) == 1);
	ASSERT_DOUBLE_EQ(1.0, lua_tonumber(L, -1));

	lua_pop(L, 1);
}

TEST_F(LuaConvertTest, PushInt)
{
	pushValue(L, 1);

	ASSERT_TRUE(lua_isnumber(L, -1) == 1);
	ASSERT_DOUBLE_EQ(1.0, lua_tonumber(L, -1));

	lua_pop(L, 1);
}

TEST_F(LuaConvertTest, PushSizeT)
{
	pushValue(L, static_cast<size_t>(1));

	ASSERT_TRUE(lua_isnumber(L, -1) == 1);
	ASSERT_DOUBLE_EQ(1.0, lua_tonumber(L, -1));

	lua_pop(L, 1);
}

TEST_F(LuaConvertTest, PushStdString)
{
	std::string testStr("TestTest");
	pushValue(L, testStr);

	ASSERT_TRUE(lua_isstring(L, -1) == 1);
	ASSERT_STREQ("TestTest", lua_tostring(L, -1));

	lua_pop(L, 1);
}

TEST_F(LuaConvertTest, PushString)
{
	const char* string = "TestTest";
	pushValue(L, string);

	ASSERT_TRUE(lua_isstring(L, -1) == 1);
	ASSERT_STREQ(string, lua_tostring(L, -1));

	lua_pop(L, 1);
}

TEST_F(LuaConvertTest, PushBool)
{
	pushValue(L, true);

	ASSERT_TRUE(lua_isboolean(L, -1) == 1);
	ASSERT_TRUE(lua_toboolean(L, -1) == 1);

	lua_pop(L, 1);

	pushValue(L, false);

	ASSERT_TRUE(lua_isboolean(L, -1) == 1);
	ASSERT_FALSE(lua_toboolean(L, -1) == 1);

	lua_pop(L, 1);
}

TEST_F(LuaConvertTest, PushCFunction)
{
	pushValue(L, &testCFunction);

	ASSERT_TRUE(lua_iscfunction(L, -1) == 1);

	lua_pop(L, 1);
}

TEST_F(LuaConvertTest, PushValue)
{
	LuaTable table = LuaTable::create(L);

	pushValue<LuaValue>(L, table);

	ASSERT_TRUE(lua_istable(L, -1) == 1);

	lua_pop(L, 1);
}

TEST_F(LuaConvertTest, PushFunction)
{
	LuaFunction func = LuaFunction::createFromCode(L, "return 0");

	pushValue<LuaValue>(L, func);

	ASSERT_TRUE(lua_isfunction(L, -1) == 1 && !lua_iscfunction(L, -1));

	lua_pop(L, 1);
}

TEST_F(LuaConvertTest, PopDouble)
{
	{
		lua_pushnumber(L, 1.0);

		double target;
		ASSERT_TRUE(popValue(L, target));

		ASSERT_DOUBLE_EQ(1.0, target);
	}
	{
		lua_pushnumber(L, 1.0);

		double target;
		ASSERT_TRUE(popValue(L, target, -1, false));

		ASSERT_DOUBLE_EQ(1.0, target);
		ASSERT_TRUE(lua_isnumber(L, -1) == 1);

		lua_pop(L, 1);
	}
	{
		lua_pushboolean(L, 1);

		double target;
		ASSERT_FALSE(popValue(L, target));

		lua_pop(L, 1);
	}
	{
		lua_pushnumber(L, 1.0);

		double target = popValue<double>(L);

		ASSERT_DOUBLE_EQ(1.0, target);
	}
	{
		lua_pushnumber(L, 1.0);

		double target = popValue<double>(L, -1, false);

		ASSERT_DOUBLE_EQ(1.0, target);
		ASSERT_TRUE(lua_isnumber(L, -1) == 1);

		lua_pop(L, 1);
	}
	{
		lua_pushliteral(L, "TestTest");

		ASSERT_THROW(popValue<double>(L), LuaException);

		lua_pop(L, 1);
	}
}

TEST_F(LuaConvertTest, PopFloat)
{
	{
		lua_pushnumber(L, 1.0);

		float target;
		ASSERT_TRUE(popValue(L, target));

		ASSERT_FLOAT_EQ(1.0f, target);
	}
	{
		lua_pushnumber(L, 1.0);

		float target;
		ASSERT_TRUE(popValue(L, target, -1, false));

		ASSERT_FLOAT_EQ(1.0f, target);
		ASSERT_TRUE(lua_isnumber(L, -1) == 1);

		lua_pop(L, 1);
	}
	{
		lua_pushboolean(L, 1);

		float target;
		ASSERT_FALSE(popValue(L, target));

		lua_pop(L, 1);
	}
}

TEST_F(LuaConvertTest, PopInt)
{
	{
		lua_pushnumber(L, 1.0);

		int target;
		ASSERT_TRUE(popValue(L, target));

		ASSERT_EQ(1, target);
	}
	{
		lua_pushnumber(L, 1.0);

		int target;
		ASSERT_TRUE(popValue(L, target, -1, false));

		ASSERT_EQ(1, target);
		ASSERT_TRUE(lua_isnumber(L, -1) == 1);

		lua_pop(L, 1);
	}
	{
		lua_pushboolean(L, 1);

		int target;
		ASSERT_FALSE(popValue(L, target));

		lua_pop(L, 1);
	}
}

TEST_F(LuaConvertTest, PopSizeT)
{
	{
		lua_pushnumber(L, 1.0);

		size_t target;
		ASSERT_TRUE(popValue(L, target));

		ASSERT_EQ(1, target);
	}
	{
		lua_pushnumber(L, 1.0);

		size_t target;
		ASSERT_TRUE(popValue(L, target, -1, false));

		ASSERT_EQ(1, target);
		ASSERT_TRUE(lua_isnumber(L, -1) == 1);

		lua_pop(L, 1);
	}
	{
		lua_pushboolean(L, 1);

		size_t target;
		ASSERT_FALSE(popValue(L, target));

		lua_pop(L, 1);
	}
}

TEST_F(LuaConvertTest, PopStdString)
{
	{
		lua_pushliteral(L, "TestTest");

		std::string target;
		ASSERT_TRUE(popValue(L, target));

		ASSERT_STREQ("TestTest", target.c_str());
	}
	{
		lua_pushliteral(L, "TestTest");

		std::string target;
		ASSERT_TRUE(popValue(L, target, -1, false));

		ASSERT_STREQ("TestTest", target.c_str());
		ASSERT_TRUE(lua_isstring(L, -1) == 1);

		lua_pop(L, 1);
	}
	{
		lua_pushboolean(L, 1);

		std::string target;
		ASSERT_FALSE(popValue(L, target));

		lua_pop(L, 1);
	}
}

TEST_F(LuaConvertTest, PopBool)
{
	{
		lua_pushboolean(L, 1);

		bool target;
		ASSERT_TRUE(popValue(L, target));

		ASSERT_TRUE(target);
	}
	{
		lua_pushboolean(L, 1);

		bool target;
		ASSERT_TRUE(popValue(L, target, -1, false));

		ASSERT_TRUE(target);
		ASSERT_TRUE(lua_isboolean(L, -1) == 1);

		lua_pop(L, 1);
	}
	{
		lua_pushnumber(L, 1.0);

		bool target;
		ASSERT_FALSE(popValue(L, target));

		lua_pop(L, 1);
	}
}

TEST_F(LuaConvertTest, PopCFunction)
{
	{
		lua_pushcfunction(L, &testCFunction);

		lua_CFunction target;
		ASSERT_TRUE(popValue(L, target));

		ASSERT_EQ(target, &testCFunction);
	}
	{
		lua_pushcfunction(L, &testCFunction);

		lua_CFunction target;
		ASSERT_TRUE(popValue(L, target, -1, false));

		ASSERT_EQ(target, &testCFunction);
		ASSERT_TRUE(lua_iscfunction(L, -1) == 1);

		lua_pop(L, 1);
	}
	{
		lua_pushnumber(L, 1.0);

		lua_CFunction target;
		ASSERT_FALSE(popValue(L, target));

		lua_pop(L, 1);
	}
}

TEST_F(LuaConvertTest, PopLuaTable)
{
	{
		lua_newtable(L);

		LuaTable target;
		ASSERT_TRUE(popValue(L, target));

		ASSERT_EQ(ValueType::TABLE, target.getValueType());
	}
	{
		lua_newtable(L);

		LuaTable target;
		ASSERT_TRUE(popValue(L, target, -1, false));

		ASSERT_EQ(ValueType::TABLE, target.getValueType());
		ASSERT_TRUE(lua_istable(L, -1) == 1);

		lua_pop(L, 1);
	}
	{
		lua_pushnumber(L, 1.0);

		LuaTable target;
		ASSERT_FALSE(popValue(L, target));

		lua_pop(L, 1);
	}
}

TEST_F(LuaConvertTest, PopLuaFunction)
{
	{
		// Put the print function onto the stack
		lua_getglobal(L, "print");

		LuaFunction target;
		ASSERT_TRUE(popValue(L, target));

		ASSERT_EQ(ValueType::FUNCTION, target.getValueType());
	}
	{
		lua_getglobal(L, "print");

		LuaFunction target;
		ASSERT_TRUE(popValue(L, target, -1, false));

		ASSERT_EQ(ValueType::FUNCTION, target.getValueType());
		ASSERT_TRUE(lua_isfunction(L, -1) == 1);

		lua_pop(L, 1);
	}
	{
		lua_pushnumber(L, 1.0);

		LuaFunction target;
		ASSERT_FALSE(popValue(L, target));

		lua_pop(L, 1);
	}
}

TEST_F(LuaConvertTest, PopLuaValue)
{
	{
		lua_pushnumber(L, 1.0);

		LuaValue target(L);
		ASSERT_TRUE(popValue(L, target));

		ASSERT_EQ(ValueType::NUMBER, target.getValueType());
	}
	{
		lua_pushnumber(L, 1.0);

		LuaValue target(L);
		ASSERT_TRUE(popValue(L, target, -1, false));

		ASSERT_EQ(ValueType::NUMBER, target.getValueType());
		ASSERT_TRUE(lua_isnumber(L, -1) == 1);

		lua_pop(L, 1);
	}
}
