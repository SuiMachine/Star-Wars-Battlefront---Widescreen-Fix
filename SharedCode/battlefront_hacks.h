#pragma once
#include <Windows.h>


class battlefront_hacks
{
public:
	battlefront_hacks();
	bool hooked;
	bool D3D9PresentHook;
	bool IsBattlefront();
	void Hook();
};
