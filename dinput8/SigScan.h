#pragma once
#include <Windows.h>
#include <string>
class SigScan
{
public:
	static void* PerformSig(byte* startAddress, byte scanArray[], char scanMask[], int maxLenght);
};

