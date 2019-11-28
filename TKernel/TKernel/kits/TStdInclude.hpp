﻿// Copyright (c) 2018-2019 Orange Software
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

#pragma warning(push)
#pragma warning(disable:4458)

///<summary>
/// CRT
///</summay>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <cassert>
#include <climits>
#include <cctype>
#include <ctime>

///<summary>
/// STL
///</summay>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <stack>
#include <queue>
#include <deque>
#include <map>
#include <set>
#include <bitset>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <random>
#include <stdexcept>
#include <typeinfo>
#include <thread>
#if _HAS_CXX17
#include <optional>
#endif // _HAS_CXX17

#if TKERNEL_WINVER > 0

///<summary>
/// Windows Base
///</summay>
#include <Windows.h>
#include <windowsx.h>
#include <tchar.h>

///<summary>
/// Extra
///</summay>
#include <tlhelp32.h>
#include <commctrl.h>
#include <commdlg.h>
#include <shellapi.h> 
#include <shlobj.h>
#include <shldisp.h>
#include <Psapi.h>
#include <dwmapi.h>

///<summary>
/// Graphics
///</summay>
#if TKERNEL_GDIPVER > 0

#if TKERNEL_GDIPVER == 11
#define GDIPVER 0x0110
#endif // TKERNEL_GDIPVER == 11

#include <gdiplus.h>

#endif // TKERNEL_WINVER > 0

#include <d2d1.h>

///<summary>
/// Common Control 6.0
///</summay>
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

///<summary>
/// link lib
///</summay>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "version.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "shlwapi")
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "gdiplus.lib")

///<summary>
/// HINST macro is abandoned.
///</summay>
//extern "C" const IMAGE_DOS_HEADER __ImageBase; // &__ImageBase
//#define HINST HINSTANCE(&__ImageBase)

#endif // TKERNEL_WINVER > 0

#pragma warning(pop)