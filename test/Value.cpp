
#include "TestUtil.hpp"

#include "LuaCpp/LuaValue.hpp"

using namespace luacpp;

class LuaValueTest : public LuaStateTest
{};

TEST_F(LuaValueTest, SetReference)
{
	ScopedLuaStackTest stackTest(L);

	LuaValue val = LuaValue::createValue(L, "TestTest");

	lua_pushnumber(L, 42.0);

	val.setReference(LuaReference::create(L));

	lua_pop(L, 1);

	ASSERT_EQ(ValueType::NUMBER, val.getValueType());
	ASSERT_DOUBLE_EQ(42.0, val.getValue<double>());
}
