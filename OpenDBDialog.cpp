// OpenDBDialog.cpp : implementation file
//

#include "stdafx.h"
#include "HustBase.h"
#include "OpenDBDialog.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "HustBaseDoc.h"
#include "HustBaseView.h"
#include "TreeList.h"
#include "SYS_Manager.h"
#include <string>


CString strss2;
// COpenDBDialog dialog

IMPLEMENT_DYNAMIC(COpenDBDialog, CDialogEx)

COpenDBDialog::COpenDBDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(COpenDBDialog::IDD, pParent)
{

}

COpenDBDialog::~COpenDBDialog()
{
}

void COpenDBDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COpenDBDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &COpenDBDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &COpenDBDialog::OnBnClickedButton1)
END_MESSAGE_MAP()


// COpenDBDialog message handlers


void COpenDBDialog::OnBnClickedOk()//确认选择，打开数据库
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
	std::string str = CT2A(strss2.GetBuffer());//数据库路径由CString转换为String
	OpenDB(str.c_str());//数据库路径由String转换为const char *并作为参数传递到OpenDB函数

	//获取当前工作路径
	char buf[1000];
	GetCurrentDirectory(1000,buf);  //使用windows api得到当前工作路径
	CString s;
	s.Format("当前工作路径设置为： %s", buf);
	AfxMessageBox(s);

	//有问题
	CHustBaseDoc *pDoc;
	pDoc=CHustBaseDoc::GetDoc();
	CHustBaseApp::pathvalue=true;
	pDoc->m_pTreeView->PopulateTree();
}


void COpenDBDialog::OnBnClickedButton1()//浏览数据库地址
{
	// TODO: Add your control notification handler code here
	char szPath[MAX_PATH];     //存放选择的目录路径 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));   

	//选择目录
	BROWSEINFO bi;   
	bi.hwndOwner = NULL;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = szPath;   
	bi.lpszTitle = "请选择目录：";   
	bi.ulFlags = 0;   
	bi.lpfn = NULL;   
	bi.lParam = 0;   
	bi.iImage = 0;   
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

	if(lp && SHGetPathFromIDList(lp, szPath))   
	{
		strss2.Format("%s", szPath);
		str.Format("选择的目录为 %s",  szPath);
		AfxMessageBox(str); 
		SetDlgItemText( IDC_EDIT1, TEXT(strss2) );

	}
	else   
		AfxMessageBox("无效的目录，请重新选择");
}
