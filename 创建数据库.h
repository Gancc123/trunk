#pragma once


// 创建数据库 dialog

class 创建数据库 : public CDialogEx
{
	DECLARE_DYNAMIC(创建数据库)

public:
	创建数据库(CWnd* pParent = NULL);   // standard constructor
	virtual ~创建数据库();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
