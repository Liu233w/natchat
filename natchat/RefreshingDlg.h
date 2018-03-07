#pragma once
#include "afxwin.h"
#include "PictureEx.h"


// CRefreshingDlg 对话框

class CRefreshingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRefreshingDlg)

public:
	CRefreshingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRefreshingDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REFRESH };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CPictureEx refreshing_img;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
