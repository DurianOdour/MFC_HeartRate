
// MFC_HeartRateDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "MFC_HeartRate.h"
#include "MFC_HeartRateDlg.h"
#include "afxdialogex.h"
const int FPS=30;
clock_t t0 ;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
Mat CMFC_HeartRateDlg::frame(640, 480, CV_8UC3);
Mat CMFC_HeartRateDlg::Img_ROI(50, 50, CV_8UC3);
IplImage *CMFC_HeartRateDlg::frame_Shoulder = cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);
CvCapture *CMFC_HeartRateDlg::cap;
CvCapture *CMFC_HeartRateDlg::cap2;
CvPoint CMFC_HeartRateDlg::LBtnDown = { 0 };
CvPoint CMFC_HeartRateDlg::LBtnUp = { 0 };
CascadeClassifier CMFC_HeartRateDlg::face_cascade; 
float CMFC_HeartRateDlg::time = 0;
time_t CMFC_HeartRateDlg::t_start;
Rect CMFC_HeartRateDlg::rect_buffer;
CMFC_HeartRateDlg::CMFC_HeartRateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFC_HEARTRATE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC_HeartRateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ImageShow, m_ImgShow);
	DDX_Control(pDX, IDC_Image_Shoulder, m_Image_Shoulder);
}

BEGIN_MESSAGE_MAP(CMFC_HeartRateDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_Detection, &CMFC_HeartRateDlg::OnBnClickedButtonDetection)
	ON_BN_CLICKED(IDC_BUTTON_Start, &CMFC_HeartRateDlg::OnBnClickedButtonStart)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CMFC_HeartRateDlg 訊息處理常式

BOOL CMFC_HeartRateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

									// TODO: 在此加入額外的初始設定
	m_ImgShow.SetWindowPos(NULL, 640, 0, 480, 360, SWP_SHOWWINDOW);
	m_Image_Shoulder.SetWindowPos(NULL, 0, 0, 640, 480, SWP_SHOWWINDOW);

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CMFC_HeartRateDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CMFC_HeartRateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CALLBACK TimeProc(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	CMFC_HeartRateDlg *pointer = (CMFC_HeartRateDlg *)dwUser;
	pointer->DoEvent();
}
void CALLBACK TimeProc2(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	CMFC_HeartRateDlg *pointer = (CMFC_HeartRateDlg *)dwUser;
	pointer->DoEvent2();
}
void CMFC_HeartRateDlg::DoEvent2()
{
	Mat image_shoulder = frame_Shoulder;
	Rect roi_shoulder = Rect(LBtnDown.x, LBtnDown.y, LBtnUp.x - LBtnDown.x, LBtnUp.y - LBtnDown.y);
	Mat image_gray = image_shoulder(roi_shoulder);
	Mat image_canny;
	
	cvtColor(image_gray, image_gray, CV_BGR2GRAY);
	for (unsigned int i = 0; i < 5; i++)
	{
	IplConvKernel *pKernel = NULL;
	pKernel = cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_RECT, NULL);
	cvDilate(&IplImage(image_gray), &IplImage(image_gray), cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_RECT, NULL), 1);
	cvErode(&IplImage(image_gray), &IplImage(image_gray), pKernel, 1);
	}
	threshold(image_gray,image_gray,200,255,CV_THRESH_BINARY);
	S_signal.push_back(PixelCalculate(image_gray));
	if(S_signal.size()==600)
	WriteTxt(S_signal, "S_signal.txt");
}
double CMFC_HeartRateDlg::PixelCalculate(Mat Image)
{
	int upstep = 0;
	int downstep = 0;
	upstep=cv::sum(Image).val[0];
	upstep = upstep / 255;
	downstep = Image.rows*Image.cols- upstep;
	return (double)(upstep- downstep)/(upstep+downstep);
}
void CMFC_HeartRateDlg::WriteTxt(vectord FilterData,const char filename[])
{

	std::fstream fp;
	fp.open(filename, std::ios::out);


	if (!fp) {//如果開啟檔案失敗，fp為0；成功，fp為非0

	}
	for (int i = 0; i < FilterData.size(); i++)
	{
		fp << FilterData[i] << "\n";
	}
	fp.close();

}
void CMFC_HeartRateDlg::WriteAddTxt(vectord FilterData, const char filename[])
{

	std::fstream fp;
	fp.open(filename, std::ios::app);


	if (!fp) {//如果開啟檔案失敗，fp為0；成功，fp為非0

	}
	for (int i = 0; i < FilterData.size(); i++)
	{
		fp << FilterData[i] << "\n";
	}
	fp.close();

}
void CMFC_HeartRateDlg::DoEvent()
{
	Mat face = Img_ROI;
	
	clock_t tend;
	if (!f_SampleDone)
	{
	
		G_signal.push_back(MeanofGreen(face));
		CString str; str.Format(_T("%d"), G_signal.size());
		SetDlgItemText(IDC_STATIC_DataNum, str);
	}
	if (G_signal.size()>=FPS*30 && !f_SampleDone)
	{
		vectord G_filtfilt_out;
		filter.filtfilt(b_coeff,a_coeff, G_signal, G_filtfilt_out);
		
		complex *G_signal_fourier = new complex[G_signal.size()];
		getFourier(G_filtfilt_out, G_signal_fourier);
		double HR = GetHeartRate(G_signal_fourier, G_signal.size());
		referenceFrequency = HR / 60;
		HR_vec.push_back(HR);
		delete [] G_signal_fourier;
		CString str; str.Format(_T("%.2f"), HR);
		SetDlgItemText(IDC_HR, str);
		f_SampleDone = true;
		tend = clock();
		float t = (float)(t0 - tend) / CLK_TCK;
		int a = 9;
	}
	if (f_SampleDone)
	{ 
	  G_signal.push_back(MeanofGreen(face));
	
	  if (G_signal.size() >= 910)
	  { 
		for (unsigned int i = 0; i < G_signal.size() - 10; i++)
		{
			  G_signal[i] = G_signal[i + 10];
		}

		G_signal.resize(900);
		vectord G_filtfilt_out;
		filter.filtfilt(b_coeff, a_coeff, G_signal, G_filtfilt_out);
		complex *G_signal_fourier = new complex[G_signal.size()];
		getFourier(G_filtfilt_out, G_signal_fourier);
	 /*****************************************/
		K_Prediction = K_statement[1];//狀態預測
		p0 = p0 + Q;//計算共變異矩陣
		kg = p0 / (p0 + RR);//卡爾曼增益
		K_statement[0] = K_Prediction + kg*(HR_vec[HR_vec.size()-1] - K_Prediction);
		K_statement[1] = K_statement[0];
		HR_KF.push_back(K_statement[0]);
		p0 = (1 - kg)*p0;//更新共變異矩陣，1為單位為矩陣
		double HR = GetHeartRate(G_signal_fourier, G_signal.size());
		referenceFrequency = K_statement[0] / 60;
		HR_vec.push_back(HR); 
	/*******************************************/
		if (HR_vec.size() % 2000 == 0) {
			WriteAddTxt(HR_vec, "HR.txt");
			HR_vec.resize(0);
			WriteAddTxt(HR_vec, "HR_KF.txt");
			HR_KF.resize(0);
			
		}
		CString str; str.Format(_T("%.1f"), HR);
		
		delete[]G_signal_fourier;
		SetDlgItemText(IDC_HR, str);
		}
	}
}
double CMFC_HeartRateDlg::GetHeartRate(complex *in_data,int dataLength)
{
	double HR = 0;
	double Attitude [70][2];
	int counter = 0;
	
	for (uint16_t i =(double) (referenceFrequency-0.3)*dataLength / FPS; i < (double)(referenceFrequency+0.3)*dataLength / FPS; i++)
	{
		Attitude[counter][0] = sqrt(pow(in_data[i + 1].re, 2) + pow(in_data[i + 1].im, 2));
		Attitude[counter][1] = (double)i*FPS/dataLength;
		counter++;
	}
	double peak[40][2];
	uint16_t peak_counter=0;
	for (size_t i = 1; i < counter-2; i++)
	{
		if (Attitude[i - 1][0]<Attitude[i][0] && Attitude[i][0]> Attitude[i + 1][0])
		{
			peak[peak_counter][0] = Attitude[i][0];
			peak[peak_counter][1] = Attitude[i][1];
			peak_counter++;
		}
	}
	/********************************************/
	double temp[1][2];
	for (uint16_t i = 0; i < peak_counter; i++) {
		for (uint16_t j = i; j < peak_counter; j++) {
			if (peak[j][0] > peak[i][0]) {
				temp[0][0] = peak[j][0]; temp[0][1] = peak[j][1];
				peak[j][0] = peak[i][0]; peak[j][1] = peak[i][1];
				peak[i][0] = temp[0][0]; peak[i][1] = temp[0][1];
			}
		}
	}
	
	double min = 10;

	for (uint16_t i = 0; i <2; i++)
	{
		if (abs(referenceFrequency - peak[i][1]) < min)
		{
			min = abs(referenceFrequency - peak[i][1]);
			HR = peak[i][1] * 60;	
		}
	}
	return HR;
}

void CMFC_HeartRateDlg::DFT(int data_no, complex *in_data, complex *out_data)
{

	int m, k;
	float w_cos, w_sin, angle_step, angle;

	angle_step = 2 * M_PI / data_no;
	for (m = 0; m<data_no; m++)
	{
		out_data[m].re = in_data[0].re;
		out_data[m].im = in_data[0].im;
		for (k = 1; k<data_no; k++)
		{
			angle = (float)m*k*angle_step;
			w_cos = cos(angle);
			w_sin = sin(angle);
			out_data[m].re += in_data[k].re * w_cos - in_data[k].im * w_sin;
			out_data[m].im += in_data[k].re * w_sin + in_data[k].im * w_cos;
		}
	}
}
double CMFC_HeartRateDlg::MeanofGreen(Mat img)
{

	int sum = 0;
	double mean;
	cv::Mat_<cv::Vec3b>::iterator it = img.begin<cv::Vec3b>();
	cv::Mat_<cv::Vec3b>::iterator itend = img.end<cv::Vec3b>();

	for (; it != itend; ++it) {
		sum = sum + (*it)[1];
	}
	mean = (double)sum / (double)(Img_ROI.rows*Img_ROI.cols);
	return mean;
	
}
void CMFC_HeartRateDlg::OnBnClickedButtonDetection()
{
	//多媒體計時器參數設定
	int SampleTime = 1000 / FPS;
	m_threadPara.m_case = 1;
	m_threadPara.hWnd = m_hWnd;
	m_lpThread = AfxBeginThread(&CMFC_HeartRateDlg::threadFun, (LPVOID)&m_threadPara);
	UINT uDelay = int(SampleTime);//m_SampleTime 為自訂的取樣時間 單位:毫秒
	//UINT uDelay2 = (int)50;
	UINT uResolution = 1;
	DWORD dwUser = (DWORD)this;
	UINT fuEvent = TIME_PERIODIC; //You also choose TIME_ONESHOT;

	timeBeginPeriod(1); //精度1ms
	FTimerID = timeSetEvent(uDelay, uResolution, TimeProc, dwUser, fuEvent);
	//FTimerID = timeSetEvent(uDelay2, uResolution, TimeProc2, dwUser, fuEvent);
	LoadData();
	t0 = clock();

}

void CMFC_HeartRateDlg::OnBnClickedButtonStart()
{
	face_cascade.load("haarcascade_frontalface_alt.xml");
	cap= cvCaptureFromCAM(1);
	//cap2 = cvCaptureFromCAM(2);
	m_threadPara.m_case = 0;
	m_threadPara.hWnd = m_hWnd;
	m_lpThread = AfxBeginThread(&CMFC_HeartRateDlg::threadFun, (LPVOID)&m_threadPara);
}
void CMFC_HeartRateDlg::Thread_Image_RGB(LPVOID lParam)
{
	CthreadParam * Thread_Info = (CthreadParam *)lParam;
	CMFC_HeartRateDlg * hWnd = (CMFC_HeartRateDlg *)CWnd::FromHandle((HWND)Thread_Info->hWnd);
	IplImage * img_buffer,*img_buffer2;
	img_buffer = cvCreateImage(cvSize(480, 360), IPL_DEPTH_8U, 3);
	Mat img_show;
	while (1)
	{
		img_buffer = cvQueryFrame(cap);
		//img_buffer2 = cvQueryFrame(cap2);
		if (img_buffer != nullptr) {
			frame = img_buffer;
			std::vector<Rect> faces;
			Mat frame_gray;
			cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
			equalizeHist(frame_gray, frame_gray);
			face_cascade.detectMultiScale(frame_gray, faces, 1.2, 3, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));//FPS下降
			if (faces.size() >= 1)
			{
				Img_ROI = frame(faces[0]);
				rectangle(frame, faces[0], CV_RGB(255, 0, 0));
				rect_buffer = faces[0];
			}
			if(faces.size()==0)Img_ROI = frame(rect_buffer);

			hWnd->ShowImage(frame, hWnd->GetDlgItem(IDC_ImageShow));
		}
	/*	if (img_buffer2 != nullptr)
		{
			cvCopy(img_buffer2, frame_Shoulder);
			IplImage* show = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 3);
			cvCopy(img_buffer2, show);
			cvRectangle(show,LBtnDown,LBtnUp,CV_RGB(255,0,0),2);
			hWnd->ShowImage(show, hWnd->GetDlgItem(IDC_Image_Shoulder));
			cvReleaseImage(&show);
		}*/
	}

}

UINT CMFC_HeartRateDlg::threadFun(LPVOID LParam)
{
	CthreadParam* para = (CthreadParam*)LParam;
	CMFC_HeartRateDlg* lpview = (CMFC_HeartRateDlg*)(para->m_lpPara);
	para->m_blthreading = TRUE;

	switch (para->m_case)
	{
	case 0:
		lpview->Thread_Image_RGB(LParam);
		break;
	case 1:
	
		break;
	case 2:

	default:
		break;
	}

	para->m_blthreading = FALSE;
	para->m_case = 0xFF;
	return 0;

}

void CMFC_HeartRateDlg::ShowImage(cv::Mat Image, CWnd* pWnd)
{
	//Windows中顯示圖像存在一個4位元組對齊的問題, 也就是每一行的位元組數必須是4的倍數.
	//而Mat的資料是連續存儲的.一般Mat的資料格式為BGR, 也就是一個圖元3個位元組, 假設我的圖片一行有5個圖元, 那一行就是15個位元組, 這不符合MFC的資料對齊方式,
	//如果我們直接把Mat的data加個資料頭再顯示出來就可能會出錯.
	//手動4位元組對齊, 就是計算每行的位元組是不是4的倍數, 不是的話, 在後面補0
	//但是我們把圖片轉成RGBA之後, 一個圖元就是4個位元組, 不管你一行幾個圖元, 一直都是對齊的.

	cv::Mat imgTmp;
	CRect rect;
	pWnd->GetClientRect(&rect);
	cv::resize(Image, imgTmp, cv::Size(rect.Width(), rect.Height()));

	switch (imgTmp.channels())
	{
	case 1:
		cv::cvtColor(imgTmp, imgTmp, CV_GRAY2BGRA);
		break;
	case 3:
		cv::cvtColor(imgTmp, imgTmp, CV_BGR2BGRA);
		break;
	default:
		break;
	}
	int pixelBytes = imgTmp.channels()*(imgTmp.depth() + 1);
	BITMAPINFO bitInfo;
	bitInfo.bmiHeader.biBitCount = 8 * pixelBytes;
	bitInfo.bmiHeader.biWidth = imgTmp.cols;
	bitInfo.bmiHeader.biHeight = -imgTmp.rows;
	bitInfo.bmiHeader.biPlanes = 1;
	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biCompression = BI_RGB;
	bitInfo.bmiHeader.biClrImportant = 0;
	bitInfo.bmiHeader.biClrUsed = 0;
	bitInfo.bmiHeader.biSizeImage = 0;
	bitInfo.bmiHeader.biXPelsPerMeter = 0;
	bitInfo.bmiHeader.biYPelsPerMeter = 0;

	CDC *pDC = pWnd->GetDC();
	::StretchDIBits(
		pDC->GetSafeHdc(),
		0, 0, rect.Width(), rect.Height(),
		0, 0, rect.Width(), rect.Height(),
		imgTmp.data,
		&bitInfo,
		DIB_RGB_COLORS,
		SRCCOPY
	);
	ReleaseDC(pDC);
}
void CMFC_HeartRateDlg::LoadData()
{
	
	std::fstream FilterReader_a, FilterReader_b ;
	FilterReader_a.open("FilterParameter_a.txt", std::ios::in); FilterReader_b.open("FilterParameter_b.txt", std::ios::in);
	for (unsigned int i = 0; i < 9; i++) {
		double temp;
		FilterReader_a >> temp;
		a_coeff.push_back(temp);
		FilterReader_b >> temp;
		b_coeff.push_back(temp);
	} FilterReader_a.close(); FilterReader_b.close();
}
void CMFC_HeartRateDlg::getFourier(vectord &InputData,complex *outFourierData)
{
	complex *data_temp = new complex[InputData.size()];
	for (unsigned int i = 0; i < InputData.size(); i++)
	{
		data_temp[i].re = InputData[i];
		data_temp[i].im = 0;
	}
	DFT(InputData.size(), data_temp, outFourierData);
	delete[] data_temp;

}

void CMFC_HeartRateDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (0 <= point.x && point.x <= 640 && 0 <= point.y && point.y<480 &&nFlags==MK_LBUTTON)
	{
		LBtnUp = { point.x ,point.y };
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CMFC_HeartRateDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (0 <= point.x && point.x <= 640 && 0<=point.y && point.y<480)
	{
		LBtnDown = { point.x,point.y };
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}
