#include "SigScan.h"

void* SigScan::PerformSig(byte* startAddress, byte * scanArray, char * scanMask, int maxLenght)
{
	int offset = 0;
	int lenghtOfMask = std::strlen(scanMask);
	while(offset < maxLenght)
	{
		bool isCorrect = true;
		for (int i = 0; i < lenghtOfMask; i++)
		{
			if (i + offset > maxLenght)
			{
				isCorrect = false;
				break;
			}

			char currentMask = scanMask[i];
			if (currentMask == '?')
				continue;
			else
			{
				//45F23
				byte* currentByte = startAddress + offset + i;
				byte compareByte = scanArray[i];
				if (*currentByte == compareByte)
					continue;
				else
				{
					isCorrect = false;
					break;
				}
			}
		}
		if (isCorrect)
			return (startAddress + offset);

		offset++;
	}

	return nullptr;
}
