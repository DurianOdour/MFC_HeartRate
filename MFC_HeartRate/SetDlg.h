#pragma once
#include "afxwin.h"


// CSetDlg ��ܤ��

class CSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetDlg)

public:
	CSetDlg(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~CSetDlg();

// ��ܤ�����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩
	HICON m_hIcon;
	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_Image_ShouderROI;
};
