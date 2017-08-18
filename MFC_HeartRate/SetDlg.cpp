// SetDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "MFC_HeartRate.h"
#include "SetDlg.h"
#include "afxdialogex.h"


// CSetDlg 對話方塊

IMPLEMENT_DYNAMIC(CSetDlg, CDialogEx)

CSetDlg::CSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CSetDlg::~CSetDlg()
{
}

void CSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Image_ShouderROI, m_Image_ShouderROI);
}
BOOL CSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

									// TODO: 在此加入額外的初始設定
	m_Image_ShouderROI.SetWindowPos(NULL, 0, 0, 1920, 1080, SWP_SHOWWINDOW);


	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

BEGIN_MESSAGE_MAP(CSetDlg, CDialogEx)
END_MESSAGE_MAP()


// CSetDlg 訊息處理常式
