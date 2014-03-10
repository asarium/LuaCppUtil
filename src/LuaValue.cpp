#include "LuaCpp/LuaValue.hpp"
#include "LuaCpp/LuaException.hpp"

namespace
{
	using namespace luacpp;

	ValueType luaToEnumType(int luaType)
	{
		switch (luaType)
		{
		case LUA_TNONE:
			return ValueType::NONE;
		case LUA_TNIL:
			return ValueType::NIL;
		case LUA_TBOOLEAN:
			return ValueType::BOOLEAN;
		case LUA_TLIGHTUSERDATA:
			return ValueType::LIGHTUSERDATA;
		case LUA_TNUMBER:
			return ValueType::NUMBER;
		case LUA_TSTRING:
			return ValueType::STRING;
		case LUA_TTABLE:
			return ValueType::TABLE;
		case LUA_TFUNCTION:
			return ValueType::FUNCTION;
		case LUA_TUSERDATA:
			return ValueType::USERDATA;
		case LUA_TTHREAD:
			return ValueType::THREAD;

		default:
			return ValueType::NONE;
		}
	}
}

namespace luacpp
{
	LuaValue LuaValue::createNil(lua_State* L)
	{
		lua_pushnil(L);

		LuaValue val(L);

		val.setReference(LuaReference::create(L));

		return val;
	}

	LuaValue::LuaValue(lua_State* state) : luaState(state), luaType(ValueType::NONE)
	{
		if (state == nullptr)
		{
			throw LuaException("Lua state pointer is not valid!");
		}
	}

	LuaValue::LuaValue(const LuaValue& other) : luaState(other.luaState)
	{
		// Just copy the reference object from the other
		this->setReference(other.reference);
	}

	LuaValue::~LuaValue()
	{
	}

	void LuaValue::setReference(LuaReferencePtr reference)
	{
		this->reference = reference;

		if (this->reference->isValid())
		{
			this->reference->pushValue();

			luaState = reference->getState();
			this->luaType = luaToEnumType(lua_type(luaState, -1));

			lua_pop(luaState, 1);
		}
	}

	const LuaReferencePtr LuaValue::getReference() const
	{
		return reference;
	}

	ValueType LuaValue::getValueType() const
	{
		return luaType;
	}

	bool LuaValue::pushValue() const
	{
		if (this->reference->isValid())
		{
			this->reference->pushValue();
			return true;
		}
		else
		{
			return false;
		}
	}
}