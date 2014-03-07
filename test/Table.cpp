
#include "TestUtil.hpp"

#include "LuaCpp/LuaTable.hpp"

using namespace luacpp;

class LuaTableTest : public LuaStateTest
{
};


TEST_F(LuaTableTest, AddValue)
{
	LuaTable table = LuaTable::create(L);
	table.addValue("key", "value");
	
	ASSERT_TRUE(table.pushValue());

	lua_getfield(L, -1, "key");

	ASSERT_TRUE(lua_isstring(L, -1) == 1);
	ASSERT_STREQ("value", lua_tostring(L, -1));
}

TEST_F(LuaTableTest, SetReference)
{
	LuaTable table = LuaTable::create(L);

	lua_pushliteral(L, "abc");
	ASSERT_THROW(table.setReference(LuaReference::create(L)), LuaException);

	lua_newtable(L);
	ASSERT_NO_THROW(table.setReference(LuaReference::create(L)));
}

TEST_F(LuaTableTest, GetValue)
{
	lua_newtable(L);
	lua_pushstring(L, "value");
	lua_setfield(L, -2, "key");

	LuaTable table;
	
	EXPECT_TRUE(convert::popValue(L, table));

	std::string val;
	ASSERT_TRUE(table.getValue("key", val));
	ASSERT_STREQ("value", val.c_str());
}

TEST_F(LuaTableTest, GetLength)
{
	lua_newtable(L);

	lua_pushnumber(L, 1);
	lua_pushstring(L, "value1");
	lua_settable(L, -3);

	lua_pushnumber(L, 2);
	lua_pushstring(L, "value2");
	lua_settable(L, -3);

	lua_pushnumber(L, 3);
	lua_pushstring(L, "value3");
	lua_settable(L, -3);

	LuaTable table;

	EXPECT_TRUE(convert::popValue(L, table));

	ASSERT_EQ(3, table.getLength());
}

TEST_F(LuaTableTest, Iterator)
{
	lua_newtable(L);

	lua_pushnumber(L, 1);
	lua_pushstring(L, "value1");
	lua_settable(L, -3);

	lua_pushnumber(L, 2);
	lua_pushstring(L, "value2");
	lua_settable(L, -3);

	lua_pushnumber(L, 3);
	lua_pushstring(L, "value3");
	lua_settable(L, -3);

	lua_pushstring(L, "value4");
	lua_setfield(L, -2, "key");

	LuaTable table;

	EXPECT_TRUE(convert::popValue(L, table));

	table.iterator();
}
