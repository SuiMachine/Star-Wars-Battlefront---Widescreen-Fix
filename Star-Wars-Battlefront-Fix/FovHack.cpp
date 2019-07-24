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
