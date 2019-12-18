#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(Test_THANDLE)
	{
	public:
		TEST_METHOD(THANDLE_Constructor)
		{
			THANDLE handle;
			Assert::IsTrue(handle.get() == nullptr);
			Assert::IsTrue(handle.invalid());
			Assert::IsTrue(!handle.valid());
			Assert::IsFalse(handle);
		}
		TEST_METHOD(THANDLE_AutoClose)
		{
			THANDLE handle(CreateEventW(nullptr, TRUE, FALSE, nullptr));
			Assert::IsTrue(handle);
			Assert::IsTrue(handle.valid());
			Assert::IsFalse(handle.invalid());
			HANDLE free_handle{ handle.get() };
			handle.~_THANDLE();
			Assert::IsFalse(handle);
			DWORD dwTemp;
			Assert::IsFalse(GetHandleInformation(free_handle, &dwTemp));
		}
		TEST_METHOD(THANDLEX_Constructor)
		{
			THANDLEX handle;
			Assert::IsTrue(handle.get() == INVALID_HANDLE_VALUE);
		}
		TEST_METHOD(THANDLE_MoveAssignment)
		{
			THANDLE handle(CreateEventW(nullptr, TRUE, FALSE, nullptr));
			Assert::IsTrue(handle);
			THANDLE another{ std::move(handle) };
			Assert::IsTrue(another);
			Assert::IsFalse(handle);
		}
		TEST_METHOD(THANDLE_Release)
		{
			THANDLE handle(CreateEventW(nullptr, TRUE, FALSE, nullptr));
			Assert::IsTrue(handle);
			auto t = handle.release();
			Assert::IsFalse(handle);
			if (t) Assert::IsFalse(handle); CloseHandle(t);
		}
	};
}