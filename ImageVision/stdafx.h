// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。  
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。  
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持









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

//全局变量参数
enum{
	INVALID = 0, NoError = 0, STOP = 0, PAUSE = 1, DETECTION = 1,CAPANDDECTION = 2,RUN = 2, SHOWEFFECT = 3, SHOWIMAGE = 4
};
//全局变量参数

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
		, hog_width(512) //default 64  //16的倍数
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
	//当前链接相机数量
	int CameraNum;
	//test
	int SelTest;
	//IO卡号码
	int IO_Device;
	//控制界面状态
	int AutoDetect;
	bool isCapture;
	bool isShowOver;
	//橡皮筋是否可用
	bool isRect;
	//用于感兴趣区域颜色渐变
	uchar cvcolor;
	bool b_cvcolor;
	//用于计算感兴趣区域
	Rect ImgROI, MFCROI, ImgFrame, MFCFrame;
	//用于计算感兴趣区域比例
	double widthRatio, heightRatio;
	//daheng 相机
	int detectNo;
	int fontNo;
	int backNo;
	int absentNo;

	int imgReuslt;
	int hog_width;
	int hog_height;
	
	//给ONpaint调用，因为mfc里面某些函数调用以后会对窗口刷新，导致看不见图片，在OnPaint调用后给刷新回来
	bool delay_display;
	//存储了结果
#ifdef __AFXWIN_H__
	//WIN32 API
	exEvent evt_capture;
	exEvent evt_capture1;
	exEvent evt_capture2;
	exEvent evt_capture3;
	//动作流程
	exEvent evt_procedure;
	//触发检测
	CMutex CameraMat_Mutex;//互斥量，为了相机读取的数据不冲突产生崩溃
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
	//创建一个选择文件夹的对话框，返回所选路径  
	static CString Show()
	{
		TCHAR           szFolderPath[MAX_PATH] = { 0 };
		CString         strFolderPath = TEXT("");

		BROWSEINFO      sInfo;
		::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
		sInfo.pidlRoot = 0;
		sInfo.lpszTitle = _T("请选择一个文件夹：");
		sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
		sInfo.lpfn = NULL;

		// 显示文件夹选择对话框  
		LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
		if (lpidlBrowse != NULL)
		{
			// 取得文件夹名  
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