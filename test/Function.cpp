
#include "TestUtil.hpp"

#include "LuaCpp/LuaFunction.hpp"
#include "LuaCpp/LuaException.hpp"
#include "LuaCpp/LuaConvert.hpp"

using namespace luacpp;

namespace
{
	int testCFunction(lua_State* L)
	{
		lua_pushnumber(L, 42.0);
		return 1;
	}

	int testErrorFunction(lua_State* L)
	{
		lua_pushliteral(L, "TestError");
		return 1;
	}
}

class LuaFunctionTest : public LuaStateTest
{
};

TEST_F(LuaFunctionTest, CreateFromCode)
{
	{
		ASSERT_THROW(LuaFunction::createFromCode(L, "/\\invalid"), luacpp::LuaException);
	}
	{
		ASSERT_NO_THROW(LuaFunction::createFromCode(L, "return 1 + 1"));
	}
}

TEST_F(LuaFunctionTest, Call)
{
	{
		// Test execution with failure
		LuaFunction function = LuaFunction::createFromCode(L, "invalid()");

		ASSERT_THROW(function.call(), luacpp::LuaException);
		ASSERT_THROW(function(), luacpp::LuaException);
	}
	{
		// Test execution without failure
		LuaFunction function = LuaFunction::createFromCode(L, "local a = 1");

		ASSERT_NO_THROW(function.call());
		ASSERT_NO_THROW(function());
	}
	{
		// Test execution without failure
		LuaFunction function = LuaFunction::createFromCode(L, "return 'abc', 5");


		LuaValueList returnValues = function();

		ASSERT_EQ(luacpp::ValueType::STRING, returnValues[0].getValueType());
		ASSERT_EQ(luacpp::ValueType::NUMBER, returnValues[1].getValueType());

		ASSERT_STREQ("abc", returnValues[0].getValue<std::string>().c_str());
		ASSERT_EQ(5, returnValues[1].getValue<int>());
	}
	{
		lua_getglobal(L, "type");

		LuaFunction func;
		ASSERT_TRUE(convert::popValue(L, func));

		LuaValue arg = LuaValue::createValue(L, "testString");
		LuaValueList returnVals = func({ arg });

		ASSERT_EQ(1, returnVals.size());
		ASSERT_EQ(ValueType::STRING, returnVals[0].getValueType());
		ASSERT_STREQ("string", returnVals[0].getValue<std::string>().c_str());
	}
}

TEST_F(LuaFunctionTest, SetEnvironment)
{
	{
		// Setup environment table
		LuaTable envionment = LuaTable::create(L);
		envionment.addValue("key", "Test");

		LuaFunction func = LuaFunction::createFromCode(L, "return key");

		func.setEnvironment(envionment);

		LuaValueList returnVals = func();

		ASSERT_EQ(1, returnVals.size());
		ASSERT_EQ(ValueType::STRING, returnVals[0].getValueType());

		ASSERT_STREQ("Test", returnVals[0].getValue<std::string>().c_str());
	}
}

TEST_F(LuaFunctionTest, SetCFunction)
{
	{
		LuaFunction func = LuaFunction::createFromCFunction(L, testCFunction);

		LuaValueList retVals = func();

		ASSERT_EQ(1, retVals.size());
		ASSERT_EQ(ValueType::NUMBER, retVals[0].getValueType());
		ASSERT_DOUBLE_EQ(42.0, retVals[0].getValue<double>());
	}
}

TEST_F(LuaFunctionTest, SetErrorFunction)
{
	{
		LuaFunction func = LuaFunction::createFromCode(L, "invalid()");
		func.setErrorFunction(&testErrorFunction);

		try
		{
			func();
			FAIL();
		}
		catch (const LuaException& err)
		{
			ASSERT_STREQ("TestError", err.what());
		}
	}
}

TEST_F(LuaFunctionTest, SetReference)
{
	{
		LuaFunction func = LuaFunction::createFromCode(L, "invalid()");
		
		lua_pushliteral(L, "abc");
		ASSERT_THROW(func.setReference(LuaReference::create(L)), LuaException);

		lua_getglobal(L, "type");
		ASSERT_NO_THROW(func.setReference(LuaReference::create(L)));
	}
}
