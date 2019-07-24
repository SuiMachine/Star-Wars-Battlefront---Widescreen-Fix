#include "MenuResOverride.h"

MenuResOverride::MenuResOverride(int width, int height)
{
	*(int*)0x0042AB4E = width;
	*(int*)0x0042AB58 = height;
	*(int*)0x005F6879 = width;
	*(int*)0x005F6881 = height;
}
