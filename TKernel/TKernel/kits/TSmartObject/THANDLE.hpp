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

#include "../TStdInclude.hpp"

template <HANDLE invalid_value = nullptr>
class THANDLE final
{
	HANDLE handle;

public:
	THANDLE() : handle(invalid_value) {}
	THANDLE(HANDLE free_handle) : handle(free_handle) {}
	THANDLE(const THANDLE&) = delete;
	THANDLE(THANDLE&& another)
	{
		handle = another.handle;
		another.handle = invalid_value;
	}
	~THANDLE()
	{
		reset();
	}

	THANDLE& operator=(HANDLE free_handle)
	{
		reset(free_handle);
		return *this;
	}

	///<summary>
	/// ���ص�ǰ����Ƿ���Ч
	///</summary>
	bool invalid() const { return handle == invalid_value; }
	///<summary>
	/// ���ص�ǰ����Ƿ���Ч
	///</summary>
	bool valid() const { return !invalid(); }
	///<summary>
	/// ���ص�ǰ����Ƿ���Ч
	///</summary>
	operator bool const() { return valid(); }
	///<summary>
	/// ��յ�ǰ���
	///</summary>
	void reset()
	{
		if (valid())
			CloseHandle(handle);
		handle = invalid_value;
	}
	///<summary>
	/// ����ǰ�����Ϊ�µľ��
	///</summary>
	void reset(HANDLE free_handle)
	{
		reset();
		handle = free_handle;
	}
	///<summary>
	/// ����ʹ�ø����ܶ������ǰ���
	///</summary>
	HANDLE release()
	{
		auto ret = handle;
		handle = invalid_value;
		return ret;
	}
	///<summary>
	/// ���ص�ǰ���
	///</summary>
	HANDLE get()
	{
		return handle;
	}


};

#endif // TKERNEL_WINVER > 0