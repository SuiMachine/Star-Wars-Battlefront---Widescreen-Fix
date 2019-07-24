#include "FovHack.h"

DWORD returnAspectratioDetour;
float correction = 1.33333f;
__declspec(naked) void AspectRatioDetour()
{
	__asm
	{
		mov [esp+28],eax
		fild dword ptr [esp+28]
		fdiv[correction]
		jmp returnAspectratioDetour
	}
}

FovHack::FovHack()
{
	HMODULE mod = GetModuleHandle(NULL);
	intptr_t address = (intptr_t)mod + 0xCBE67;
	HookInsideFunction(address, AspectRatioDetour, &returnAspectratioDetour, 8);

}

void FovHack::RefreshResolution()
{
	while (true)
	{
		short ScreenWidth = *(short*)0x00728F70;
		short ScreenHeight = *(short*)0x00728F74;
		if(ScreenWidth != 0 && ScreenHeight != 0)
			correction = ScreenWidth * 1.0f / ScreenHeight / 1.3333333f;
		Sleep(2000);
	}
}
