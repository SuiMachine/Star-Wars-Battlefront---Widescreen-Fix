#include "FovHack.h"

DWORD returnAspectratioDetour;
float correction = 1.33333f;
__declspec(naked) void AspectRatioDetour()
{
	__asm
	{
		mov [esp+28],eax
		fild dword ptr [esp+28]
		fdiv [correction]
		jmp returnAspectratioDetour
	}
}

void RefreshResolutionThread()
{
	while (true)
	{
		short ScreenWidth = *(short*)0x00728F70;
		short ScreenHeight = *(short*)0x00728F74;
		if (ScreenWidth != 0 && ScreenHeight != 0)
			correction = ScreenWidth * 1.0f / ScreenHeight / 1.3333333f;
		Sleep(2000);
	}
}

FovHack::FovHack()
{
	HMODULE mod = GetModuleHandle(NULL);
	HookInsideFunction((intptr_t)mod + 0xCBE67, AspectRatioDetour, &returnAspectratioDetour, 8);
	
	//Silent told me to fix it... 1 handle... oh well.
	HANDLE initThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)RefreshResolutionThread, NULL, NULL, NULL);
	SetThreadPriority(initThread, THREAD_PRIORITY_BELOW_NORMAL);
	CloseHandle(initThread);
}

