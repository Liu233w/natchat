// PrivateMessageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "natchat.h"
#include "PrivateMessageDlg.h"
#include "afxdialogex.h"


// CPrivateMessageDlg 对话框

IMPLEMENT_DYNAMIC(CPrivateMessageDlg, CDialogEx)

CPrivateMessageDlg::CPrivateMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SENDBOX, pParent) {

}

CPrivateMessageDlg::CPrivateMessageDlg(const CString& title, const CString& IP_adr, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SENDBOX, pParent)
{
	mytitle = title;
	this->IP_adr = IP_adr;
}

CPrivateMessageDlg::~CPrivateMessageDlg()
{
}

BOOL CPrivateMessageDlg::OnInitDialog()
{
	this->SetWindowTextW(L"Private Message");
	this->SetDlgItemTextW(IDC_PRIVATETEXT, mytitle);
	return TRUE;
}


void CPrivateMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPrivateMessageDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CANCEL, &CPrivateMessageDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_SENDPRIVATE, &CPrivateMessageDlg::OnBnClickedSendprivate)
END_MESSAGE_MAP()


// CPrivateMessageDlg 消息处理程序


void CPrivateMessageDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(0);
}


void CPrivateMessageDlg::OnBnClickedSendprivate()
{
	// TODO: 在此添加控件通知处理程序代码
	CString message;
	GetDlgItemText(IDC_PRIVATEEDIT, message);
	if (message == L"") return;
	std::string msg, ip;
	cstring2string(message, msg);
	cstring2string(IP_adr, ip);
	SendMessageToIp(msg.c_str(), ip.c_str());
	EndDialog(0);
}

BOOL CPrivateMessageDlg::PreTranslateMessage(MSG* pMsg)
{
	if (GetDlgItem(IDC_PRIVATEEDIT) == GetFocus()) {
		do
		{
			if (pMsg->message != WM_KEYDOWN)
			{
				break;
			}
			const SHORT l_cnKeyState = 0x8000;
			if (l_cnKeyState != (GetKeyState(VK_CONTROL) & l_cnKeyState))
			{
				break;
			}
			if (pMsg->wParam == VK_RETURN)
			{
				CString message;
				GetDlgItemText(IDC_PRIVATEEDIT, message);
				if (message == L"") break;
				std::string msg, ip;
				cstring2string(message, msg);
				cstring2string(IP_adr, ip);
				SendMessageToIp(msg.c_str(), ip.c_str());
				EndDialog(0);
				return(TRUE);
			}
		} while (FALSE);
		return   CDialog::PreTranslateMessage(pMsg);
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;
	}


	return   CDialog::PreTranslateMessage(pMsg);
}