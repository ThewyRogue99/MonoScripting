#pragma once

#include <cstdio>
#include <cstdarg>

#include <mono/jit/jit.h>

#include <iostream>

#include <fcntl.h>
#include <io.h>

namespace Console
{
	template<typename ...Args>
	void Print(const char* fmt, Args ...args)
	{
		int val = _setmode(_fileno(stdout), _O_U8TEXT);
		printf(fmt, args...);
	}

	template<typename ...Args>
	void WPrint(const wchar_t* fmt, Args ...args)
	{
		int val = _setmode(_fileno(stdout), _O_U16TEXT);
		wprintf(fmt, args...);
	}

	void CSWrite(MonoString* str)
	{
		int val = _setmode(_fileno(stdout), _O_U16TEXT);

		wprintf(L"%s", mono_string_to_utf16(str));
	}

	void CSWriteLine(MonoString* str)
	{
		int val = _setmode(_fileno(stdout), _O_U16TEXT);

		wprintf(L"%s\n", mono_string_to_utf16(str));
	}
};