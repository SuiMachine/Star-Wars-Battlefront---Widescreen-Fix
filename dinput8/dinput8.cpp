#include "dinput8.h"

short ScreenWidthLast = 800;
bool bHooked = false;

void WidescreenFix()
{
	while (true)
	{
		short ScreenWidth = *(short*)0x00728F70;
		if (ScreenWidthLast != ScreenWidth)
			Sleep(10000);
		ScreenWidthLast = ScreenWidth;
		*(short*)0x00728F74 = ScreenWidthLast / 4 * 3;
		Sleep(1000);
	}
}

//Dll Main
bool WINAPI DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			//Get module location and its ini file
			char path[MAX_PATH];

			//Get dll from Windows directory
			GetSystemDirectory(path, MAX_PATH);
			strcat_s(path, "\\dinput8.dll");

			//Set pointers
			dinput8.dll = LoadLibraryA(path);
			dinput8.DirectInput8Create = (LPWDirectInput8Create)GetProcAddress(dinput8.dll, "DirectInput8Create");
			dinput8.DllCanUnloadNow = (LPWDllCanUnloadNow)GetProcAddress(dinput8.dll, "DllCanUnloadNow");
			dinput8.DllGetClassObject = (LPWDllGetClassObject)GetProcAddress(dinput8.dll, "DllGetClassObject");
			dinput8.DllRegisterServer = (LPWDllRegisterServer)GetProcAddress(dinput8.dll, "DllRegisterServer");
			dinput8.DllUnregisterServer = (LPWDllUnregisterServer)GetProcAddress(dinput8.dll, "DllUnregisterServer");

			break;
		}
		case DLL_PROCESS_DETACH:
		{
			FreeLibrary(hModule);
			break;
		}
		return true;
	}

	return TRUE;
}

HRESULT WINAPI DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, LPUNKNOWN punkOuter)
{
	//Following makes sure only 1 thread is created and that it is created only for battlefront.exe
	if (!bHooked)
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
				CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)WidescreenFix, NULL, NULL, NULL);
			}
		}

		bHooked = true;
	}

	HRESULT hr = dinput8.DirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);

	return hr;
}

HRESULT WINAPI DllCanUnloadNow()
{
	return dinput8.DllCanUnloadNow();
}

HRESULT WINAPI DllGetClassObject(REFCLSID riidlsid, REFIID riidltf, LPVOID whatever)
{
	return dinput8.DllGetClassObject(riidlsid, riidltf, whatever);
}

HRESULT WINAPI DllRegisterServer()
{
	return dinput8.DllRegisterServer();
}

HRESULT WINAPI DllUnregisterServer()
{
	return dinput8.DllUnregisterServer();
}