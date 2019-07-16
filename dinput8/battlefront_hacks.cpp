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


DWORD returnDetourPresent = 0x0;

int __fastcall ContinuePresent(int Empty, int Empty2, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	__asm
	{
		mov edi, edi
		push ebp
		mov ebp, esp
		jmp returnDetourPresent
	}
}

int __stdcall FramerateCap(CONST RECT* pSourceRect, int Empty, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	static LARGE_INTEGER PerformanceCount1;
	static LARGE_INTEGER PerformanceCount2;
	static bool bOnce1 = false;
	static double targetFrameTime = 1000.0 / 20.0;
	static double t = 0.0;
	static DWORD i = 0;

	if (!bOnce1)
	{
		bOnce1 = true;
		QueryPerformanceCounter(&PerformanceCount1);
		PerformanceCount1.QuadPart = PerformanceCount1.QuadPart >> i;
	}

	while (true)
	{
		QueryPerformanceCounter(&PerformanceCount2);
		if (t == 0.0)
		{
			LARGE_INTEGER PerformanceCount3;
			static bool bOnce2 = false;

			if (!bOnce2)
			{
				bOnce2 = true;
				QueryPerformanceFrequency(&PerformanceCount3);
				i = 0;
				t = 1000.0 / (double)PerformanceCount3.QuadPart;
				auto v = t * 2147483648.0;
				if (60000.0 > v)
				{
					while (true)
					{
						++i;
						v *= 2.0;
						t *= 2.0;
						if (60000.0 <= v)
							break;
					}
				}
			}
			SleepEx(0, 1);
			break;
		}

		if (((double)((PerformanceCount2.QuadPart >> i) - PerformanceCount1.QuadPart) * t) >= targetFrameTime)
			break;

		SleepEx(0, 1);
	}
	QueryPerformanceCounter(&PerformanceCount2);
	PerformanceCount1.QuadPart = PerformanceCount2.QuadPart >> i;
	return ContinuePresent(PerformanceCount1.LowPart, PerformanceCount1.HighPart, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

void battlefront_hacks::Hook()
{
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)WidescreenFix, NULL, NULL, NULL);
#if _DEBUG
	if (this->D3D9PresentHook)
	{
		HMODULE d3d9Module = GetModuleHandle("d3d9.dll");
		MODULEINFO modInfo;
		GetModuleInformation(GetCurrentProcess(), d3d9Module, &modInfo, sizeof(MODULEINFO));
		byte sigTarget[] = { 0xC7, 0x6, 0x0, 0x0, 0x0, 0x0, 0x89, 0x86, 0x0, 0x0, 0x0, 0x0, 0x89, 0x86 };
		char sigMask[] = "xx????xx????xx";
		void* ID3D9PresetAddy = SigScan::PerformSig((byte*)d3d9Module, sigTarget, sigMask, modInfo.SizeOfImage);

		if (ID3D9PresetAddy != nullptr)
		{
			//DWORD addy = **(DWORD**)((DWORD)ID3D9PresetAddy + 2);
			//HookInsideFunction(addy, FramerateCap, &returnDetourPresent, 0x5);
		}
		else
		{
			MessageBox(NULL, "Failed to find D3D9::Present, FPS cap won't be applied and it was disabled in dinput8.ini file.", "Error", MB_OK | MB_ICONEXCLAMATION);
		}
	}
#endif
	this->hooked = true;

}