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

#if TKERNEL_WINVER > 0

#include "../../TStdInclude.hpp"

///<summary>
/// TWinAugBase
/// 继承 TWindow 的同时继承 TWinAugBase，即可实现窗口增强。
/// 编写窗口增强时，需要使用虚继承。
///</summary>

class TWinAugBase
{
	friend class TWindow;

	using ProcType = std::function<void(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)>;

protected:
	std::vector<ProcType> _front_procs;
	std::vector<ProcType> _back_procs;
	template<typename T>
	void append_front(T* _this)
	{
		_front_procs.push_back(std::bind(&T::AugProc, _this,
			std::placeholders::_1, std::placeholders::_2,
			std::placeholders::_3, std::placeholders::_4));
	}
};

#endif // TKERNEL_WINVER > 0