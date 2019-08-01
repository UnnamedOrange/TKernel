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

// TOpenSaveDialog

#pragma once

#include "TStdInclude.hpp"

class TOpenSaveDialog : protected OPENFILENAMEW
{
	std::vector<WCHAR> buffer;

public:
	TOpenSaveDialog()
	{
		lStructSize = sizeof(OPENFILENAMEW);	// �ṹ��С
		hwndOwner = NULL;						// �Ի���ӵ���ߣ���Ϊ��
		hInstance = NULL;						// ��ʼ��Ϊ NULL��ʹ��ϵͳ�Ի��򱣳�Ϊ NULL
		lpstrFilter = L"�����ļ�\0*.*\0\0";		// ɸѡ������Ϊ�գ�Ϊ��ʱ��ݷ�ʽ��ָ�򡣽�βΪ���� \0�������� \0 �ָ���ע�ⲻ���� std::wstring
		lpstrCustomFilter = nullptr;			// ��ʼ��Ϊ nullptr�����ֿգ��û��Զ��������
		nMaxCustFilter = NULL;					// ��ʼ��Ϊ NULL���� lpstrCustomFilter Ϊ NULL ������ NULL
		nFilterIndex = 1;						// �������������� 1 ��ʼ��0 ��ʾ�û��Զ��������
		lpstrFile = nullptr;					// ��ȡ���ļ����Ļ��壬����ļ��� \0(OFN_EXPLORER) ���߿ո� (!OFN_EXPLORER) �ָ��������̫С��ǰ�����ֽڱ�������Ҫ�Ļ�������С
		nMaxFile = NULL;						// ���������ȣ����� 256
		lpstrFileTitle = nullptr;				// �������ļ�������չ��������·�����Ļ��壬��Ϊ nullptr
		nMaxFileTitle = NULL;					// ����������
		lpstrInitialDir = nullptr;				// ��ʼĿ¼
		lpstrTitle = nullptr;					// �Ի�����⣬��Ϊ�գ�Ĭ��Ϊ���棬��
		Flags = NULL;							// ���
		// OFN_ALLOWMULTISELECT	���ļ�
		// OFN_CREATEPROMPT		�����Ƿ��½�
		// OFN_OVERWRITEPROMPT	�����Ƿ񸲸�
		// OFN_ENABLESIZING		���������С������OFN_EXPLORER
		// OFN_EXPLORER			�򿪺����ΪĬ�Ϸ��
		// OFN_FILEMUSTEXIST	�ļ��������
		nFileOffset = NULL;						// �ļ������·�����Ŀ�ʼ�±꣬���ڶ��ļ��Ի�����ֵΪ��һ��·���Ķ�Ӧֵ
		nFileExtension = NULL;					// ��չ�����·�����Ŀ�ʼ�±꣬����.��ͬ��
		lpstrDefExt = NULL;						// Ĭ����չ��������.����ǰ�����ַ���Ч
		lCustData = NULL;						// ��ʼ��Ϊ NULL�����ڹ��ӳ��򣬲���
		lpfnHook = nullptr;						// ��ʼ��Ϊ nullptr�����ڹ��ӳ��򣬲���
		lpTemplateName = nullptr;				// ��ʼ��Ϊ nullptr�������Զ���Ի��򣬲���
		pvReserved = NULL;						// ����
		dwReserved = NULL;						// ����
		FlagsEx = NULL;							// ��չ���
	}

public:
	HWND hwndParent = nullptr;
	LPCWSTR& strFilter = lpstrFilter;
	std::wstring strTitle = L"";

public:
	std::pair<BOOL, std::wstring> OpenFile(DWORD nBufferMax = 256)
	{
		buffer.resize(nBufferMax);
		lpstrFile = buffer.data();
		nMaxFile = (DWORD)buffer.size();

		hwndOwner = hwndParent;
		lpstrTitle = strTitle.c_str();
		Flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_FILEMUSTEXIST;

		BOOL ret = GetOpenFileNameW(this);
		return std::make_pair(ret, std::wstring(lpstrFile));
	}
	std::pair<BOOL, std::wstring> SaveFile(DWORD nBufferMax = 256)
	{
		buffer.resize(nBufferMax);
		lpstrFile = buffer.data();
		nMaxFile = (DWORD)buffer.size();

		hwndOwner = hwndParent;
		lpstrTitle = strTitle.c_str();
		Flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_OVERWRITEPROMPT;

		BOOL ret = GetSaveFileNameW(this);
		return std::make_pair(ret, std::wstring(lpstrFile));
	}
};