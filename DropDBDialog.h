#pragma once


// DropDBDialog dialog

class DropDBDialog : public CDialogEx
{
	DECLARE_DYNAMIC(DropDBDialog)

public:
	DropDBDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~DropDBDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
};
