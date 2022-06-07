#pragma once

#include <cstdio>
#include <cstdarg>

namespace Console
{
	template<typename ...Args>
	void Print(const char* fmt, Args ...args)
	{
		printf(fmt, args...);
	}
};