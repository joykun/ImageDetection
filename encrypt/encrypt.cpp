#pragma once
#include "encrypt.h"

//$(SolutionDir)$(Configuration)
#define EXCEPTION_EXECUTE_HANDLER 1

void GetCpuID(string& strCpuID)
{// CPU ID
	unsigned int bException = false;
	unsigned char szCpu[32] = { 0 };
	unsigned int  uCpuID = 0U;
	char szBuf[256] = { 0 };
	unsigned int s1 = 0U;
	unsigned int s2 = 0U;

	__try
	{
		_asm
		{
			mov eax, 01h
				xor edx, edx
				cpuid
				mov s1, edx
				mov s2, eax
		}
		sprintf(szBuf, "%08X%08X", s1, s2);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		bException = true;
	}
	if (!bException)
	{

		strCpuID = szBuf;
	}
}