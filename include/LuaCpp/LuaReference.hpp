#ifndef LUA_REFERENCE_H
#define LUA_REFERENCE_H
#pragma once

#include "LuaCpp/LuaHeaders.hpp"

#include <boost/smart_ptr.hpp>

namespace luacpp
{
	class LuaReference;

	/**
	 * @brief A pointer to a lua reference
	 */
	typedef boost::shared_ptr<LuaReference> LuaReferencePtr;

	/**
	* @brief A lua-value reference.
	*
	* Wraps a reference to a lua-value and provides a way to handle multiple users of that reference and
	* automatic reference freeing.
	*/
	class LuaReference
	{
	private:
		/**
		* @brief Initializes a lua reference.
		*
		* Sets the lua_State which will be used to hold the reference and the actual reference value.
		*
		* @param state The lua_State where the reference points to a value.
		* @param reference The reference value, should be >= 0.
		*/
		LuaReference(lua_State* state, int reference);

		lua_State* luaState;
		int mReference;

	public:
		/**
		* @brief Creates a lua-reference.
		*
		* Copies the value at @c position and creates a reference to it. Also replaces
		* the value at that position with the copied value.
		*
		* @param state The state to create the reference in.
		* @param position The stack position of the value, defaults to the top of the stack (-1).
		* @return The LuaReference instance which got created.
		*/
		static LuaReferencePtr create(lua_State* state, int position = -1);

		/**
		 * @brief Copies another lua reference
		 * There is no copy-constructor as unintentional copying could lead to excessive creation and deletion of lua references
		 * 
		 * @param other The other pointer
		 * @return luacpp::LuaReferencePtr The new reference pointer
		 */
		static LuaReferencePtr copy(const LuaReferencePtr& other);

		/**
		* @brief Default constructor, initializes an invalid reference
		*/
		LuaReference() :
			luaState(nullptr), mReference(-1)
		{
		}

		/**
		* @brief Releases the lua reference.
		*/
		~LuaReference();

		lua_State* getState() { return luaState; }

		/**
		* @brief Gets the actual reference number.
		* @return The reference number
		*/
		int getReference() const;

		/**
		* @brief Checks if the reference is valid.
		* @return @c true when valid, @c false otherwise.
		*/
		bool isValid() const;

		/**
		* @brief Removes the Lua reference
		* @return @c true when the reference was removed @c false otherwise
		*/
		bool removeReference();

		/**
		* @brief Pushes the referenced value onto the stack.
		*/
		void pushValue() const;
	};
}

#endif // LUA_REFERENCE_H