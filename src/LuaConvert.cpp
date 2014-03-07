#include "LuaCpp/LuaException.hpp"
#include "LuaCpp/LuaConvert.hpp"
#include "LuaCpp/LuaTable.hpp"
#include "LuaCpp/LuaFunction.hpp"
#include "LuaCpp/LuaReference.hpp"

#include "LuaCpp/LuaHeaders.hpp"

namespace
{
	bool isValidIndex(lua_State* state, int index)
	{
		if (1 <= abs(index) && abs(index) <= lua_gettop(state))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

namespace luacpp
{
	namespace convert
	{
		template<>
		void pushValue<double>(lua_State* luaState, const double& value)
		{
			lua_pushnumber(luaState, value);
		}

		template<>
		void pushValue<float>(lua_State* luaState, const float& value)
		{
			lua_pushnumber(luaState, value);
		}

		template<>
		void pushValue<int>(lua_State* luaState, const int& value)
		{
			lua_pushnumber(luaState, value);
		}

		template<>
		void pushValue<size_t>(lua_State* luaState, const size_t& value)
		{
			lua_pushnumber(luaState, value);
		}

		template<>
		void pushValue<std::string>(lua_State* luaState, const std::string& value)
		{
			lua_pushlstring(luaState, value.c_str(), value.size());
		}

		template<>
		void pushValue<const char*>(lua_State* luaState, const char* const& value)
		{
			lua_pushstring(luaState, value);
		}

		template<>
		void pushValue<bool>(lua_State* luaState, const bool& value)
		{
			lua_pushboolean(luaState, value);
		}

		template<>
		void pushValue<lua_CFunction>(lua_State* luaState,
		                              const lua_CFunction& value)
		{
			lua_pushcfunction(luaState, value);
		}

		template<>
		void pushValue<LuaValue>(lua_State* luaState, const LuaValue& value)
		{
			if (luaState != value.luaState)
			{
				throw LuaException("Lua state mismatch!");
			}

			value.pushValue();
		}

		template<>
		bool popValue<double>(lua_State* luaState, double& target,
		                      int stackposition, bool remove)
		{
			if (!isValidIndex(luaState, stackposition))
			{
				throw LuaException("Specified stack position is not valid!");
			}

			if (!lua_isnumber(luaState, stackposition))
			{
				return false;
			}
			else
			{
				target = lua_tonumber(luaState, stackposition);

				if (remove)
				{
					lua_remove(luaState, stackposition);
				}

				return true;
			}
		}

		template<>
		bool popValue<float>(lua_State* luaState, float& target,
		                     int stackposition, bool remove)
		{
			if (!isValidIndex(luaState, stackposition))
			{
				throw LuaException("Specified stack position is not valid!");
			}

			if (!lua_isnumber(luaState, stackposition))
			{
				return false;
			}
			else
			{
				target = static_cast<float>(lua_tonumber(luaState, stackposition));

				if (remove)
				{
					lua_remove(luaState, stackposition);
				}

				return true;
			}
		}

		template<>
		bool popValue<int>(lua_State* luaState, int& target,
		                   int stackposition, bool remove)
		{
			if (!isValidIndex(luaState, stackposition))
			{
				throw LuaException("Specified stack position is not valid!");
			}

			if (!lua_isnumber(luaState, stackposition))
			{
				return false;
			}
			else
			{
				target = static_cast<int>(lua_tonumber(luaState, stackposition));

				if (remove)
				{
					lua_remove(luaState, stackposition);
				}

				return true;
			}
		}

		template<>
		bool popValue<size_t>(lua_State* luaState, size_t& target,
			int stackposition, bool remove)
		{
			if (!isValidIndex(luaState, stackposition))
			{
				throw LuaException("Specified stack position is not valid!");
			}

			if (!lua_isnumber(luaState, stackposition))
			{
				return false;
			}
			else
			{
				target = static_cast<size_t>(lua_tonumber(luaState, stackposition));

				if (remove)
				{
					lua_remove(luaState, stackposition);
				}

				return true;
			}
		}

		template<>
		bool popValue<std::string>(lua_State* luaState, std::string& target,
		                           int stackposition, bool remove)
		{
			if (!isValidIndex(luaState, stackposition))
			{
				throw LuaException("Specified stack position is not valid!");
			}

			if (!lua_isstring(luaState, stackposition))
			{
				return false;
			}
			else
			{
				size_t size;
				const char* string = lua_tolstring(luaState, stackposition, &size);
				target.assign(string, size);

				if (remove)
				{
					lua_remove(luaState, stackposition);
				}

				return true;
			}
		}

		template<>
		bool popValue<bool>(lua_State* luaState, bool& target,
		                    int stackposition, bool remove)
		{
			if (!isValidIndex(luaState, stackposition))
			{
				throw LuaException("Specified stack position is not valid!");
			}

			if (!lua_isboolean(luaState, stackposition))
			{
				return false;
			}
			else
			{
				target = lua_toboolean(luaState, stackposition) != 0;

				if (remove)
				{
					lua_remove(luaState, stackposition);
				}

				return true;
			}
		}

		template<>
		bool popValue<lua_CFunction>(lua_State* luaState,
		                             lua_CFunction& target, int stackposition, bool remove)
		{
			if (!isValidIndex(luaState, stackposition))
			{
				throw LuaException("Specified stack position is not valid!");
			}

			if (!lua_iscfunction(luaState, stackposition))
			{
				return false;
			}
			else
			{
				target = lua_tocfunction(luaState, stackposition);

				if (remove)
				{
					lua_remove(luaState, stackposition);
				}

				return true;
			}
		}

		template<>
		bool popValue<LuaTable>(lua_State* luaState, LuaTable& target,
		                        int stackposition, bool remove)
		{
			if (!isValidIndex(luaState, stackposition))
			{
				throw LuaException("Specified stack position is not valid!");
			}

			if (target.luaState != nullptr && luaState != target.luaState)
			{
				throw LuaException("Lua state mismatch detected!");
			}

			if (!lua_istable(luaState, stackposition))
			{
				return false;
			}
			else
			{
				target.setReference(LuaReference::create(luaState, stackposition));

				if (remove)
				{
					lua_remove(luaState, stackposition);
				}

				return true;
			}
		}

		template<>
		bool popValue<LuaFunction>(lua_State* luaState, LuaFunction& target,
		                           int stackposition, bool remove)
		{
			if (!isValidIndex(luaState, stackposition))
			{
				throw LuaException("Specified stack position is not valid!");
			}

			if (target.luaState != nullptr && luaState != target.luaState)
			{
				throw LuaException("Lua state mismatch detected!");
			}

			if (!lua_isfunction(luaState, stackposition))
			{
				return false;
			}
			else
			{
				target.setReference(LuaReference::create(luaState, stackposition));

				if (remove)
				{
					lua_remove(luaState, stackposition);
				}

				return true;
			}
		}

		template<>
		bool popValue<LuaValue>(lua_State* luaState, LuaValue& target,
		                        int stackposition, bool remove)
		{
			if (!isValidIndex(luaState, stackposition))
			{
				throw LuaException("Specified stack position is not valid!");
			}

			if (target.luaState != nullptr && luaState != target.luaState)
			{
				throw LuaException("Lua state mismatch detected!");
			}

			target.setReference(LuaReference::create(luaState, stackposition));

			if (remove)
			{
				lua_remove(luaState, stackposition);
			}

			return true;
		}
	}
}