// test.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageVision.h"
#include "test.h"
#include "afxdialogex.h"


// test 对话框

IMPLEMENT_DYNAMIC(test, CDialogEx)

test::test(CWnd* pParent /*=NULL*/)
	: CDialogEx(test::IDD, pParent)
	, m_val1(0)
	, m_val2(0)
	, m_val3(0)
{

}

test::~test()
{
}

void test::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_VAL1, m_val1);
	DDX_Text(pDX, IDC_EDIT_VAL2, m_val2);
	DDX_Text(pDX, IDC_EDIT_VAL3, m_val3);
}


BEGIN_MESSAGE_MAP(test, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_COFIRM, &test::OnBnClickedButtonCofirm)
END_MESSAGE_MAP()


// test 消息处理程序


void test::OnBnClickedButtonCofirm()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	g.mytest.val1 = m_val1;
	g.mytest.val2 = m_val2;
	g.mytest.val3 = m_val3;
}

