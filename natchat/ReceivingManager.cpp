#include "stdafx.h"
#include "ReceivingManager.h"
#include "MessageHeader.h"
#include "TicToc.h"
#include "File.h"

#include "Resource.h"

#include "ChatService.h"

namespace inner_network
{
	void ReceivingManager::initReceivingManager(const int receivingPort)
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
		ReceivingManager::receivingThread = std::thread(ReceivingManager::startReceivingLoop, slisten);
		receivingThread.detach();
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
			history.senderName = getUserNameFromIp(ip_buf);
			history.time = std::chrono::system_clock::now();
			{
				std::lock_guard<std::mutex> lk(g_HistoryMutex);
				g_Histories.push_back(history);
			}
			PostMessage(g_hHWnd, WM_RECOMMEND_REFRESH_HISTORIES, NULL, NULL);
		}
		else if (msg[0] == MSG_TOC)
		{
			handleToc(msg, ip_buf);
		}
		else if (msg[0] == MSG_PRIVATE)
		{
			History history;
			history.isPrivate = true;
			history.message = msg.substr(1);
			history.senderName = getUserNameFromIp(ip_buf);
			history.time = std::chrono::system_clock::now();
			{
				std::lock_guard<std::mutex> lk(g_HistoryMutex);
				g_Histories.push_back(history);
			}
			PostMessage(g_hHWnd, WM_RECOMMEND_REFRESH_HISTORIES, NULL, NULL);
		}
		else if (msg[0] == MSG_FILE)
		{
			// 会阻塞接收线程直到保存完文件
			handleFileMessage(msg);
		}
	}

	void ReceivingManager::startReceivingLoop(SOCKET sListen)
	{
		WSADATA wsdata;
		//启动SOCKET库，版本为2.0  
		WSAStartup(0x0202, &wsdata);

		std::string res;

		//循环接收数据
		SOCKET sClient;
		sockaddr_in remoteAddr;
		int nAddrlen = sizeof(remoteAddr);
		char buffer[256];
		while (true)
		{
			// 等待连接
			sClient = accept(sListen, (SOCKADDR *)&remoteAddr, &nAddrlen);
			if (sClient == INVALID_SOCKET)
			{
				PostMessage(g_hHWnd, WM_RECEIVE_MESSAGE_ERROR, 0, (LPARAM)"无法创建接收连接");
			}

			//接收数据
			while (true)
			{
				int ret = recv(sClient, buffer, 255, 0);
				if (ret > 0)
				{
					res.append(buffer, buffer + ret);
					static const size_t MSG_END_SIZE = strlen(MESSAGE_END);
					if (res.size() > MSG_END_SIZE 
						&& res.substr(res.size() - MSG_END_SIZE, MSG_END_SIZE) == MESSAGE_END)
					{
						// 已经读到结尾
						// res.pop_back();
						res.erase(res.size() - MSG_END_SIZE);
						break;
					}
				}
			}

			handleMessage(std::move(res), remoteAddr);

			closesocket(sClient);
		}
	}

}