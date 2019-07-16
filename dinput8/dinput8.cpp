#include "dinput8.h"

battlefront_hacks* hacks;

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
#pragma warning(disable:6387)
			dinput8.DirectInput8Create = (LPWDirectInput8Create)GetProcAddress(dinput8.dll, "DirectInput8Create");
			dinput8.DllCanUnloadNow = (LPWDllCanUnloadNow)GetProcAddress(dinput8.dll, "DllCanUnloadNow");
			dinput8.DllGetClassObject = (LPWDllGetClassObject)GetProcAddress(dinput8.dll, "DllGetClassObject");
			dinput8.DllRegisterServer = (LPWDllRegisterServer)GetProcAddress(dinput8.dll, "DllRegisterServer");
			dinput8.DllUnregisterServer = (LPWDllUnregisterServer)GetProcAddress(dinput8.dll, "DllUnregisterServer");
#pragma warning(enable:6387) 

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
	if (hacks == NULL)
		hacks = new battlefront_hacks();

	//Following makes sure only 1 thread is created and that it is created only for battlefront.exe
	if (!hacks->hooked && hacks->IsBattlefront())
	{
		hacks->Hook();
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