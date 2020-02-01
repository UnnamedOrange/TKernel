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
			TMessageThread mt([](UINT, std::any) {});
		}
		TEST_METHOD(construct)
		{
			for (int i = 0; i < 1926; i++)
			{
				std::atomic<bool> test{};
				TMessageThread mt([&](UINT message, std::any param)
					{
						if (message == WM_APP)
							test = true;
					});
				mt.post_message(WM_APP, std::any());
				mt.~TMessageThread();
				Assert::IsTrue(test);
			}
		}
		TEST_METHOD(placement_new)
		{
			std::atomic<bool> test{};
			TMessageThread mt([&](UINT message, std::any param)
				{
					if (message == WM_APP)
						test = true;
				});
			mt.post_message(WM_APP, std::any());
			mt.~TMessageThread();
			Assert::IsTrue(test);
			test = false;

			std::mutex mutex;
			std::unique_lock<std::mutex> lock(mutex);
			std::condition_variable cv;
			new(&mt) TMessageThread([&](UINT message, std::any param)
				{
					if (message == WM_APP)
					{
						test = true;
						cv.notify_one();
					}
				});
			mt.post_message(WM_APP, std::any());
			cv.wait(lock, [&]()->bool { return test; });
			Assert::IsTrue(test);
		}
		TEST_METHOD(application_test)
		{
			struct msgbox
			{
				std::wstring text;
				std::wstring caption;
			};
			TMessageThread mt([&](UINT message, std::any param)
				{
					switch (message)
					{
					case 0:
					{
						const auto& [text, caption] = std::any_cast<msgbox>(param);
						// MessageBoxW(nullptr, text.c_str(), caption.c_str(), NULL);
						break;
					}
					case 1:
					{
						MessageBeep(std::any_cast<long>(param));
						break;
					}
					default:
						break;
					}
				});
			mt.post_message(0, msgbox{ L"233", L"666" });
			mt.post_message(1, MB_ICONINFORMATION);
		}
	};
}