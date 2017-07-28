
// MFC_HeartRateDlg.h : 標頭檔
//

#pragma once
#include"cv.h"
#include"highgui.h"
#include<iostream>
#include<fstream>
#include"time.h"
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
	MMRESULT FTimerID;
	void DoEvent();
	double GetHeartRate(complex * in_data);
	void ShiftData();
	void FaceDetect();

	CthreadParam m_threadPara;
	CWinThread*  m_lpThread;
	static UINT threadFun(LPVOID LParam);
	void WriteTxt(void);
	void ShowImage(cv::Mat Image, CWnd * pWnd);
	void Thread_Image_RGB(LPVOID lParam);

	void Thread_ListenTime(LPVOID lParam);


	afx_msg void OnBnClickedButtonDetection();
	void GetMeanAndFourier(float * Data, int dataNo, complex * out_data);
	afx_msg void OnBnClickedButtonStart();
	CascadeClassifier face_cascade;
	static CvCapture *cap;
	static Mat frame;
	static cv::Point LT, RB ;
	Mat Img_ROI;
	std::vector<Rect> faces;
	float GData[1000] = { 0 };
	static float time;
	void DFT(int data_no, complex * in_data, complex * out_data);
	float MeanofGreen(Mat img);
	bool f_ROI_successed = false;
	bool f_SampleDone = false;
	int dataNum = 0;
	static time_t t_start;
	CStatic m_ImgShow;
	complex *G_Fourier;
};
