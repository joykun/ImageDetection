#pragma once



// control 窗体视图

class control : public CFormView
{
	DECLARE_DYNCREATE(control)

protected:
	control();           // 动态创建所使用的受保护的构造函数
	virtual ~control();

public:
	enum { IDD = IDD_DIALOG_CONTROL };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonRoi();
	afx_msg void OnBnClickedButtonOnlysvm();
	afx_msg LRESULT UpdateUI(WPARAM wParam, LPARAM lParam);
	int m_coord_x;
	int m_coord_y;
	int m_coord_width;
	int m_existmin;
	int m_existmax;
	int m_dialateDis;
	int m_wmin;
	int m_wmax;
	int m_hmin;
	int m_hmax;
	float m_ratio;
	int m_coord_height;
	double m_shutter;
	afx_msg void OnBnClickedButtonTopImg();
	afx_msg void OnBnClickedButtonBottomImg();
	afx_msg void OnBnClickedButtonOpendir();
	//afx_msg void OnBnClickedButtonBottomImg2();
	afx_msg void OnBnClickedButtonDefaulttrain();
	bool GetDir(CString strPicDir, vector<CString>& strPath);
	afx_msg void OnBnClickedButtontest();
	int m_delay;
	//afx_msg void OnBnClickedButtonSavedelay();
	afx_msg void OnBnClickedButtonDelaysv();
};


