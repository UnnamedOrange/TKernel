// Copyright (c) Orange Lee. Licensed under the MIT Licence.
// See the LICENCE file in the repository root for full licence text.

#pragma once

#if _CRT_HAS_CXX17

#include "TStdInclude.hpp"

class TMessageThread
{
	std::thread __thread;
	std::condition_variable __cv;
	std::atomic<bool> __destructed{};

	std::mutex __mutex;
	std::queue<std::pair<UINT, std::any>> __queue;

	void internal_proc()
	{
		while (true)
		{
			std::unique_lock<std::mutex> lock(__mutex);
			__cv.wait(lock, [this]()->bool
				{
					return __queue.size() || __destructed;
				});

			if (__queue.empty() && __destructed)
				break;

			auto msg = std::move(__queue.front());
			__queue.pop();
			lock.unlock();
			f(msg.first, std::move(msg.second));
		}
	}

	using proc = std::function<void(UINT, std::any)>;
	proc f;

public:
	TMessageThread(proc f) : f(f)
	{
		__thread = std::move(std::thread(&TMessageThread::internal_proc, this));
	}
	~TMessageThread()
	{
		__destructed = true;
		__cv.notify_one();
		if (__thread.joinable())
			__thread.join();
		__queue = std::move(std::queue<std::pair<UINT, std::any>>());
		__destructed = false;
	}

public:
	void post_message(UINT message, std::any param)
	{
		std::lock_guard<std::mutex> _(__mutex);
		__queue.push(std::make_pair(message, std::move(param)));
		__cv.notify_one();
	}
};

#endif // _CRT_HAS_CXX17