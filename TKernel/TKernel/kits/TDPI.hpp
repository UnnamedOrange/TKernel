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

#if TKERNEL_WINVER >= 1607

#include "TStdInclude.hpp"

class TDPI final
{
	HWND __hwnd{};
	WNDPROC __origin_proc{};

	UINT __dpi{};

public:
	TDPI()
	{

	}

private:
	static auto& AccessMap()
	{
		static std::unordered_map<HWND, TDPI*> _;
		return _;
	}
	static LRESULT CALLBACK __new_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		TDPI& t = *(AccessMap()[hwnd]);
		if (message == WM_DPICHANGED)
		{
			t.__dpi = HIWORD(wParam);

			RECT* const prcNewWindow = (RECT*)lParam;
			SetWindowPos(hwnd,
				NULL,
				prcNewWindow->left,
				prcNewWindow->top,
				prcNewWindow->right - prcNewWindow->left,
				prcNewWindow->bottom - prcNewWindow->top,
				SWP_NOZORDER | SWP_NOACTIVATE);
		}
		return t.__origin_proc(hwnd, message, wParam, lParam);
	}

public:
	void AttachToWindow(HWND hwnd)
	{
		if (__origin_proc)
			throw std::runtime_error("This TDPI object has already attached to a window.");
		__origin_proc = reinterpret_cast<WNDPROC>(GetWindowLongPtrW(hwnd, GWLP_WNDPROC));
		if (!__origin_proc)
			throw std::runtime_error("Fail to attach to the window.");
		AccessMap()[hwnd] = this;
		SetWindowLongPtrW(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(__new_proc));
		__hwnd = hwnd;

		__dpi = GetDpiForWindow(hwnd);
	}
	void Detach()
	{
		if (__origin_proc)
		{
			SetWindowLongPtrW(__hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(__origin_proc));
			AccessMap().erase(__hwnd);
			__origin_proc = nullptr;
			__hwnd = nullptr;
		}
	}

public:
	~TDPI()
	{
		Detach();
	}

public:
	operator double()
	{
		return static_cast<double>(__dpi) / USER_DEFAULT_SCREEN_DPI;
	}
};

#endif // TKERNEL_WINVER >= 1607