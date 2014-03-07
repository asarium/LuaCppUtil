
#include "TestUtil.hpp"

#include "LuaCpp/LuaReference.hpp"

using namespace luacpp;

class LuaReferenceTest : public LuaStateTest
{};

TEST_F(LuaReferenceTest, Create)
{
	{
		lua_pushboolean(L, 1);
		lua_pushnumber(L, 42.0);

		auto top = lua_gettop(L);

		auto ref = LuaReference::create(L, -2);

		// This may not modify the stack!
		ASSERT_EQ(top, lua_gettop(L));

		ref->pushValue();

		ASSERT_TRUE(lua_isboolean(L, -1) == 1);
		ASSERT_TRUE(lua_toboolean(L, -1) == 1);
	}
}

TEST_F(LuaReferenceTest, IsValid)
{
	{
		LuaReference ref;

		ASSERT_FALSE(ref.isValid());
	}
	{
		lua_pushboolean(L, 1);

		LuaReferencePtr refPtr = LuaReference::create(L);

		ASSERT_TRUE(refPtr->isValid());

		refPtr->removeReference();

		ASSERT_FALSE(refPtr->isValid());
	}
}

TEST_F(LuaReferenceTest, RemoveReference)
{
	lua_pushboolean(L, 1);

	LuaReferencePtr refPtr = LuaReference::create(L);

	ASSERT_TRUE(refPtr->removeReference());

	ASSERT_FALSE(refPtr->removeReference());
}

TEST_F(LuaReferenceTest, PushValue)
{
	lua_pushboolean(L, 1);

	LuaReferencePtr refPtr = LuaReference::create(L);

	lua_pop(L, 1);

	refPtr->pushValue();

	ASSERT_TRUE(lua_isboolean(L, -1) == 1);
	ASSERT_EQ(1, lua_toboolean(L, -1));
}
