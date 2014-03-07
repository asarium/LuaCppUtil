#ifndef LUA_EXCEPTION_H
#define LUA_EXCEPTION_H
#pragma once

#include <string>

namespace luacpp
{
	/**
	 * @brief An exception within lua
	 */
	class LuaException : public std::exception
	{
	public:

		LuaException(const std::string& message = "Lua Error!") throw() : msg(message)
		{
		}

		virtual ~LuaException() throw()
		{
		}

		const char* what() const throw() override
		{
			return msg.c_str();
		}

	private:
		std::string msg;
	};
}

#endif