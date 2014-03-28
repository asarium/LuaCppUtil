#include "LuaCpp/LuaFunction.hpp"
#include "LuaCpp/LuaException.hpp"

#include "LuaCpp/LuaHeaders.hpp"

namespace luacpp
{
	LuaFunction LuaFunction::createFromCFunction(lua_State* L, lua_CFunction function)
	{
		LuaFunction func;

		lua_pushcfunction(L, function);

		func.setReference(LuaReference::create(L));

		lua_pop(L, 1);

		return func;
	}

	LuaFunction LuaFunction::createFromCode(lua_State* L, std::string const& code, std::string const& name)
	{
		int err = luaL_loadbuffer(L, code.c_str(), code.length(), name.c_str());

		if (!err)
		{
			LuaFunction func;

			func.setReference(LuaReference::create(L));

			lua_pop(L, 1);

			return func;
		}
		else
		{
			// Get the error message
			size_t len;
			const char* err = lua_tolstring(L, -1, &len);

			lua_pop(L, 1);

			throw LuaException(std::string(err, len));
		}
	}

	LuaFunction::LuaFunction() : LuaValue(), isCFunction(false), errorFunction(nullptr)
	{
	}

	LuaFunction::LuaFunction(const LuaFunction& other) : LuaValue(other), errorFunction(nullptr)
	{
	}
	
	LuaFunction::~LuaFunction()
	{
	}

	bool LuaFunction::setEnvironment(const LuaTable& table)
	{
		if (!table.getReference()->isValid())
		{
			throw LuaException("Table reference is not valid!");
		}

		this->pushValue();
		table.pushValue();

		bool ret = lua_setfenv(luaState, -2) != 0;

		// Pop the function again
		lua_pop(luaState, 1);

		return ret;
	}

	LuaValueList LuaFunction::operator()(const LuaValueList& args)
	{
		return this->call(args);
	}

	void LuaFunction::setReference(LuaReferencePtr reference)
	{
		reference->pushValue();

		lua_State* L = reference->getState();
		
		if (lua_type(L, -1) != LUA_TFUNCTION)
		{
			lua_pop(L, 1);
			throw LuaException("Reference does not refere to a function!");
		}
		else
		{
			lua_pop(L, 1);
			LuaValue::setReference(reference);
		}
	}

	LuaValueList LuaFunction::call(const LuaValueList& args)
	{
		int err_idx = 0;
		int stackTop;

		if (errorFunction)
		{
			// push the error function
			errorFunction->pushValue();
			err_idx = lua_gettop(luaState);
			stackTop = err_idx;
		}
		else
		{
			stackTop = lua_gettop(luaState);
		}

		// Push the function onto the stack
		this->pushValue();

		// Push the arguments onto the stack
		for (LuaValueList::const_iterator iter = args.begin(); iter != args.end(); ++iter)
		{
			iter->pushValue();
		}

		// actually call the function now!
		int err = lua_pcall(luaState, args.size(), LUA_MULTRET, err_idx);

		if (!err)
		{
			int numReturn = lua_gettop(luaState) - stackTop;
			LuaValueList values;
			values.reserve(numReturn);

			LuaValue val;
			for (int i = 0; i < numReturn; ++i)
			{
				auto top = lua_gettop(luaState);
				if (convert::popValue(luaState, val))
				{
					top = lua_gettop(luaState);
					// Add values at the begin as the last return value is on top
					// of the stack.
					values.insert(values.begin(), val);
				}
			}

			if (err_idx != 0)
			{
				// Remove the error function
				lua_remove(luaState, err_idx);
			}

			return values;
		}
		else
		{
			// Throw exception with generated message
			LuaException exception(convert::popValue<std::string>(luaState));

			if (err_idx != 0)
			{
				// Pop the error function
				lua_pop(luaState, 1);
			}

			throw exception;
		}
	}
}