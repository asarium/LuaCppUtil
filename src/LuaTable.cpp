#include "LuaCpp/LuaException.hpp"
#include "LuaCpp/LuaTable.hpp"

namespace luacpp
{
	LuaTable LuaTable::create(lua_State* state)
	{
		LuaTable table;

		lua_newtable(state);

		table.setReference(LuaReference::create(state));

		lua_pop(state, 1);

		return table;
	}

	LuaTable::LuaTable() : LuaValue()
	{
	}

	LuaTable::LuaTable(const LuaTable& other) : LuaValue(other)
	{
	}

	LuaTable::~LuaTable()
	{
	}

	bool LuaTable::setMetatable(const LuaTable& table)
	{
		if (!table.getReference()->isValid())
		{
			throw LuaException("Meta table reference is not valid!");
		}

		this->pushValue();
		table.pushValue();

		lua_setmetatable(luaState, -2);

		lua_pop(luaState, 1);

		return true;
	}

	void LuaTable::setReference(luacpp::LuaReferencePtr reference)
	{
		reference->pushValue();

		lua_State* L = reference->getState();

		if (lua_type(L, -1) != LUA_TTABLE)
		{
			lua_pop(L, 1);
			throw LuaException("Reference does not refere to a table!");
		}
		else
		{
			lua_pop(L, 1);
			LuaValue::setReference(reference);
		}
	}

	size_t LuaTable::getLength()
	{
		this->pushValue();

		size_t length = lua_objlen(luaState, -1);

		lua_pop(luaState, 1);

		return length;
	}

	LuaTableIterator LuaTable::iterator()
	{
		this->pushValue();

		return LuaTableIterator(this);
	}

	LuaTableIterator::LuaTableIterator(LuaTable* parent) : parent(parent)
	{
		// Prepare the iteration
		lua_pushnil(parent->luaState);
	}

	bool LuaTableIterator::toNext()
	{
		if (lua_next(parent->luaState, -2) == 0)
		{
			// Pop the table we are iterating
			lua_pop(parent->luaState, 1);
			return false;
		}
		else
		{
			return true;
		}
	}
}