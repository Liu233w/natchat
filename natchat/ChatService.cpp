#include "stdafx.h"
#include "ChatService.h"
#include "MessageHeader.h"
#include "ReceivingManager.h"

#include <string>
#include <winsock2.h>

using namespace inner_network;

void initNetworkAndThreads()
{
	//初始化WSA
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		printErrorAndExit(L"无法初始化 Socket");
	}

	// 初始化管理器
	l_pSendingManager = std::unique_ptr<SendingManager>(new SendingManager);
}

void SendMessageToIp(const char * message, const char * ip)
{
	std::string msg;
	msg.reserve(strlen(message) + 1); // 留出一位来存放消息类型
	msg[0] = inner_network::MSG_PRIVATE;
	msg += message;
	l_pSendingManager->send(ip, MESSAGE_RECV_PORT, msg.c_str(), msg.size());
}
