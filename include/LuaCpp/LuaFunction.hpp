#ifndef LuaFunction_H
#define LuaFunction_H
#pragma once

#include <vector>
#include <string>
#include <functional>

#include "LuaCpp/LuaConvert.hpp"
#include "LuaCpp/LuaValue.hpp"
#include "LuaCpp/LuaTable.hpp"

namespace luacpp
{
	typedef std::vector<LuaValue> LuaValueList;
	class LuaFunction;

	/**
	 * @brief A reference to lua code.
	 *
	 * Wraps a Lua-function value which can be called as a function from C++ code into Lua code.
	 * Provides ways to set arguments and get return values.
	 *
	 * @see luacpp::convert
	 */
	class LuaFunction : public LuaValue
	{
	public:
		/**
		 * @brief Compile lua code and return the function.
		 * The specified code is compiled into a lua functions which can then be called with the returned class.
		 * 
		 * @param L The lua state
		 * @param code The actual function code
		 * @param name The name of the function, for debugging
		 * @return luacpp::LuaFunction The function object which can be used for calling that lua function
		 * 
		 * @exception LuaException Thrown when the compilation fails with the error as the execption message.
		 */
		static LuaFunction createFromCode(lua_State* L, const std::string& code, const std::string& name = "");

		/**
		 * @brief Creates a function object from a lua_CFunction
		 * 
		 * @param L The lua state
		 * @param function The function pointer
		 * @return luacpp::LuaFunction
		 */
		static LuaFunction createFromCFunction(lua_State* L, lua_CFunction function);

		/**
		 * @brief Default constructor
		 */
		LuaFunction();
		
		/**
		 * @brief Copy-constructor
		 *
		 * Copies the reference of the other function and sets it as the reference of this object.
		 *
		 * @param other The other function.
		 */
		LuaFunction(const LuaFunction& other);

		/**
		 * @brief Frees the reference to the function if it exists.
		 */
		virtual ~LuaFunction();

		/**
		 * @brief Sets the function environment.
		 *
		 * @param environment The table which will be the new environment of the function.
		 * @return `true` when successful, `false` otherwise
		 */
		bool setEnvironment(const LuaTable& environment);

		/**
		 * @brief Sets the function called when an error occurs
		 * This lua_Cfunction will be called by lua if an error occus while executing this function
		 * 
		 * @return void
		 */
		void setErrorFunction(const LuaFunction& errorFunc)
		{
			errorFunction = errorFunc.getReference();
		}

		/**
		 * @brief Sets a new reference.
		 * This overload checks if the passed reference is a function
		 * 
		 * @param ref The new reference
		 * @return void
		 */
		void setReference(LuaReferencePtr ref) override;

		/**
		 * @brief Calls the function.
		 * This function first pushes the error function onto the stack if one was set, then
		 * the arguments and then the function. After that the function is called with lua_pcall
		 * and the return values are converted into a LuaValueList.
		 * 
		 * @param arguments The arguments passed to the functions. Defaults to none
		 * @return luacpp::LuaValueList The values returned by the function call
		 * 
		 * @exception LuaException If an error occurs while executing the function an exception is thrown
		 * 	with the message of the error.
		 */
		LuaValueList call(const LuaValueList& arguments = LuaValueList());

		/**
		 * @brief Calls the function. See call().
		 * @return Same as call().
		 */
		LuaValueList operator()(const LuaValueList& arguments = LuaValueList());
	private:
		bool isCFunction; //!< @c true to indicate that this is a C-function, mainly used for checking the values

		LuaReferencePtr errorFunction;
	};
}

#endif