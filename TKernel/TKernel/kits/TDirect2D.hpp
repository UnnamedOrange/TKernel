// Copyright (c) Orange Lee. Licensed under the MIT Licence.
// See the LICENCE file in the repository root for full licence text.

#pragma once

#if TKERNEL_WINVER >= 7

#include "TStdInclude.hpp"

#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1_2.h>
#include <d2d1_3.h>
#include <dwrite.h>
#include <dwrite_1.h>
#include <dwrite_2.h>
#include <dwrite_3.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

class TDirect2D final
{
public:
	template<typename T>
	static bool safe_release(T& p)
	{
		if (p)
		{
			p->Release();
			p = nullptr;
			return true;
		}
		return false;
	}
};

#endif // TKERNEL_WINVER >= 7