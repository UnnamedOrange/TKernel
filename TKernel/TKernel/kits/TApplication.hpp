// Copyright (c) 2018-2019 Orange Software
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

#pragma once

#if TKERNEL_WINVER > 0

#include "TStdInclude.hpp"

class TApplication
{
	//public:
	//	TBlock LoadResource(LPCWSTR lpcResourceName, LPCWSTR lpcTypeName)
	//	{
	//		TBlock block;
	//		HRSRC hResInfo = FindResourceW(hInstance, lpcResourceName, lpcTypeName);
	//		if (!hResInfo) return throw std::exception(), block;
	//		HGLOBAL hResource = ::LoadResource(hInstance, hResInfo);
	//		if (!hResource) return throw std::exception(), block;
	//		DWORD dwSize = SizeofResource(hInstance, hResInfo);
	//		if (!dwSize) return throw std::exception(), block;
	//
	//		LPVOID p = LockResource(hResource);
	//		block.realloc(dwSize);
	//		block.assign(0, p, dwSize);
	//		FreeResource(hResource);
	//
	//		return block;
	//	}

	// hInstance
private:
	HINSTANCE __hInstance{};
public:
	const HINSTANCE& hInstance{ __hInstance };

	// Execute
public:
	virtual int App() = 0;
public:
	template<typename AppType>
	static int Execute(HINSTANCE hInstance = GetModuleHandleW(nullptr))
	{
		return std::make_unique<AppType>(hInstance)->App();
	}

	// CommandLine
private:
	std::vector<std::wstring> CommandLine;
private:
	void __QueryCommandLine()
	{
		LPWSTR lpCmdLine = GetCommandLineW();
		int nCmd;
		LPWSTR* lpCmdLineArray = CommandLineToArgvW(lpCmdLine, &nCmd);
		for (int i = 0; i < nCmd; i++)
			CommandLine.push_back(lpCmdLineArray[i]);
		LocalFree((HLOCAL)lpCmdLineArray);
	}
public:
	const std::vector<std::wstring>& GetCmdLine() const { return CommandLine; }

	// App version info
private:
	std::tuple<int, int, int, int> VersionInfo{};
	void __QueryAppVersionInfo()
	{
		VS_FIXEDFILEINFO* pffi;
		// in fact dwHandle is ignored
		DWORD dwHandle = 0;
		// return 0 if failed
		std::vector<WCHAR> strFileName(65536);
		GetModuleFileNameW(hInstance, strFileName.data(), 65536);
		DWORD dwSize = GetFileVersionInfoSizeW(strFileName.data(), &dwHandle);
		if (!dwSize)
			return void(VersionInfo = std::tuple<int, int, int, int>(1, 0, 0, 0));
		auto pBlock = std::make_unique<BYTE[]>(dwSize);
		GetFileVersionInfoW(strFileName.data(), 0, dwSize, pBlock.get());
		// dwSize is no longer used
		VerQueryValueW(pBlock.get(), L"\\", (LPVOID*)&pffi, (PUINT)&dwSize);

		return void(VersionInfo = std::tuple<int, int, int, int>
			(HIWORD(pffi->dwProductVersionMS),
				LOWORD(pffi->dwProductVersionMS),
				HIWORD(pffi->dwProductVersionLS),
				LOWORD(pffi->dwProductVersionLS)));
	}
public:
	const auto& GetAppVersion() const
	{
		return VersionInfo;
	}

	// Single Instance
private:
	HANDLE __hMutex{};
	HANDLE __hMapFile{};
private:
	std::string __GetClassIdentity(unsigned int idx) const
	{
		std::string type{ typeid(*this).name() };
		std::default_random_engine random_engine(static_cast<unsigned int>(std::hash<std::string>()(type)) + idx);
		std::uniform_int_distribution<unsigned int> random_distribution(0u, UINT_MAX);
		char buffer[256]; // class xxx (32-bit random hex)
		sprintf_s(buffer, "%s %x", type.c_str(), random_distribution(random_engine));
		return buffer;
	}
public:
	class SingleInstance
	{
	public:
		SingleInstance()
		{

		}
	};
	// 要求应用程序在用户级单例启动，否则抛出 TApplication::SingleInstance 异常
	void DemandSingleInstance()
	{
		if (__hMutex)
			throw std::runtime_error("DemandSingleInstance cannot be called twice.");

		__hMutex = CreateMutexA(nullptr, FALSE, __GetClassIdentity(0).c_str());
		if (!__hMutex)
			throw std::runtime_error("Fail to CreateMutex.");

		DWORD dwLastError = GetLastError();
		if (dwLastError == ERROR_ALREADY_EXISTS)
		{
			CloseHandle(__hMutex);
			__hMutex = nullptr;
			throw SingleInstance();
		}
	}
private:
	void __CreateSharedMemory()
	{
		if (!__hMapFile)
		{
			__hMapFile = CreateFileMappingA(INVALID_HANDLE_VALUE, // 内存 
				nullptr, PAGE_READWRITE, 0, sizeof(__int64), __GetClassIdentity(1).c_str());
			if (!__hMapFile)
				throw std::runtime_error("Fail to CreateFileMappingA.");
		}
	}
public:
	void WriteSharedInt64(__int64 data)
	{
		__CreateSharedMemory();

		__int64* p = reinterpret_cast<__int64*>(
			MapViewOfFile(__hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(__int64)));
		if (!p)
			throw std::runtime_error("Fail to MapViewOfFile.");
		*p = data;
		UnmapViewOfFile(p);
	}
	__int64 ReadSharedInt64()
	{
		__CreateSharedMemory();

		__int64* p = reinterpret_cast<__int64*>(
			MapViewOfFile(__hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(__int64)));
		if (GetLastError() != ERROR_ALREADY_EXISTS)
			throw std::logic_error("The shared memory have not been written yet.");
		if (!p)
			throw std::runtime_error("Fail to MapViewOfFile.");
		__int64 ret = *p;
		UnmapViewOfFile(p);

		return ret;
	}

	// Constructor
public:
	TApplication(HINSTANCE hInstance) : __hInstance(hInstance)
	{
		__QueryCommandLine();
		__QueryAppVersionInfo();

#if TKERNEL_WINVER >= 1703
		SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
#endif
	}
};

#endif // TKERNEL_WINVER > 0