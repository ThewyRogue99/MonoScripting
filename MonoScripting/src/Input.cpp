#include "Input.h"

namespace Input
{
	bool IsKeyDown(int key)
	{
		return GetKeyState(key) & 0x8000;
	}
}