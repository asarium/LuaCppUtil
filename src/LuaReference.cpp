
#include <LuaCpp/LuaReference.hpp>
#include <lauxlib.h>
#include <LuaCpp/LuaException.hpp>

#include "LuaCpp/LuaHeaders.hpp"

namespace luacpp
{
	LuaReferencePtr LuaReference::create(lua_State* state, int position)
	{
		if (state == nullptr)
		{
			throw LuaException("Need a valid lua state!");
		}

		lua_pushvalue(state, position);

		LuaReferencePtr ref(new LuaReference(state, luaL_ref(state, LUA_REGISTRYINDEX)));

		return ref;
	}

	LuaReferencePtr LuaReference::copy(LuaReferencePtr const& other)
	{
		other->pushValue();

		return create(other->luaState);
	}

	LuaReference::LuaReference(lua_State* state, int reference) :
		luaState(state), mReference(reference)
	{
		if (state == nullptr)
		{
			throw LuaException("Need a valid lua state!");
		}

		if (reference < 0)
		{
			throw LuaException("Reference must be greater than or equal to zero!");
		}
	}

	LuaReference::~LuaReference()
	{
		this->removeReference();
	}

	bool LuaReference::removeReference()
	{
		if (this->isValid())
		{
			luaL_unref(luaState, LUA_REGISTRYINDEX, mReference);
			mReference = -1;
			return true;
		}
		else
		{
			return false;
		}
	}

	int LuaReference::getReference() const
	{
		return mReference;
	}

	bool LuaReference::isValid() const
	{
		if (luaState == nullptr)
		{
			return false;
		}

		if (mReference < 0)
		{
			return false;
		}

		return true;
	}

	void LuaReference::pushValue() const
	{
		if (this->isValid())
		{
			lua_rawgeti(luaState, LUA_REGISTRYINDEX, this->getReference());
		}
	}
}