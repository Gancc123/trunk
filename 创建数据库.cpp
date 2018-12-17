// 创建数据库.cpp : implementation file
//

#include "stdafx.h"
#include "HustBase.h"
#include "创建数据库.h"
#include "afxdialogex.h"


// 创建数据库 dialog

IMPLEMENT_DYNAMIC(创建数据库, CDialogEx)

创建数据库::创建数据库(CWnd* pParent /*=NULL*/)
	: CDialogEx(创建数据库::IDD, pParent)
{

}

创建数据库::~创建数据库()
{
}

void 创建数据库::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(创建数据库, CDialogEx)
END_MESSAGE_MAP()


// 创建数据库 message handlers
