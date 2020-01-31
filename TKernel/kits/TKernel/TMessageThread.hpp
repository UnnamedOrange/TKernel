// Copyright (c) Orange Lee. Licensed under the MIT Licence.
// See the LICENCE file in the repository root for full licence text.

#pragma once

#if TKERNEL_WINVER > 0

#include "TStdInclude.hpp"

class TMessageThread final
{
	std::thread __thread;
	std::mutex __mutex;
	std::condition_variable __cv;
	std::atomic<bool> constructed{};

	void internal_proc()
	{
		MSG msg;
		PeekMessageW(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
		constructed = true;
		__cv.notify_one();
		while (GetMessageW(&msg, nullptr, 0, 0))
		{
			f(msg.message, msg.wParam, msg.lParam);
		}
	}

	using proc = std::function<void(UINT, WPARAM, LPARAM)>;
	proc f;

public:
	TMessageThread(proc f) :
		f(f)
	{
		__thread = std::move(std::thread(&TMessageThread::internal_proc, this));
		std::unique_lock<std::mutex> lock(__mutex);
		__cv.wait(lock, [&]()->bool { return constructed; });
	}
	~TMessageThread()
	{
		post_message(WM_QUIT, 0, 0);
		if (__thread.joinable())
			__thread.join();
		constructed = false;
	}

public:
	void post_message(UINT message, WPARAM wParam, LPARAM lParam)
	{
		PostThreadMessageW(*reinterpret_cast<DWORD*>(&__thread.get_id()), message, wParam, lParam);
	}
};

#endif // TKERNEL_WINVER > 0