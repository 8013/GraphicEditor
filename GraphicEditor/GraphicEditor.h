
// GraphicEditor.h : GraphicEditor Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CGraphicEditorApp:
// �йش����ʵ�֣������ GraphicEditor.cpp
//

class CGraphicEditorApp : public CWinApp{
public:
	CGraphicEditorApp();

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGraphicEditorApp theApp;
