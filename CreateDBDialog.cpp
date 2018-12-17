// CreateDBDialog.cpp : implementation file
//

#include "stdafx.h"
#include "HustBase.h"
#include "CreateDBDialog.h"
#include "afxdialogex.h"
#include <direct.h>    //头文件  
#include <string>
#include "SYS_Manager.h"

CString strss;

// CreateDBDialog dialog

IMPLEMENT_DYNAMIC(CreateDBDialog, CDialogEx)

CreateDBDialog::CreateDBDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CreateDBDialog::IDD, pParent)
{

}

CreateDBDialog::~CreateDBDialog()
{
}

void CreateDBDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CreateDBDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CreateDBDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CreateDBDialog::OnBnClickedButton1)
END_MESSAGE_MAP()


// CreateDBDialog message handlers


void CreateDBDialog::OnBnClickedOk()
{
	
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();

	//获取文件路径和数据库名
	CString cstrDbPath;
	CString cstrDbName;
	//获取文件路径
	cstrDbPath = strss;//形如"D:\DBMS"
	//获取数据库名
	GetDlgItemText(IDC_EDIT2,cstrDbName); //取输入框的值，并赋给cstrDbName;

	//CString 转 String
	std::string strDbPath = CT2A(cstrDbPath.GetBuffer());//形如"D:\DBMS"
	std::string strDbName = CT2A(cstrDbName.GetBuffer());//形如"student"

	//创建文件夹
	//string dirName = "D:\\test";    
	//_mkdir(dirName.c_str());  
	std::string str1 = strDbPath + "\\" + strDbName;//形如"D:\DBMS\student"
	_mkdir(str1.c_str());
	
	//string 转 char *
	char * c1, * c2;
	const int len1 = str1.length();
	c1 = new char[len1 + 1];
	strcpy(c1, str1.c_str());
	const int len2 = strDbName.length();
	c2 = new char[len2 + 1];
	strcpy(c2, strDbName.c_str());

	//调用创建数据库函数
	CreateDB(c1, c2);//参数形如"D:\DBMS\student","student"

	AfxMessageBox("创建成功");
}


void CreateDBDialog::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	char szPath[MAX_PATH];     //存放选择的目录路径 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));   

	BROWSEINFO bi;   
	bi.hwndOwner = NULL;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = szPath;   
	bi.lpszTitle = "请选择需要打包的目录：";   
	bi.ulFlags = 0;   
	bi.lpfn = NULL;   
	bi.lParam = 0;   
	bi.iImage = 0;   
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

	if(lp && SHGetPathFromIDList(lp, szPath))   
	{
		strss.Format("%s", szPath);
		str.Format("选择的目录为 %s",  szPath);
		AfxMessageBox(str); 
		SetDlgItemText( IDC_EDIT3, TEXT(strss) );

	}
	else   
		AfxMessageBox("无效的目录，请重新选择");
}
