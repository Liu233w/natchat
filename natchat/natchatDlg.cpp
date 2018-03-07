
// natchatDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "natchat.h"
#include "natchatDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CnatchatDlg 对话框



CnatchatDlg::CnatchatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NATCHAT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CnatchatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPLIST, M_IPList);
}

BEGIN_MESSAGE_MAP(CnatchatDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_CLICK, IDC_IPLIST, &CnatchatDlg::OnNMClickIplist)
	ON_NOTIFY(NM_DBLCLK, IDC_IPLIST, &CnatchatDlg::OnNMDblclkIplist)
	ON_BN_CLICKED(IDC_REFRESH, &CnatchatDlg::OnBnClickedRefresh)
END_MESSAGE_MAP()


// CnatchatDlg 消息处理程序

BOOL CnatchatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码
	CRect rect;

	// 获取编程语言列表视图控件的位置和大小   
	M_IPList.GetClientRect(&rect);

	// 为列表视图控件添加全行选中和栅格风格   
	M_IPList.SetExtendedStyle(M_IPList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 为列表视图控件添加三列   
	M_IPList.InsertColumn(0, _T("HOST"), LVCFMT_CENTER, rect.Width() / 2, 0);
	M_IPList.InsertColumn(1, _T("IP"), LVCFMT_CENTER, rect.Width() / 2, 1);

	// 在列表视图控件中插入列表项，并设置列表子项文本   
	M_IPList.InsertItem(0, _T("Java"));
	M_IPList.SetItemText(0, 1, _T("192.168.1.6"));
	M_IPList.InsertItem(1, _T("C"));
	M_IPList.SetItemText(1, 1, _T("192.168.1.7"));
	M_IPList.InsertItem(2, _T("C#"));
	M_IPList.SetItemText(2, 1, _T("192.168.1.8"));
	M_IPList.InsertItem(3, _T("C++"));
	M_IPList.SetItemText(3, 1, _T("192.168.1.4"));
	M_IPList.InsertItem(3, _T("C++"));
	M_IPList.SetItemText(3, 1, _T("192.168.1.4"));
	M_IPList.InsertItem(4, _T("C++"));
	M_IPList.SetItemText(4, 1, _T("192.168.1.4"));
	M_IPList.InsertItem(5, _T("C++"));
	M_IPList.SetItemText(5, 1, _T("192.168.1.4"));
	M_IPList.InsertItem(6, _T("C++"));
	M_IPList.SetItemText(6, 1, _T("192.168.1.4"));
	M_IPList.InsertItem(7, _T("C++"));
	M_IPList.SetItemText(7, 1, _T("192.168.1.4"));
	M_IPList.InsertItem(8, _T("C++"));
	M_IPList.SetItemText(8, 1, _T("192.168.1.4"));
	M_IPList.InsertItem(9, _T("C++"));
	M_IPList.SetItemText(9, 1, _T("192.168.1.4"));
	M_IPList.InsertItem(10, _T("C++"));
	M_IPList.SetItemText(10, 1, _T("192.168.1.4"));
	M_IPList.InsertItem(11, _T("C++"));
	M_IPList.SetItemText(11, 1, _T("192.168.1.4"));
	M_IPList.InsertItem(12, _T("C++"));
	M_IPList.SetItemText(12, 1, _T("192.168.1.4"));
	M_IPList.InsertItem(13, _T("C++"));
	M_IPList.SetItemText(13, 1, _T("192.168.1.4"));
	M_IPList.InsertItem(14, _T("C++"));
	M_IPList.SetItemText(14, 1, _T("192.168.1.4"));
	M_IPList.InsertItem(15, _T("C++"));
	M_IPList.SetItemText(15, 1, _T("192.168.1.4"));
	M_IPList.InsertItem(16, _T("C++"));
	M_IPList.SetItemText(16, 1, _T("192.168.1.4"));
	M_IPList.InsertItem(17, _T("C++"));
	M_IPList.SetItemText(17, 1, _T("192.168.1.4"));
	M_IPList.InsertItem(18, _T("C++"));
	M_IPList.SetItemText(18, 1, _T("192.168.1.4"));
	M_IPList.InsertItem(19, _T("C++"));
	M_IPList.SetItemText(19, 1, _T("192.168.1.4"));
	M_IPList.InsertItem(20, _T("C++"));
	M_IPList.SetItemText(20, 1, _T("192.168.1.4"));


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CnatchatDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CnatchatDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CnatchatDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CnatchatDlg::OnNMClickIplist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	CString hostName;    // 选择host的名称字符串   
	CString IPName;    // 选择IP的名称字符串   
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;

	if (-1 != pNMListView->iItem)        // 如果iItem不是-1，就说明有列表项被选择   
	{
		// 获取被选择列表项第一个子项的文本   
		hostName = M_IPList.GetItemText(pNMListView->iItem, 0);
		IPName = M_IPList.GetItemText(pNMListView->iItem, 1);
		// 显示
		SetDlgItemText(IDC_RECRIVERHOST, hostName);
		SetDlgItemText(IDC_RECRIVERIP, IPName);
	}
	else {
		SetDlgItemText(IDC_RECRIVERHOST, L"");
		SetDlgItemText(IDC_RECRIVERIP, L"");
	}
	*pResult = 0;
}



void CnatchatDlg::OnNMDblclkIplist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;
	CString hostName;
	CString head = L"Private Message To ";

	if (-1 != pNMListView->iItem) {
		hostName = M_IPList.GetItemText(pNMListView->iItem, 0);
		CPrivateMessageDlg pm(head + hostName + L":");
		pm.DoModal();
	}
	*pResult = 0;
}


void CnatchatDlg::OnBnClickedRefresh()
{
	// TODO: 在此添加控件通知处理程序代码
	CRefreshingDlg rfd;
	rfd.DoModal();
}
