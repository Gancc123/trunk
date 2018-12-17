#pragma once


// COpenDBDialog dialog

class COpenDBDialog : public CDialogEx
{
	DECLARE_DYNAMIC(COpenDBDialog)

public:
	COpenDBDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~COpenDBDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
};
