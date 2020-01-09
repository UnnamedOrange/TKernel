// Copyright (c) Orange Lee. Licensed under the MIT Licence.
// See the LICENCE file in the repository root for full licence text.

#pragma once

#pragma warning(push)
#pragma warning(disable:4458) // Gdiplus 声明隐藏了类成员

// C Library
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cfenv>
#include <cfloat>
#include <cinttypes>
#include <ciso646>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cuchar>
#include <cwchar>
#include <cwctype>

// Containers
#include <array>
#include <bitset>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Input/Output Stream Library
#include <iostream>
#include <fstream>
#include <sstream>

// Atomics and threading library
#include <atomic>
#include <condition_variable>
#include <future>
#include <mutex>
#include <thread>

// Miscellaneous headers
#include <algorithm>
#include <chrono>
#include <codecvt>
#include <complex>
#include <exception>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <locale>
#include <memory>
#include <new>
#include <numeric>
#include <random>
#include <ratio>
#include <regex>
#include <stdexcept>
#include <string>
#include <system_error>
#include <tuple>
#include <typeindex>
#include <typeinfo>
#include <type_traits>
#include <utility>
#include <valarray>

// CXX17
#if _HAS_CXX17
#include <optional>
#include <any>
#include <variant>
#include <string_view>
#include <filesystem>
#include <execution>
#endif // _HAS_CXX17

// CXX20
#if _HAS_CXX20

#endif // _HAS_CXX20

#if TKERNEL_WINVER > 0

// Windows Base
#include <Windows.h>
#include <windowsx.h>
#include <tchar.h>

// Extra
#include <tlhelp32.h>
#include <commctrl.h>
#include <commdlg.h>
#include <shellapi.h>
#include <shlobj.h>
#include <shldisp.h>
#include <Psapi.h>
#include <dwmapi.h>

// Graphics
#if TKERNEL_GDIPVER > 0

#if TKERNEL_GDIPVER == 11
#define GDIPVER 0x0110
#endif // TKERNEL_GDIPVER == 11

#include <gdiplus.h>

#endif // TKERNEL_WINVER > 0

// Common Control 6.0
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

// link lib
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "version.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "shlwapi")
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "gdiplus.lib")

#endif // TKERNEL_WINVER > 0

#pragma warning(pop)