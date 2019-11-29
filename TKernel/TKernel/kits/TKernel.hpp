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

#pragma warning(push)
#ifdef TKERNEL_DEV
#pragma warning(disable:4100) // 未引用的形参
#endif

#ifndef TKERNEL_WINVER
///<summary>
/// 对于 Windows 10，请按年份-月份指定版本；对于 Windows 10 以前版本，请使用 7、8；对于不依赖 Windows 的程序，请使用 0
///</summary>
#define TKERNEL_WINVER 1903
#endif // TKERNEL_WINVER

#if TKERNEL_WINVER > 0
#ifndef TKERNEL_GDIPVER
///<summary>
/// 对于 GDIP 1.0，请使用 10；对于 GDIP 1.1，请使用 11；对于不使用 GDIP 的程序，请使用 0
///</summary>
#define TKERNEL_GDIPVER 11
#endif // TKERNEL_GDIPVER
#endif // TKERNEL_WINVER

///<summary>
/// TStdInclude <version>Alpha 10.0</version>
/// 包含所有的头文件
///<summary>
#include "TStdInclude.hpp"

///<summary>
/// TSmartObject <version>Alpha 10.0</version>
/// 操作系统智能对象
///</summary>
#include "TSmartObject/TSmartObject.hpp"

///<summary>
/// TApplication <version>Alpha 10.0</version>
/// 应用程序的入口
///</summary>
#include "TApplication.hpp"

///<summary>
/// TGdiplus <version>Alpha 10.0</version>
/// GDIP 加载器
///</summary>
#include "TGdiplus.hpp"

///<summary>
/// TWindow <version>Alpha 10.0</version>
/// 面向对象窗口
///</summary>
#include "TWindow/TWindow.hpp"

#pragma warning(pop)