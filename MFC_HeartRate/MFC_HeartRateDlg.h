
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
void CALLBACK TimeProc(UINT uTimerID, UINT uMsg,DWORD dwUser, DWORD dw1, DWORD dw2);
void CALLBACK TimeProc2(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
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
	/****************/
	float p0 = 5;//狀態預估模型共變異數
	double K_statement[2] = { 60,60 };//狀態值
	float Q = 3;//速度雜訊
	double RR = 5; //觀測之量測向量雜訊的共變異數
	double K_Prediction =0; //速度預測
	double kg = 3;// 卡爾曼增益
	/*****************************/
	double referenceFrequency = 1.0;
	vectord G_signal, b_coeff, a_coeff;
	vectord S_signal, HR_vec, HR_KF;
	MMRESULT FTimerID;
	CFilter filter;
	void DoEvent();
	void DoEvent2();
	

	
	CthreadParam m_threadPara;
	CWinThread*  m_lpThread;
	static UINT threadFun(LPVOID LParam);
	
	void ShowImage(cv::Mat Image, CWnd * pWnd);
	void LoadData();
	void getFourier(vectord & InputData, complex * outFourierData);
	void Thread_Image_RGB(LPVOID lParam);
	void WriteTxt(vectord FilterData, const char filename[]);
	void WriteAddTxt(vectord FilterData, const char filename[]);
	double PixelCalculate(Mat Image);
	double GetHeartRate(complex * in_data, int dataLength);

	afx_msg void OnBnClickedButtonDetection();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	static CvCapture *cap,*cap2;
	static Mat frame, Img_ROI;
	static IplImage* frame_Shoulder;
	static CvPoint LBtnDown, LBtnUp;
	static CascadeClassifier face_cascade;
	static Rect rect_buffer;
	


	static float time;
	void DFT(int data_no, complex * in_data, complex * out_data);

	double MeanofGreen(Mat img);
	
	bool f_ROI_successed = false;
	bool f_SampleDone = false;
	
	static time_t t_start;
	CStatic m_ImgShow;
	CStatic m_Image_Shoulder;
	
	
};
