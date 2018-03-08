#pragma once

#include "ChatService.h"

// CPrivateMessageDlg 对话框

class CPrivateMessageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPrivateMessageDlg)

public:
	CPrivateMessageDlg(CWnd* pParent = NULL);   // 标准构造函数
	CPrivateMessageDlg(const CString& title, const CString& IP_adr,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPrivateMessageDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SENDBOX };
#endif

private:
	CString mytitle;
	CString IP_adr;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedSendprivate();
};
