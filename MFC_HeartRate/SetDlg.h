#pragma once
#include "afxwin.h"


// CSetDlg 對話方塊

class CSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetDlg)

public:
	CSetDlg(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CSetDlg();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	HICON m_hIcon;
	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_Image_ShouderROI;
};
