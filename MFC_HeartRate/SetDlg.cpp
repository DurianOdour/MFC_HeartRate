// SetDlg.cpp : ��@��
//

#include "stdafx.h"
#include "MFC_HeartRate.h"
#include "SetDlg.h"
#include "afxdialogex.h"


// CSetDlg ��ܤ��

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

	// �]�w����ܤ�����ϥܡC�����ε{�����D�������O��ܤ���ɡA
	// �ج[�|�۰ʱq�Ʀ��@�~
	SetIcon(m_hIcon, TRUE);			// �]�w�j�ϥ�
	SetIcon(m_hIcon, FALSE);		// �]�w�p�ϥ�

									// TODO: �b���[�J�B�~����l�]�w
	m_Image_ShouderROI.SetWindowPos(NULL, 0, 0, 1920, 1080, SWP_SHOWWINDOW);


	return TRUE;  // �Ǧ^ TRUE�A���D�z�ﱱ��]�w�J�I
}

BEGIN_MESSAGE_MAP(CSetDlg, CDialogEx)
END_MESSAGE_MAP()


// CSetDlg �T���B�z�`��
