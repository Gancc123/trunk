// DropDBDialog.cpp : implementation file
//

#include "stdafx.h"
#include "HustBase.h"
#include "DropDBDialog.h"
#include "afxdialogex.h"
#include <windows.h>
#include <string>
#include "SYS_Manager.h"


CString strDirectory;
// DropDBDialog dialog

IMPLEMENT_DYNAMIC(DropDBDialog, CDialogEx)

DropDBDialog::DropDBDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(DropDBDialog::IDD, pParent)
{

}

DropDBDialog::~DropDBDialog()
{
}

void DropDBDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DropDBDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &DropDBDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &DropDBDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// DropDBDialog message handlers


void DropDBDialog::OnBnClickedButton1()//浏览数据库以删除
{
	// TODO: Add your control notification handler code here
	char szPath[MAX_PATH];     //存放选择的目录路径 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));   

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
		strDirectory.Format("%s", szPath);
		str.Format("选择的目录为 %s",  szPath);
		AfxMessageBox(str); 
		SetDlgItemText( IDC_EDIT1, TEXT(strDirectory) );

	}
	else   
		AfxMessageBox("无效的目录，请重新选择");
	
}


void DropDBDialog::OnBnClickedOk()//确认删除选中的数据库
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
	//CString 转 String
	std::string str = CT2A(strDirectory.GetBuffer());//形如"D:\DBMS\student"
	//String 转 const char *并调用DropDB(const char *dbpath)删除文件夹
	DropDB(str.c_str());
	AfxMessageBox("删除成功");
	
}

