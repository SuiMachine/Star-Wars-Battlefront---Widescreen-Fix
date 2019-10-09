#include <windows.h>
#include "MenuResOverride.h"
#include "HookFunctions.h"
#include "FovHack.h"
#include "../externals/inireader/IniReader.h"

MenuResOverride* MenuHack;
FovHack* fovHack;

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		CIniReader iniReader("");
		HMODULE mod = GetModuleHandle(NULL);
		UnprotectModule(mod);

		if (MenuHack == NULL && iniReader.ReadInteger("MenuOverride", "Width", 0) != 0 && iniReader.ReadInteger("MenuOverride", "Height", 0) != 0)
		{
			MenuHack = new MenuResOverride(iniReader.ReadInteger("MenuOverride", "Width", 0), iniReader.ReadInteger("MenuOverride", "Height", 0));
		}

		if (fovHack == NULL && iniReader.ReadBoolean("Game", "CorrectAspectRatio", true))
		{
			fovHack = new FovHack();
		}


		//0042B6BD - GUI scale
	}
	return TRUE;
}