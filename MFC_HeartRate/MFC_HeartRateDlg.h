
// MFC_HeartRateDlg.h : ���Y��
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

//***************************�h�C��p�ɾ�
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
// CMFC_HeartRateDlg ��ܤ��
class CMFC_HeartRateDlg : public CDialogEx
{
	// �غc
public:
	CMFC_HeartRateDlg(CWnd* pParent = NULL);	// �зǫغc�禡

												// ��ܤ�����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_HEARTRATE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �䴩
												// �{���X��@
protected:
	HICON m_hIcon;

	// ���ͪ��T�������禡
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	/****************/
	float p0 = 5;//���A�w���ҫ��@�ܲ���
	double K_statement[2] = { 60,60 };//���A��
	float Q = 3;//�t�����T
	double RR = 5; //�[�����q���V�q���T���@�ܲ���
	double K_Prediction =0; //�t�׹w��
	double kg = 3;// �d���ҼW�q
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
