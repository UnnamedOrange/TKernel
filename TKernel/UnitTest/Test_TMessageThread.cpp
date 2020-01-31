// Copyright (c) Orange Lee. Licensed under the MIT Licence.
// See the LICENCE file in the repository root for full licence text.

#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(Test_TMessageThread)
	{
	public:
		TEST_METHOD(empty_test)
		{
			TMessageThread mt([](UINT message, WPARAM wParam, LPARAM lParam) {});
		}
		TEST_METHOD(construct)
		{
			for (int i = 0; i < 1926; i++)
			{
				std::atomic<bool> test{};
				TMessageThread mt([&](UINT message, WPARAM wParam, LPARAM lParam)
					{
						if (message == WM_APP)
							test = true;
					});
				mt.post_message(WM_APP, 0, 0);
				mt.~TMessageThread();
				Assert::IsTrue(test);
			}
		}
		TEST_METHOD(placement_new)
		{
			std::atomic<bool> test{};
			TMessageThread mt([&](UINT message, WPARAM wParam, LPARAM lParam)
				{
					if (message == WM_APP)
						test = true;
				});
			mt.post_message(WM_APP, 0, 0);
			mt.~TMessageThread();
			Assert::IsTrue(test);
			test = false;

			std::mutex mutex;
			std::unique_lock<std::mutex> lock(mutex);
			std::condition_variable cv;
			new(&mt) TMessageThread([&](UINT message, WPARAM wParam, LPARAM lParam)
				{
					if (message == WM_APP)
					{
						test = true;
						cv.notify_one();
					}
				});
			mt.post_message(WM_APP, 0, 0);
			cv.wait(lock, [&]()->bool { return test; });
			Assert::IsTrue(test);
		}
	};
}