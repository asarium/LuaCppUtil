
#ifndef LUA_UTIL_H
#define LUA_UTIL_H
#pragma once

#include <type_traits> //for std::underlying_type

#include "LuaCpp/LuaValue.hpp"
#include "LuaCpp/LuaTable.hpp"
#include "LuaCpp/LuaConvert.hpp"

namespace luacpp
{
	class LuaTable;

	namespace util
	{
		
		/**
		 * @brief Fills the given container with all key-value pairs.
		 * Each key-value pair is converted into the type given to the Container and then push_back is used to add it.
		 * 
		 * @param table The table to be used
		 * @param keyValueList The container which should be used
		 * @return void
		 * 
		 * @tparam Container Must be a container type with a push_back function and must have a value_type
		 * 	which exposes a first_type and second_type typedef (for example std::pair).
		 */
		template<typename Container>
		void tableListPairs(LuaTable& table, Container& keyValueList)
		{
			typedef typename Container::value_type::first_type key_type;
			typedef typename Container::value_type::second_type value_type;

			keyValueList.clear();

			LuaTableIterator iter = table.iterator();
			lua_State* L = table.luaState;

			while (iter.toNext())
			{
				value_type value = convert::popValue<value_type>(L);

				// lua_next gets confused if we use lua_tolstring here so copy the value and pop it
				lua_pushvalue(L, -1);
				key_type key = convert::popValue<key_type>(L);

				keyValueList.push_back(std::make_pair(key, value));
			}
		}

		/**
		 * @brief Fills the given list with all the values from the table.
		 * This will fill the list with the same value as @c ipairs function in lua.
		 * 
		 * @param table The table to be used
		 * @param list The list into which the values should be inserted
		 * @return void
		 * 
		 * @tparam Container The type of the container into which the values should be inserted,
		 * 	must have a value_type typedef so the lua values can be converted and a push_back function.
		 * 
		 * @exception LuaException May be thrown if an element of the table is not convertible to
		 * 	Container::value_type.
		 */
		template<typename Container>
		void tableToList(LuaTable& table, Container& list)
		{
			list.clear();

			size_t length = table.getLength();

			// Lua arrays begin at 1
			for (size_t i = 1; i <= length; ++i)
			{
				list.push_back(table.getValue<typename Container::value_type>(i));
			}
		}

		const char* getValueName(ValueType type);
	}
}

#endif // LUA_UTIL_H
