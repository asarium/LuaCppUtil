
#ifndef LUAVALUE_H
#define LUAVALUE_H
#pragma once

#include <cstdint>
#include <csetjmp>

#include <LuaCpp/LuaConvert.hpp>
#include <LuaCpp/LuaReference.hpp>
#include <LuaCpp/LuaException.hpp>

#include "LuaCpp/LuaHeaders.hpp"

#include <boost/smart_ptr.hpp>

namespace luacpp
{
	/**
	 * @brief The values of a LuaValue 
	 */
	enum class ValueType : uint8_t
	{
		NONE,
		NIL,
		BOOLEAN,
		LIGHTUSERDATA,
		STRING,
		NUMBER,
		TABLE,
		FUNCTION,
		USERDATA,
		THREAD
	};

	/**
	 * @brief Represents a Lua-value
	 *
	 * This class holds a reference to a lua value and provides type checking to ensure that the value is still the same.
	 */
	class LuaValue
	{
	public:
		/**
		 * @brief Creates a LuaValue.
		 * This is done by pushing the value onto the lua stack and creating a reference to it.
		 * 
		 * @param state The lua state
		 * @param value The value to be referenced
		 * @return luacpp::LuaValue A new LuaValue instance which references the specified value
		 */
		template<class ValueType>
		static LuaValue createValue(lua_State* state, const ValueType& value)
		{
			LuaValue retVal(state);

			convert::pushValue(state, value);

			retVal.setReference(LuaReference::create(state));

			// Remove the value again
			lua_pop(state, 1);

			return retVal;
		}

		static LuaValue createNil(lua_State* L);

		/**
		 * @brief Default constructor, creates an invalid LuaValue
		 */
		LuaValue() : luaState(nullptr), luaType(ValueType::NONE) {}

		/**
		 * @brief Initializes the lua value
		 *
		 * The instance does not point to a lua-value after the constructor has finished. To reference
		 * a value use #setReference(LuaReferencePtr)
		 *
		 * @param state The lua state
		 */
		LuaValue(lua_State* state);

		/**
		 * @brief Copy-constructor
		 * @param other The other LuaValue.
		 */
		LuaValue(const LuaValue& other);

		/**
		 * @brief Releases the reference
		 */
		virtual ~LuaValue();

		/**
		 * @brief Sets a new LuaReference.
		 *
		 * @param reference The new lua reference.
		 */
		virtual void setReference(LuaReferencePtr reference);

		/**
		 * @brief Gets the LuaReference.
		 *
		 * This reference is used to reference the actual lua value.
		 *
		 * @return The LuaReference instance.
		 */
		virtual const LuaReferencePtr getReference() const;

		/**
		 * @brief Gets the lua type of this value.
		 * @return One of the LUA_T* defines.
		 */
		virtual ValueType getValueType() const;

		/**
		 * @brief Checks if the value is of the specified type.
		 * 
		 * @return bool @c true when it is, @c false if it isn't.
		 */
		inline bool is(ValueType check) const { return luaType == check; }

		/**
		 * @brief Sets a new value, possible changing the type
		 * 
		 * @param value The new value
		 */
		template<class Type>
		void setValue(const Type& value)
		{
			// Push the new value
			convert::pushValue(luaState, value);

			// And create a reference for it
			setReference(LuaReference::create(luaState));
		}

		/**
		 * @brief Gets the value or throws an exception
		 * 
		 * @return Type The value
		 * 
		 * @exception LuaException Thrown when the conversion failed.
		 */
		template<class Type>
		Type getValue() const
		{
			reference->pushValue();

			try
			{
				return convert::popValue<Type>(luaState);
			}
			catch (...)
			{
				lua_pop(luaState, 1);
				throw;
			}
		}

		/**
		 * @brief Specifies if the lua value is valid.
		 * 
		 * @return bool @c true if it can be used and have an underlying reference, @c false otherwise.
		 */
		bool isValid() const { return reference->isValid(); }

		/**
		 * @brief Pushes this lua value onto the stack.
		 */
		virtual bool pushValue() const;

		lua_State* luaState; //!< The lua state of this value.
	protected:
		LuaReferencePtr reference;

		ValueType luaType;
	};

	/**
	* @brief Checks for equality of the lua values.
	* @param lhs The left value
	* @param rhs The right lua value.
	* @return `true` when the value are equal as specified by the lua "==" operator.
	*/
	template<typename Type>
	bool operator==(const LuaValue& lhs, const Type& rhs)
	{
		lhs.pushValue();
		convert::pushValue(lhs.luaState, rhs);

		bool result = lua_equal(lhs.luaState, -2, -1) != 0;

		lua_pop(lhs.luaState, 2);

		return result;
	}

	/**
	* @brief Checks if the other value is bigger than this value.
	* @param lhs The left lua value.
	* @param rhs The right lua value.
	* @return `true` when the second value is bigger than this value as specified
	* 			by lua.
	*/
	template<typename Type>
	bool operator<(const LuaValue& lhs, const Type& rhs)
	{
		lhs.pushValue();
		convert::pushValue(lhs.luaState, rhs);

		bool result = lua_lessthan(lhs.luaState, -2, -1) != 0;

		lua_pop(lhs.luaState, 2);

		return result;
	}

	template<typename Type>
	bool operator!=(const LuaValue& lhs, const Type& rhs)
	{
		return !(lhs == rhs);
	}

	template<typename Type>
	bool operator>(const LuaValue& lhs, const Type& rhs)
	{
		return !(lhs <= rhs);
	}

	template<typename Type>
	bool operator<=(const LuaValue& lhs, const Type& rhs)
	{
		return (lhs == rhs) || (lhs < rhs);
	}
	
	template<typename Type>
	bool operator>=(const LuaValue& lhs, const Type& rhs)
	{
		return !(lhs < rhs);
	}
}

#endif