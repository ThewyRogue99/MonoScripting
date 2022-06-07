#pragma once

#include <cstdio>
#include <cstdarg>

class Console
{
public:
	template<typename ...Args>
	void Print(const char* fmt, Args ...args)
	{
		printf(fmt, args...);
	}
};