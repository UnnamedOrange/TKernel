#pragma once
#define TKERNEL_DEV
#include "kits/Tkernel/TKernel.hpp"
#include "resource.h"

class TKernelDemo : public TApplication<TKernelDemo>
{
	class Dialog : public TWindowDialogBox,
		public TWinAug::TDPI,
		public TWinAug::TWinSize
	{
	public:
		Dialog()
		{
			property__lpTemplateName__(MAKEINTRESOURCEW(IDD_DIALOG1));
		}

		virtual INT_PTR WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			if (message == WM_COMMAND && wParam == IDCANCEL)
				EndDialog(hwnd, 0);
			return FALSE;
		}
	} dlg;
	class Window : public TWindowNormal,
		public TWinAug::TDPI,
		public TWinAug::TWinSize,
		public TWinAug::TWinMinmaxinfo
	{
		TKernelDemo& app;

	public:
		Window(TKernelDemo* instance) : app(*instance)
		{

		}
		virtual LRESULT WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			if (message == WM_DESTROY)
			{
				app.DemandSingleInstance();
				PostQuitMessage(0);
			}
			return DefWindowProcW(hwnd, message, wParam, lParam);
		}

		// Í¨¹ý TWinMinmaxinfo ¼Ì³Ð
		virtual POINT _QueryMinTrackSize() override
		{
			return POINT{ static_cast<LONG>(640 * dpi), static_cast<LONG>(480 * dpi) };
		}
	};

public:
	using TApplication::TApplication;

	virtual int OnExecute() override
	{
		dlg.Create(hInstance, nullptr);
		auto wnd = std::make_unique<Window>(this);
		wnd->Create(hInstance, nullptr);
		ShowWindow(wnd->GetHwnd(), SW_SHOW);
		UpdateWindow(wnd->GetHwnd());
		return wnd->MessageLoop();
	}
};