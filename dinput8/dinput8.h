#pragma once

#include <dinput.h>
#include <iostream>
#include <thread>
#define INITGUID
#define DIRECTINPUT_VERSION 0x0800

class f_iDirectInput8 : public IDirectInput8
{
private:
	LPDIRECTINPUT8 f_DirectInput8;

public:
	f_iDirectInput8(LPDIRECTINPUT8 pDirectInput8) { f_DirectInput8 = pDirectInput8; }
	f_iDirectInput8() { f_DirectInput8 = NULL; }

	STDMETHOD(DirectInput8Create)(THIS_ HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, LPUNKNOWN punkOuter);
	STDMETHOD(DllCanUnloadNow)(THIS);
	STDMETHOD(DllGetClassObject)(THIS_ REFCLSID, REFIID, LPVOID);
	STDMETHOD(DllRegisterServer)(THIS);
	STDMETHOD(DllUnregisterServer)(THIS);

};

typedef HRESULT(WINAPI* LPWDirectInput8Create)(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);
typedef HRESULT(WINAPI* LPWDllCanUnloadNow)();
typedef HRESULT(WINAPI* LPWDllGetClassObject)(REFCLSID, REFIID, LPVOID);
typedef HRESULT(WINAPI* LPWDllRegisterServer)();
typedef HRESULT(WINAPI* LPWDllUnregisterServer)();


struct dinput8_dll
{
	HMODULE dll;
	LPWDirectInput8Create DirectInput8Create;
	LPWDllCanUnloadNow DllCanUnloadNow;
	LPWDllGetClassObject DllGetClassObject;
	LPWDllRegisterServer DllRegisterServer;
	LPWDllUnregisterServer DllUnregisterServer;

} dinput8;
