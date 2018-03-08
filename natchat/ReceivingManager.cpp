#include "stdafx.h"
#include "ReceivingManager.h"
#include "MessageHeader.h"
#include "TicToc.h"

#include "Resource.h"

#include "ChatService.h"

namespace inner_network
{
	ReceivingManager::ReceivingManager(const int receivingPort)
	{
		//创建套接字
		SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (slisten == INVALID_SOCKET)
		{
			printErrorAndExit(L"无法创建接听Socket");
		}

		//绑定IP和端口
		sockaddr_in sin;
		sin.sin_family = AF_INET;
		sin.sin_port = htons(receivingPort);
		sin.sin_addr.S_un.S_addr = INADDR_ANY;
		if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
		{
			printErrorAndExit(L"无法绑定接听端口！");
		}

		//开始监听
		if (listen(slisten, 5) == SOCKET_ERROR)
		{
			printErrorAndExit(L"无法开始监听");
		}

		// 开始循环接收消息
		receivingThread = std::thread(&ReceivingManager::startReceivingLoop, this, slisten);
	}


	ReceivingManager::~ReceivingManager()
	{
	}

	void handleMessage(std::string msg, sockaddr_in addr)
	{
		// 获取ip
		char ip_buf[20]; // 使用 inet_ntop 而不是 inet_ntoa 来保证安全
		inet_ntop(AF_INET, &addr.sin_addr, ip_buf, sizeof(ip_buf));

		if (msg[0] == MSG_REGULAR)
		{
			History history;
			history.isPrivate = false;
			history.message = msg.substr(1);
			history.message = getUserNameFromIp(ip_buf);
			history.time = std::chrono::system_clock::now();
			std::unique_lock<std::mutex> lk(g_HistoryMutex);
			g_Histories.push_back(history);
		}
		else if (msg[0] == MSG_TIC)
		{
			handleTic(std::move(msg), ip_buf);
		}
	}

	void ReceivingManager::startReceivingLoop(SOCKET sListen)
	{
		std::string res;

		//循环接收数据
		SOCKET sClient;
		sockaddr_in remoteAddr;
		int nAddrlen = sizeof(remoteAddr);
		char buffer[255];
		while (true)
		{
			// 等待连接
			sClient = accept(sListen, (SOCKADDR *)&remoteAddr, &nAddrlen);
			if (sClient == INVALID_SOCKET)
			{
				PostMessage(AfxGetMainWnd()->m_hWnd, IDC_RECEIVE_MESSAGE_ERROR, 0, (LPARAM)"无法创建接收连接");
			}

			//接收数据
			while (true)
			{
				int ret = recv(sClient, buffer, 255, 0);
				if (ret > 0)
				{
					buffer[ret] = '\0';
					res.append(buffer);
					if (res[res.size() - 1] == '\f')
					{
						// 已经读到结尾
						res.pop_back();
						break;
					}
				}
			}

			handleMessage(std::move(res), remoteAddr);
		}
	}

	void printErrorAndExit(const wchar_t* errMsg)
	{
		MessageBox(AfxGetMainWnd()->m_hWnd, errMsg, L"初始化错误", MB_OK);
		exit(0);
	}

}