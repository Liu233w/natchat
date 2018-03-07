
// natchatDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "PrivateMessageDlg.h"
#include "RefreshingDlg.h"


// CnatchatDlg 对话框
class CnatchatDlg : public CDialogEx
{
// 构造
public:
	CnatchatDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NATCHAT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnLbnSelchangeList2();
	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl M_IPList;
	afx_msg void OnNMClickIplist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStnClickedReceivertext();
	afx_msg void OnNMDblclkIplist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRefresh();
};
