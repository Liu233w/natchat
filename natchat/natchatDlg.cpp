
// natchatDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "natchat.h"
#include "natchatDlg.h"
#include "afxdialogex.h"
#include <iomanip>
#include <locale>
#include <codecvt>
#include "ChatService.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 声明全局变量
HWND g_hHWnd;
std::list<History> g_Histories;
std::mutex g_HistoryMutex;

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
	DDX_Control(pDX, IDC_SENDANI, send_animation);
	DDX_Control(pDX, IDC_EMOTIONANI, emotion_animation);
	DDX_Control(pDX, IDC_EDIT1, show_history);
}

BEGIN_MESSAGE_MAP(CnatchatDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_CLICK, IDC_IPLIST, &CnatchatDlg::OnNMClickIplist)
	ON_NOTIFY(NM_DBLCLK, IDC_IPLIST, &CnatchatDlg::OnNMDblclkIplist)
	ON_BN_CLICKED(IDC_REFRESH, &CnatchatDlg::OnBnClickedRefresh)
	ON_WM_MOUSEMOVE()
//	ON_WM_SETCURSOR()
ON_BN_CLICKED(IDC_CHOOSEFILE, &CnatchatDlg::OnBnClickedChoosefile)
//ON_STN_CLICKED(IDC_SENDANI, &CnatchatDlg::OnStnClickedSendani)
//ON_STN_CLICKED(IDC_EMOTIONANI, &CnatchatDlg::OnStnClickedEmotionani)
//ON_WM_LBUTTONDBLCLK()
ON_WM_LBUTTONDOWN()
ON_MESSAGE(WM_RECOMMEND_REFRESH_HISTORIES, OnRecommendRefreshHistories)
ON_MESSAGE(WM_RECEIVE_TOC, &CnatchatDlg::OnReceiveToc)
ON_MESSAGE(WM_RECEIVE_TIC, &CnatchatDlg::OnReceiveTic)
ON_WM_DESTROY()
ON_BN_CLICKED(IDC_SENDFILE, &CnatchatDlg::OnBnClickedSendfile)
ON_MESSAGE(WM_RECEIVE_FILE, &CnatchatDlg::OnReceiveFile)
ON_MESSAGE(WM_SEND_FILE_DONE, &CnatchatDlg::OnSendFileDone)
ON_MESSAGE(WM_SEND_FILE_ERROR, &CnatchatDlg::OnSendFileError)
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
	this->SetWindowText(L"natchat");
	//ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码
	CDialogEx::SetBackgroundColor(RGB(255, 255, 255));
	CRect rect;

	// 获取编程语言列表视图控件的位置和大小   
	M_IPList.GetClientRect(&rect);

	// 为列表视图控件添加全行选中和栅格风格   
	M_IPList.SetExtendedStyle(M_IPList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 为列表视图控件添加三列   
	M_IPList.InsertColumn(0, _T("HOSTNAME"), LVCFMT_CENTER, rect.Width() / 2, 0);
	M_IPList.InsertColumn(1, _T("IP"), LVCFMT_CENTER, rect.Width() / 2, 1);

	// 在列表视图控件中插入列表项，并设置列表子项文本   
	M_IPList.InsertItem(0, _T("DESKTOP"));
	M_IPList.SetItemText(0, 1, _T("192.168.1.4"));
	M_IPList.InsertItem(1, _T("LAPTOP"));
	M_IPList.SetItemText(1, 1, _T("192.168.1.6"));

	CRect send_rect;
	GetDlgItem(IDC_SENDANI)->GetWindowRect(&send_rect);
	ScreenToClient(&send_rect);
	//send_animation.Create(NULL, WS_CHILD | WS_VISIBLE | SS_ENHMETAFILE, CRect(0, 0, 100, 100), this, 1234);
	if (send_animation.Load(MAKEINTRESOURCE(IDR_SEND), _T("Gif"))) {
		//refreshing_img.SetBkColor(RGB(255, 255, 255));
		send_animation.SetPaintRect(&send_rect);
		send_animation.MoveWindow(&send_rect, TRUE);
		send_animation.Draw();
		Sleep(10);
		send_animation.Stop();
	}

	CRect emotion_rect;
	GetDlgItem(IDC_EMOTIONANI)->GetWindowRect(&emotion_rect);
	ScreenToClient(&emotion_rect);
	//send_animation.Create(NULL, WS_CHILD | WS_VISIBLE | SS_ENHMETAFILE, CRect(0, 0, 100, 100), this, 1234);
	if (emotion_animation.Load(MAKEINTRESOURCE(IDR_EMOTION), _T("Gif"))) {
		//refreshing_img.SetBkColor(RGB(255, 255, 255));
		emotion_animation.SetPaintRect(&emotion_rect);
		emotion_animation.MoveWindow(&emotion_rect, TRUE);
		emotion_animation.Draw();
		Sleep(10);
		emotion_animation.Stop();
	}

	// 存储主窗口句柄
	g_hHWnd = this->m_hWnd;

	initNetworkAndThreads();

	//refreshUserList();

	//cursor_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)(cursor_thread), NULL, 0, &myownthread);

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
	CString hostName, IP_adr;
	CString head = L"Private Message To ";

	if (-1 != pNMListView->iItem) {
		hostName = M_IPList.GetItemText(pNMListView->iItem, 0);
		IP_adr = M_IPList.GetItemText(pNMListView->iItem, 1);
		CPrivateMessageDlg pm(head + hostName + L":", IP_adr);
		pm.DoModal();
	}
	*pResult = 0;
}


void CnatchatDlg::OnBnClickedRefresh()
{
	// TODO: 在此添加控件通知处理程序代码
	CRefreshingDlg rfd;
	rfd.DoModal();
	//refreshUserList();
}


void CnatchatDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	GetDlgItem(IDC_SENDANI)->GetWindowRect(&send_rect);
	//ScreenToClient(&send_rect);
	GetDlgItem(IDC_EMOTIONANI)->GetWindowRect(&emotion_rect);
	//ScreenToClient(&emotion_rect);
	POINT cur_point;
	GetCursorPos(&cur_point);
	//ScreenToClient(&cur_point);

	bool do_second_judge = true;

	if (send_rect.PtInRect(cur_point))
	{
		if (!send_started_draw) {
			send_animation.Draw();
			send_started_draw = true;
		}
		//if (!is_cursor_hand) {
			::SetCursor(LoadCursor(NULL, IDC_HAND));
			is_cursor_hand = true;
		//}
		do_second_judge = false;
	}
	else {
		if (send_started_draw) {
			send_animation.Stop();
			send_started_draw = false;
		}
		if (is_cursor_hand) {
			::SetCursor(LoadCursor(NULL, IDC_ARROW));
			is_cursor_hand = false;
		}
	}


	if (do_second_judge) {
		if (emotion_rect.PtInRect(cur_point))
		{
			if (!emotion_started_draw) {
				emotion_animation.Draw();
				emotion_started_draw = true;
			}
			if (!is_cursor_hand) {
				::SetCursor(LoadCursor(NULL, IDC_HAND));
				is_cursor_hand = true;
			}
		}
		else {
			if (emotion_started_draw) {
				emotion_animation.Stop();
				emotion_started_draw = false;
			}
			if (is_cursor_hand) {
				::SetCursor(LoadCursor(NULL, IDC_ARROW));
				is_cursor_hand = false;
			}

		}
	}
	
	CDialogEx::OnMouseMove(nFlags, point);
}




void CnatchatDlg::OnBnClickedChoosefile()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFile = _T("");

	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("All Files (*.*)|*.*||"), NULL);

	if (dlgFile.DoModal())
	{
		strFile = dlgFile.GetPathName();
		SetDlgItemText(IDC_SELECTEDFILE, strFile);
	}
}



void CnatchatDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	GetDlgItem(IDC_SENDANI)->GetWindowRect(&send_rect);
	ScreenToClient(&send_rect);
	GetDlgItem(IDC_EMOTIONANI)->GetWindowRect(&emotion_rect);
	ScreenToClient(&emotion_rect);
	//POINT cur_point;
	//GetCursorPos(&cur_point);

	CString cur_edit_text;
	GetDlgItemText(IDC_EDIT2, cur_edit_text);
	std::string cur_edit_text_s;
	cstring2string(cur_edit_text, cur_edit_text_s);

	if (send_rect.PtInRect(point)) {
		std::vector<std::string> ip_list;
		CString ip_cs;
		int lineCount = M_IPList.GetItemCount();
		for (int i = 0;i < lineCount; i++) {
			ip_cs = M_IPList.GetItemText(i, 1);
			std::string ip_s;
			cstring2string(ip_cs, ip_s);
			ip_list.push_back(ip_s);
		}
		BroadcastMessageToIps(cur_edit_text_s.c_str(), ip_list);
		SetDlgItemText(IDC_EDIT2, L"");
	}
	else if (emotion_rect.PtInRect(point)) {
		MessageBox(L"emoji");
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


LRESULT CnatchatDlg::OnRecommendRefreshHistories(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加命令处理程序代码
	std::lock_guard<std::mutex> lk(g_HistoryMutex);
	CString show_txt;
	std::list<History>::iterator h_itor;
	h_itor = g_Histories.begin();
	while (h_itor != g_Histories.end())
	{
		show_txt += (h_itor->senderName).c_str();
		show_txt += " ";

		if (h_itor->isPrivate) {
			show_txt += " Send you a private message";
		}

		std::time_t msg_time = std::chrono::system_clock::to_time_t(h_itor->time);
		std::ctime(&msg_time);
		show_txt += " at ";
		
		show_txt += std::ctime(&msg_time);
		show_txt += " : \r\n";

		show_txt += std::wstring_convert<std::codecvt_utf8<wchar_t> >().from_bytes((h_itor->message).c_str()).c_str();;
		show_txt += "\r\n";
		++h_itor;
	}
	SetDlgItemText(IDC_EDIT1, show_txt);
	show_history.LineScroll(show_history.GetLineCount());
	return 0;
}


afx_msg LRESULT CnatchatDlg::OnReceiveToc(WPARAM wParam, LPARAM lParam)
{
	M_IPList.DeleteAllItems();
	std::vector<std::pair<std::string, std::string> > user_list = getUsers();
	std::vector<std::pair<std::string, std::string> >::iterator u_itor = user_list.begin();
	while (u_itor != user_list.end()) {
		int cur_row = M_IPList.GetItemCount();
		std::string ip = u_itor->first;
		std::string hostname = u_itor->second;
		M_IPList.InsertItem(cur_row, CString((u_itor->second).c_str()));
		M_IPList.SetItemText(cur_row, 1, CString((u_itor->first).c_str()));
		++u_itor;
	}
	return 0;
}


afx_msg LRESULT CnatchatDlg::OnReceiveTic(WPARAM wParam, LPARAM lParam)
{
	M_IPList.DeleteAllItems();
	std::vector<std::pair<std::string, std::string> > user_list = getUsers();
	std::vector<std::pair<std::string, std::string> >::iterator u_itor = user_list.begin();
	while (u_itor != user_list.end()) {
		int cur_row = M_IPList.GetItemCount();
		std::string ip = u_itor->first;
		std::string hostname = u_itor->second;
		M_IPList.InsertItem(cur_row, CString((u_itor->second).c_str()));
		M_IPList.SetItemText(cur_row, 1, CString((u_itor->first).c_str()));
		++u_itor;
	}
	return 0;
}


void CnatchatDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	exit(0);
	// TODO: 在此处添加消息处理程序代码
}


void CnatchatDlg::OnBnClickedSendfile()
{
	// TODO: 在此添加控件通知处理程序代码
	CString rsv_ip, file_name, status;
	GetDlgItemText(IDC_RECRIVERIP, rsv_ip);
	GetDlgItemText(IDC_SELECTEDFILE, file_name);
	if (rsv_ip == L"") {
		MessageBox(L"Please select receiver!");
		return;
	}
	else if (file_name == L"") {
		MessageBox(L"Please select a file!");
		return;
	}
	sendFileToIp(file_name, rsv_ip);
	SetDlgItemText(IDC_STATUS, L"Sending file");
	GetDlgItem(IDC_SENDFILE)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHOOSEFILE)->EnableWindow(FALSE);
	SetDlgItemText(IDC_RECRIVERIP, L"");
	SetDlgItemText(IDC_RECRIVERHOST, L"");
	SetDlgItemText(IDC_SELECTEDFILE, L"");
}


afx_msg LRESULT CnatchatDlg::OnReceiveFile(WPARAM wParam, LPARAM lParam)
{
	if (IDYES == AfxMessageBox(_T("您收到一个文件，是否接收？"), MB_YESNO)) {
		CFileDialog dlg(FALSE, _T("."), (wchar_t *)lParam);
		if (dlg.DoModal() == IDOK)
		{
			CString strFile = dlg.GetPathName();//获取完整路径
			g_wsSaveFilePath = strFile.GetString();
		}
	}
	
	return 0;
}


afx_msg LRESULT CnatchatDlg::OnSendFileDone(WPARAM wParam, LPARAM lParam)
{
	SetDlgItemText(IDC_STATUS, L"Finish sending file");
	GetDlgItem(IDC_SENDFILE)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHOOSEFILE)->EnableWindow(TRUE);
	return 0;
}


afx_msg LRESULT CnatchatDlg::OnSendFileError(WPARAM wParam, LPARAM lParam)
{
	SetDlgItemText(IDC_STATUS, L"Fail to send file");
	GetDlgItem(IDC_SENDFILE)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHOOSEFILE)->EnableWindow(TRUE);
	return 0;
}
