#include "battlefront_hacks.h"
short ScreenWidthLast = 800;

void WidescreenFix()
{
	while (true)
	{
		short ScreenWidth = *(short*)0x00728F70;
		if (ScreenWidth != 0)
		{
			if (ScreenWidthLast != ScreenWidth)
				Sleep(10000);
			ScreenWidthLast = ScreenWidth;
			*(short*)0x00728F74 = ScreenWidthLast / 4 * 3;
		}
		Sleep(1000);
	}
}

battlefront_hacks::battlefront_hacks()
{
	this->hooked = false;
	this->D3D9PresentHook = true;
}

bool battlefront_hacks::IsBattlefront()
{
	HMODULE baseModule = GetModuleHandle(NULL);
	char buffer[MAX_PATH];
	if (GetModuleFileName(baseModule, buffer, MAX_PATH) != 0)
	{
		char fileName[MAX_PATH];
		GetFileTitle(buffer, fileName, MAX_PATH);
		for (int i = 0; i < strlen(fileName); i++)
		{
			fileName[i] = tolower(fileName[i]);
		}

		if (strstr(fileName, "battlefront.exe"))
		{
			return true;
		}
	}
	return false;
}

void battlefront_hacks::Hook()
{
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)WidescreenFix, NULL, NULL, NULL);
	this->hooked = true;

}