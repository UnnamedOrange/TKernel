// Copyright (c) Orange Lee. Licensed under the MIT Licence.
// See the LICENCE file in the repository root for full licence text.

#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(Test_TDirect2D)
	{
	public:
		TEST_METHOD(safe_release)
		{
			ID2D1Factory* factory{};
			Assert::IsFalse(TDirect2D::safe_release(factory));
			Assert::IsTrue(SUCCEEDED(D2D1CreateFactory(D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_MULTI_THREADED, &factory)));
			Assert::IsTrue(TDirect2D::safe_release(factory));
			Assert::IsFalse(factory);
		}
	};
}