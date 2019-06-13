// Copyright (C) 2018 Orange Software
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// TDPI

// DPI Helper

#pragma once
#include "TStdInclude.h"

#include "TError.h"

// 模板参数决定是否返回之前获取的值，默认为是
template <BOOL bCached = TRUE>
class TDPI final
{
	static const UINT iRegularDPI = 96;
	UINT iCntDPI = NULL;

	BOOL bInitialized = FALSE;
	HMODULE hModule = NULL;
	std::function<UINT()> __GetDpiForSystem;

	VOID Initialize()
	{
		if (!bInitialized)
		{
			bInitialized = TRUE;
			if (hModule = LoadLibraryW(L"user32.dll"))
				__GetDpiForSystem = GetProcAddress(hModule, "GetDpiForSystem");
		}
	}

	UINT GetCurrentDPI()
	{
		if (!bInitialized)
			Initialize();

		if (!bCached || !iCntDPI)
		{
			if (__GetDpiForSystem)
			{
				return iCntDPI = __GetDpiForSystem();
			}
			else
			{
				HDC hdc = GetDC(NULL);
				UINT ret = GetDeviceCaps(hdc, LOGPIXELSX);
				ReleaseDC(NULL, hdc);
				return iCntDPI = ret;
			}
		}
		else
		{
			return iCntDPI;
		}
	}

public:
	TDPI() { Initialize(); }
	~TDPI()
	{
		if (hModule)
		{
			FreeLibrary(hModule);
			hModule = NULL;
		}
	}

public:
	template <typename T>
	T operator() (T in)
	{
		return (T)(in * ((double)GetCurrentDPI() / iRegularDPI));
	}
};