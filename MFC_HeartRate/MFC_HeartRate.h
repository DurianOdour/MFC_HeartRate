
// MFC_HeartRate.h : PROJECT_NAME ���ε{�����D�n���Y��
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�� PCH �]�t���ɮ׫e���]�t 'stdafx.h'"
#endif

#include "resource.h"		// �D�n�Ÿ�


// CMFC_HeartRateApp: 
// �аѾ\��@�����O�� MFC_HeartRate.cpp
//

class CMFC_HeartRateApp : public CWinApp
{
public:
	CMFC_HeartRateApp();

// �мg
public:
	virtual BOOL InitInstance();

// �{���X��@

	DECLARE_MESSAGE_MAP()
};

extern CMFC_HeartRateApp theApp;