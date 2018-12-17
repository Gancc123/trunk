#pragma once


// CreateDBDialog dialog

class CreateDBDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CreateDBDialog)

public:
	CreateDBDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CreateDBDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
};
