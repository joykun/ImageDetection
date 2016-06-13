// control.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageVision.h"
#include "control.h"
#include "MainFrm.h"

// control

IMPLEMENT_DYNCREATE(control, CFormView)

control::control()
	: CFormView(control::IDD)
	, m_coord_x(0)
	, m_coord_y(0)
	, m_coord_width(0)
	, m_existmin(0)
	, m_existmax(0)
	, m_dialateDis(0)
	, m_wmin(0)
	, m_wmax(0)
	, m_hmin(0)
	, m_hmax(0)
	, m_ratio(0)
	, m_coord_height(0)
	, m_shutter(0)
	, m_delay(0)
{

}

control::~control()
{
}

void control::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_coord_x);
	DDX_Text(pDX, IDC_EDIT3, m_coord_y);
	DDX_Text(pDX, IDC_EDIT2, m_coord_width);
	DDX_Text(pDX, IDC_EDIT5, m_existmin);
	DDX_Text(pDX, IDC_EDIT6, m_existmax);
	DDX_Text(pDX, IDC_EDIT7, m_dialateDis);
	DDX_Text(pDX, IDC_EDIT8, m_wmin);
	DDX_Text(pDX, IDC_EDIT9, m_wmax);
	DDX_Text(pDX, IDC_EDIT10, m_hmin);
	DDX_Text(pDX, IDC_EDIT11, m_hmax);
	DDX_Text(pDX, IDC_EDIT4, m_ratio);
	DDX_Text(pDX, IDC_EDIT12, m_coord_height);
	DDX_Text(pDX, IDC_EDIT_SHUTTER, m_shutter);
	DDX_Text(pDX, IDC_EDIT_DELAY, m_delay);
}

BEGIN_MESSAGE_MAP(control, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &control::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_ROI, &control::OnBnClickedButtonRoi)
	ON_BN_CLICKED(IDC_BUTTON_ONLYSVM, &control::OnBnClickedButtonOnlysvm)

	ON_MESSAGE(WMMSG_UPDATEUI, &control::UpdateUI)
	ON_BN_CLICKED(IDC_BUTTON_TOP_IMG, &control::OnBnClickedButtonTopImg)
	ON_BN_CLICKED(IDC_BUTTON_BOTTOM_IMG, &control::OnBnClickedButtonBottomImg)
	ON_BN_CLICKED(IDC_BUTTON_OPENDIR, &control::OnBnClickedButtonOpendir)
//	ON_BN_CLICKED(IDC_BUTTON_BOTTOM_TRAIN, &control::OnBnClickedButtonBottomTrain)
//	ON_BN_CLICKED(IDC_BUTTON_BOTTOM_IMG2, &control::OnBnClickedButtonBottomImg2)
ON_BN_CLICKED(IDC_BUTTON_DEFAULTTRAIN, &control::OnBnClickedButtonDefaulttrain)
ON_BN_CLICKED(IDC_BUTTONTEST, &control::OnBnClickedButtontest)
//ON_BN_CLICKED(IDC_BUTTON_SAVEDELAY, &control::OnBnClickedButtonSavedelay)
ON_BN_CLICKED(IDC_BUTTON_DELAYSV, &control::OnBnClickedButtonDelaysv)
END_MESSAGE_MAP()


// control 诊断

#ifdef _DEBUG
void control::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void control::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// control 消息处理程序


void control::OnBnClickedButtonSave()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	g.ImgROI.x = m_coord_x;
	g.ImgROI.y = m_coord_y;
	g.ImgROI.width = m_coord_width;
	g.ImgROI.height = m_coord_height;
	g.ini.m_nSear_X = m_coord_x;
	g.ini.m_nSear_Y = m_coord_y;
	g.ini.m_nSear_W = m_coord_width;
	g.ini.m_nSear_H = m_coord_height;
	g.ini.m_iExistMin = m_existmin;
	g.ini.m_iExistMax = m_existmax;
	g.ini.m_blocksize = m_dialateDis;
	g.ini.m_iExistHeightMax = m_hmax;		//物体存在高度像素上限
	g.ini.m_iExistHeightMin = m_hmin;		//物体存在高度像素下限
	g.ini.m_iExistWidthMax = m_wmax;		//物体存在宽度像素上限
	g.ini.m_iExistWidthMin = m_wmin;		//物体存在宽度像素下限
	g.ini.m_nWHRatio = m_ratio;
	g.ini.m_shutter = m_shutter;
	g.ini.SaveParaFile(PARA_ALL);

	//注意查看分割窗口后获得的指针ImageVisionView
	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CView *pview = (CView *)pMain->ImageVisionView;
	pview->SendMessage(WMMSG_PARAMESSAGE, 1, 1); //发消息

	AfxMessageBox(L"参数设置成功");
}


void control::OnBnClickedButtonRoi()
{
	// TODO:  在此添加控件通知处理程序代码
	if (STOP != g.AutoDetect)
		g.Opencv.m_RawImg = g.Opencv.Cameramat.clone();

	if (!g.Opencv.m_RawImg.empty()) g.Opencv.mat0 = g.Opencv.m_RawImg.clone();

	g.AutoDetect = STOP;
	g.isRect = true;
}


void control::OnBnClickedButtonOnlysvm()
{
	// TODO:  在此添加控件通知处理程序代码
	if (0 != g.ini.m_imageDealMode)
	{
		g.ini.m_imageDealMode = 0;
		GetDlgItem(IDC_BUTTON_ONLYSVM)->SetWindowText(L"只包含分类器方法");
	}
	else
	{
		g.ini.m_imageDealMode = 1;
		GetDlgItem(IDC_BUTTON_ONLYSVM)->SetWindowText(L"旧方法+分类器法");
	}
	UpdateData(FALSE);
	g.ini.SaveParaFile(PARA_ALL);
}

LRESULT control::UpdateUI(WPARAM wParam, LPARAM lParam)
{


	g.ImgROI.x = g.ini.m_nSear_X;
	g.ImgROI.y = g.ini.m_nSear_Y;
	g.ImgROI.width = g.ini.m_nSear_W;
	g.ImgROI.height = g.ini.m_nSear_H;
	m_coord_x = g.ini.m_nSear_X;
	m_coord_y = g.ini.m_nSear_Y;
	m_coord_width = g.ini.m_nSear_W;
	m_coord_height = g.ini.m_nSear_H;
	m_existmin = g.ini.m_iExistMin;
	m_existmax = g.ini.m_iExistMax;
	m_dialateDis = g.ini.m_blocksize;

	m_hmax = g.ini.m_iExistHeightMax;		//物体存在高度像素上限
	m_hmin = g.ini.m_iExistHeightMin;		//物体存在高度像素下限
	m_wmax = g.ini.m_iExistWidthMax;		//物体存在宽度像素上限
	m_wmin = g.ini.m_iExistWidthMin;		//物体存在宽度像素下限
	m_ratio = g.ini.m_nWHRatio;
	m_shutter = g.ini.m_shutter;
	m_delay = g.ini.m_delay;


	if (0 != g.ini.m_imageDealMode)
		GetDlgItem(IDC_BUTTON_ONLYSVM)->SetWindowText(L"旧方法+分类器法");
	else
		GetDlgItem(IDC_BUTTON_ONLYSVM)->SetWindowText(L"只包含分类器方法");

	Sleep(100);
	UpdateData(FALSE);
	return 0;
}

void control::OnBnClickedButtonTopImg()
{
	// TODO:  在此添加控件通知处理程序代码
	Mat src;
	if (RUN == g.AutoDetect)
		src = g.Opencv.Cameramat.clone();
	else
		src = g.Opencv.m_RawImg.clone();
	g.Opencv.SaveImage(src, L"Image\\正面", L"");
}


void control::OnBnClickedButtonBottomImg()
{
	// TODO:  在此添加控件通知处理程序代码
	Mat src;
	if (RUN == g.AutoDetect)
		src = g.Opencv.Cameramat.clone();
	else
		src = g.Opencv.m_RawImg.clone();
	g.Opencv.SaveImage(src, L"Image\\反面", L"");
}


void control::OnBnClickedButtonOpendir()
{
	// TODO:  在此添加控件通知处理程序代码
	char* szCmdLine = "explorer.exe Image";
	WinExec(szCmdLine, SW_SHOW);
}


void control::OnBnClickedButtonDefaulttrain()
{
	// TODO:  在此添加控件通知处理程序代码
	
	GetDir(L"Image\\正面", g.TopSideImgPath);
	GetDir(L"Image\\反面", g.BottomSideImgPath);
	g.Opencv.hog_svm_train();
}


bool control::GetDir(CString strPicDir, vector<CString>& strPath)
{
	strPath.clear();
	CFileFind fileFinder;

	if (strPicDir.Right(1) == TEXT("\\"))
	{
		int nPos = strPicDir.ReverseFind(TEXT('\\'));
		strPicDir = strPicDir.Left(nPos);
	}

	CString strPicFile = TEXT("");
	strPicFile.Format(TEXT("%s\\%s"), strPicDir, TEXT("*.*"));

	BOOL bWorking = fileFinder.FindFile(strPicFile);
	while (bWorking)
	{
		bWorking = fileFinder.FindNextFile();
		if (fileFinder.IsDots())
		{
			continue;
		}

		CString strFilePath = fileFinder.GetFilePath();
		//		if (fileFinder.IsDirectory())
		//		{
		//继续遍历目录
		//			ProcPicDir(strFilePath);
		//		}
		//		else
		//		{
		int nPos = strFilePath.ReverseFind(TEXT('.'));
		CString strExt = strFilePath.Right(strFilePath.GetLength() - nPos - 1);
		if (strExt.CompareNoCase(TEXT("jpg")) == 0 ||
			strExt.CompareNoCase(TEXT("jpeg")) == 0 ||
			strExt.CompareNoCase(TEXT("bmp")) == 0)
		{
			//处理图片
			//...
			strPath.push_back(strFilePath);
		}
	}
	fileFinder.Close();
	return true;
}


void control::OnBnClickedButtontest()
{
	// TODO:  在此添加控件通知处理程序代码
	/*
	g.mc.Back();
	Sleep(1000);
	g.mc.Front();
	Sleep(1000);
	g.mc.Reset();
	*/
}

/*
void control::OnBnClickedButtonSavedelay()
{
	// TODO:  在此添加控件通知处理程序代码


}
*/

void control::OnBnClickedButtonDelaysv()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_delay < 2) m_delay = 2;
	g.ini.m_delay = m_delay;
	//m_delay = g.ini.m_delay;
	g.ini.SaveParaFile(PARA_PRJ);
	CString str;
	str.Format(_T("设置成功为 %d ms!"), g.ini.m_delay);
	AfxMessageBox(str);
	UpdateData(FALSE);
}
