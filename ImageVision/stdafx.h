// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif






#ifndef ______INCLUDE_FILE______
#define ______INCLUDE_FILE______


#define MAX_CAMERA 1

#define u8t	unsigned char

//ȫ�ֱ�������
enum{
	INVALID = 0, NoError = 0, STOP = 0, PAUSE = 1, DETECTION = 1,CAPANDDECTION = 2,RUN = 2, SHOWEFFECT = 3, SHOWIMAGE = 4
};
//ȫ�ֱ�������

enum{
	NoCard, V1380LEADTECH
};

#include "opencv.h"
//#include "ocr.h"

#ifdef _DEBUG

#include "opencv2\opencv.hpp"

#include "imgDeal.h"
#pragma comment(lib,"../x64/DEBUG/img0.0.1.lib")



//#include "opencv2\opencv.hpp"

#else
#include "imgDeal.h"

#pragma comment(lib,"../x64/Release/img0.0.1.lib")
#endif

#include "motioncard/MotionCard.h"
//#ifdef __MOTION_CARD__
//#pragma comment(lib,"motioncard/Dmc1380.lib")
//#endif

#include "include\Preferences.h"
#ifdef _WIN64

#include "opencv2\opencv.hpp"

#ifdef _DEBUG
#pragma comment(lib,"opencv/x64/opencv_world310.lib")
#else
#pragma comment(lib,"opencv/x64/opencv_world310.lib")
#endif

#else

#pragma comment(lib,"camera/lib/x86/GxIAPI.lib")
#pragma comment(lib,"camera/lib/x86/DxImageProc.lib")

#endif


#define WMMSG_PARAMESSAGE WM_USER+1001
#define WMMSG_UPDATEUI WM_USER+1002

#include "camera\DahengCam.h"

class exEvent
{
public:
	exEvent()
		:evt(FALSE, TRUE)
		, isSetEvent(false)
		, Wmessage(0)
		, Lmessage(0)
	{
	};
	~exEvent(){};

	bool SetEvent()		{ evt.SetEvent();	isSetEvent = true;	 return true; }
	bool ResetEvent()	{ evt.ResetEvent(); isSetEvent = false;	 return false; }
	bool EventState()	{ return isSetEvent; }

	unsigned int Wmessage;
	unsigned int Lmessage;
	CEvent evt;
private:
	bool isSetEvent;
};


#define IN_PRODUCT_COME		1
#define OUT_FRONT			0x02
#define OUT_BACK			0x04
#define OUT_NO_PRODUCT		3
#define OUT_ERROR			4
#define OUT_SOFT_START		5
#define OUT_OVER_DETECT		6

class __global
{
public:
	__global()
		:CameraNum(1)
		, SelTest(0)
		, IO_Device(0)
		, AutoDetect(RUN)
		, isCapture(false)
		, hog_width(512) //default 64  //16�ı���
		, hog_height(64) //default 128
		, imgReuslt(0)
		, isRect(false)
		, widthRatio(0.0)
		, heightRatio(0.0)
		, CameraMat_Mutex(FALSE, NULL, NULL)
		, isShowOver(false)
		, delay_display(false)
		, testPicTime(0)
	{
		detectNo = 0;
		fontNo = 0;
		backNo = 0;
		absentNo = 0;
	};
	virtual ~__global()
	{
	};

public:
	//��ǰ�����������
	int CameraNum;
	//test
	int SelTest;
	//IO������
	int IO_Device;
	//���ƽ���״̬
	int AutoDetect;
	bool isCapture;
	bool isShowOver;
	//��Ƥ���Ƿ����
	bool isRect;
	//���ڸ���Ȥ������ɫ����
	uchar cvcolor;
	bool b_cvcolor;
	//���ڼ������Ȥ����
	Rect ImgROI, MFCROI, ImgFrame, MFCFrame;
	//���ڼ������Ȥ�������
	double widthRatio, heightRatio;
	//daheng ���
	int detectNo;
	int fontNo;
	int backNo;
	int absentNo;

	int imgReuslt;
	int hog_width;
	int hog_height;
	
	//��ONpaint���ã���Ϊmfc����ĳЩ���������Ժ��Դ���ˢ�£����¿�����ͼƬ����OnPaint���ú��ˢ�»���
	bool delay_display;
	//�洢�˽��
#ifdef __AFXWIN_H__
	//WIN32 API
	exEvent evt_capture;
	exEvent evt_capture1;
	exEvent evt_capture2;
	exEvent evt_capture3;
	//��������
	exEvent evt_procedure;
	//�������
	CMutex CameraMat_Mutex;//��������Ϊ�������ȡ�����ݲ���ͻ��������
	exEvent evt_detect;
	CString resultmsg;
	CString train_data_path;
	vector<CString> TopSideImgPath;
	vector<CString> BottomSideImgPath;
	vector<CString> testPic;
	int testPicTime;
#else
	//standard C++
	vector<std::string> TopSideImgPath;
	vector<std::string> BottomSideImgPath;
	std::string train_data_path;
	std::string resultmsg;
#endif
	opencv Opencv;
//	Ocr ocr;
	CPreferences ini;
	Motion mc;

	struct tagTEST
	{
		int val1;
		int val2;
		double val3;
		int val4;
		int val5;
			

	};
	tagTEST mytest;
};
extern __global g;





#endif




#ifndef SELECT_FOLDER_DLG_H  
#define SELECT_FOLDER_DLG_H  


#ifndef BIF_NEWDIALOGSTYLE  
#define  BIF_NEWDIALOGSTYLE  0x0040  
#endif  

class CSelectFolderDlg
{
public:
	//����һ��ѡ���ļ��еĶԻ��򣬷�����ѡ·��  
	static CString Show()
	{
		TCHAR           szFolderPath[MAX_PATH] = { 0 };
		CString         strFolderPath = TEXT("");

		BROWSEINFO      sInfo;
		::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
		sInfo.pidlRoot = 0;
		sInfo.lpszTitle = _T("��ѡ��һ���ļ��У�");
		sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
		sInfo.lpfn = NULL;

		// ��ʾ�ļ���ѡ��Ի���  
		LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
		if (lpidlBrowse != NULL)
		{
			// ȡ���ļ�����  
			if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
			{
				strFolderPath = szFolderPath;
			}
		}
		if (lpidlBrowse != NULL)
		{
			::CoTaskMemFree(lpidlBrowse);
		}

		return strFolderPath;

	}

};

#endif

//#define EXCEPTION_EXECUTE_HANDLER 0


#include "encrypt.h"


//#define _TEST 1