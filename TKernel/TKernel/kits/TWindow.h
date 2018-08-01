// TWindow

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

#pragma once

#include "kits/TStdInclude.h"

#include "kits/TError.h"

class TWindowBase abstract
{
private:
	HWND hWnd = NULL;
public:
	HWND GetHwnd() { return hWnd; }

private:
	HACCEL hAccel = NULL;
public:
	VOID SetCurrentAccelerator(HACCEL handle) { hAccel = handle; }
	VOID RevokeCurrentAccelerator() { hAccel = NULL; }

public:
	INT MessageLoop()
	{
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0))
		{
			if (!hAccel || !TranslateAccelerator(hWnd, hAccel, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		return (INT)msg.wParam;
	}

private:
	std::pair<PVOID, std::wstring> GetIdentity()
	{
		auto vptr = *((PVOID*)this); // 取 this 指向的内容的前 8 个字节，即虚函数表的指针
		WCHAR clsName[256];
		swprintf_s(clsName, L"0x%p", vptr);
		return std::make_pair(vptr, std::wstring(clsName));
	}

private:
	virtual BOOL __RegisterClasses(const std::wstring& strClassName) = 0;
	virtual INT_PTR __Create(INT_PTR param) = 0;
	std::wstring strClassName;
public:
	const std::wstring& GetClsName() { return strClassName; }

private:
	INT_PTR InitInstance(INT_PTR param)
	{
		if (hWnd) return 0;
		static std::set<PVOID> set;
		auto id = GetIdentity();
		if (!set.count(id.first))
			if (__RegisterClasses(id.second))
				set.insert(id.first);
		strClassName = id.second;
		return __Create(param);
	}
public:
	virtual INT_PTR Create(INT_PTR param = 0)
	{
		return InitInstance(param);
	}

	friend class TWindow;
	friend class TDialog;
};

class TWindow abstract : public TWindowBase
{
private:
	static LRESULT CALLBACK VirtualProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		TWindow* p = (TWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

		if (message == WM_CREATE)
		{
			p = (TWindow*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)p);
		}

		LRESULT ret;
		if (p) ret = p->WndProc(hwnd, message, wParam, lParam);
		else ret = DefWindowProc(hwnd, message, wParam, lParam);

		if (p && message == WM_DESTROY)
			p->hWnd = NULL;

		return ret;
	}
	virtual LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;

private:
	// 通过 TWindowBase 继承
	virtual BOOL __RegisterClasses(const std::wstring& strClassName) override
	{
		WNDCLASSEXW wndclassex = { sizeof(WNDCLASSEXW) };
		wndclassex.style = CS_HREDRAW | CS_VREDRAW;
		wndclassex.lpfnWndProc = VirtualProc;
		wndclassex.cbClsExtra = 0;
		wndclassex.cbWndExtra = 0;
		wndclassex.hInstance = HINST;
		wndclassex.hIcon = NULL;
		wndclassex.hIconSm = NULL;
		wndclassex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclassex.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
		wndclassex.lpszMenuName = NULL;
		wndclassex.lpszClassName = strClassName.c_str();
		return RegisterClassExW(&wndclassex);
	}
	virtual INT_PTR __Create(INT_PTR) override
	{
		hWnd = CreateWindowEx(NULL, strClassName.c_str(), L"",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			NULL, NULL, HINST, (LPVOID)this);
		return (INT_PTR)0;
	}

public:
	HWND Create()
	{
		InitInstance(0);
		return hWnd;
	}
	HWND CreatePopup()
	{
		InitInstance(0);
		SetWindowLong(hWnd, GWL_STYLE, WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX);
		return hWnd;
	}

public:
	VOID ShowAndUpdate()
	{
		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);
	}
};

class TDialog abstract : public TWindowBase
{
private:
	static INT_PTR CALLBACK VirtualProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		TDialog* p = (TDialog*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

		if (message == WM_INITDIALOG)
		{
			p = (TDialog*)lParam;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)p);
		}

		LRESULT ret;
		if (p) ret = p->WndProc(hwnd, message, wParam, lParam);
		else ret = 0;

		if (p && message == WM_DESTROY)
			p->hWnd = NULL;

		return ret;
	}
	virtual INT_PTR CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;

public:
	struct TDialogParam
	{
		LPCWSTR lpTemplateName = NULL;
		HWND hwndParent = NULL;
		TDialogParam() = default;
		TDialogParam(LPCWSTR lpTemplateName, HWND hwndParent) :
			lpTemplateName(lpTemplateName), hwndParent(hwndParent) {}
	};
private:
	// 通过 TWindowBase 继承
	virtual BOOL __RegisterClasses(const std::wstring& strClassName) override
	{
		return FALSE;
	}
	virtual INT_PTR __Create(INT_PTR param) override
	{
		auto ptr = (TDialogParam*)param;
		if (!ptr)
			return 0;
		return DialogBoxParam(HINST, ptr->lpTemplateName, ptr->hwndParent, VirtualProc, (LPARAM)this);
	}
public:
	INT_PTR Create(TDialogParam param)
	{
		return InitInstance((INT_PTR)&param);
	}
};