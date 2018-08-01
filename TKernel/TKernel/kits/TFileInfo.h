// TFileInfo

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

// Build 1 in 2018-6-4
#pragma once

#include "TStdInclude.h"

#include "TError.h"

class TFileInfo : public VS_FIXEDFILEINFO
{
	TFileInfo& operator=(const VS_FIXEDFILEINFO& b)
	{
#define assign(x) x = b.x
		assign(dwSignature);
		assign(dwStrucVersion);
		assign(dwFileVersionMS);
		assign(dwFileVersionLS);
		assign(dwProductVersionMS);
		assign(dwProductVersionLS);
		assign(dwFileFlagsMask);
		assign(dwFileFlags);
		assign(dwFileOS);
		assign(dwFileType);
		assign(dwFileSubtype);
		assign(dwFileDateMS);
		assign(dwFileDateLS);
#undef assign
		return *this;
	}
	VOID GetFileInfo(LPCWSTR lpcFileName)
	{
		VS_FIXEDFILEINFO* pffi;
		// in fact dwHandle is ignored
		DWORD dwHandle;
		// return 0 if failed
		DWORD dwSize = GetFileVersionInfoSizeW(lpcFileName, &dwHandle);
		if (!dwSize)
			TError();
		BYTE* pBlock = new BYTE[dwSize];
		GetFileVersionInfoW(lpcFileName, dwHandle, dwSize, pBlock);
		// dwSize is no longer used
		VerQueryValueW(pBlock, L"\\", (LPVOID*)&pffi, (PUINT)&dwSize);
		*this = *pffi;
		delete[] pBlock;
	}
	VOID GetFileInfo() // get own file info
	{
		WCHAR szFileName[MAX_PATH];
		GetModuleFileNameW(HINST, szFileName, MAX_PATH);
		GetFileInfo(szFileName);
	}

public:
	TFileInfo() { GetFileInfo(); }
	explicit TFileInfo(LPCWSTR lpcFileName) { GetFileInfo(lpcFileName); }
	TFileInfo(const TFileInfo&) = default;
	TFileInfo(TFileInfo&&) = delete;

public:
	DWORD GetVer1() const { return (DWORD)HIWORD(dwProductVersionMS); }
	DWORD GetVer2() const { return (DWORD)LOWORD(dwProductVersionMS); }
	DWORD GetVer3() const { return (DWORD)HIWORD(dwProductVersionLS); }
	DWORD GetVer4() const { return (DWORD)LOWORD(dwProductVersionLS); }
};