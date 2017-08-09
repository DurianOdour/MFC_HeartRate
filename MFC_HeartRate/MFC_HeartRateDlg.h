
// MFC_HeartRateDlg.h : 標頭檔
//

#pragma once
#include"cv.h"
#include"highgui.h"
#include<iostream>
#include<fstream>
#include"time.h"
#include"CFilter.h"
#define _USE_MATH_DEFINES
#include"math.h"

//***************************多媒體計時器
#include <windows.h>
#include <mmsystem.h>
#include "afxwin.h"
#pragma comment(lib, "winmm.lib")
void CALLBACK TimeProc(UINT uTimerID, UINT uMsg,
	DWORD dwUser, DWORD dw1, DWORD dw2);
//***********************************
using namespace cv;
struct complex
{
	double re;
	double im;
};
struct CthreadParam
{
public:
	HWND hWnd;
	LPVOID m_lpPara;
	UINT   m_case;
	BOOL m_blthreading;
};
// CMFC_HeartRateDlg 對話方塊
class CMFC_HeartRateDlg : public CDialogEx
{
	// 建構
public:
	CMFC_HeartRateDlg(CWnd* pParent = NULL);	// 標準建構函式

												// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_HEARTRATE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援


														// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	vectord G_signal, b_coeff, a_coeff;
	MMRESULT FTimerID;
	CFilter filter;
	void DoEvent();
	double GetHeartRate(complex * in_data, int dataLength);

	void FaceDetect();
	CthreadParam m_threadPara;
	CWinThread*  m_lpThread;
	static UINT threadFun(LPVOID LParam);
	
	void ShowImage(cv::Mat Image, CWnd * pWnd);
	void LoadData();
	void getFourier(vectord & InputData, complex * outFourierData);
	void Thread_Image_RGB(LPVOID lParam);
	void Thread_ListenTime(LPVOID lParam);

	afx_msg void OnBnClickedButtonDetection();
	afx_msg void OnBnClickedButtonStart();
	CascadeClassifier face_cascade;
	static CvCapture *cap;
	static Mat frame;
	static cv::Point LT, RB ;
	Mat Img_ROI;
	std::vector<Rect> faces;
	
	static float time;
	void DFT(int data_no, complex * in_data, complex * out_data);

	double MeanofGreen(Mat img);
	
	bool f_ROI_successed = false;
	bool f_SampleDone = false;
	
	static time_t t_start;
	CStatic m_ImgShow;
	
};
