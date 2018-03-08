#include "stdafx.h"

#include <cstring>

#include "TicToc.h"
#include "ChatService.h"
#include "Resource.h"
#include "MessageHeader.h"

// 声明全局变量
std::map<std::string, std::string> l_AllUser;
std::mutex l_AllUserMutex;

namespace inner_network
{
	void handleTic(const char *msg, const char* ip)
	{
		{
			std::lock_guard<std::mutex> lk(l_AllUserMutex);
			l_AllUser[ip] = msg + 1;
		}

		sendTocTo(ip);
		// 保证内存安全，这里必须用 send
		SendMessage(g_hHWnd, WM_RECEIVE_TIC, NULL, (LPARAM)ip);
	}

	void handleToc(std::string msg, const char * ip)
	{
		{
			std::lock_guard<std::mutex> lk(l_AllUserMutex);
			l_AllUser[ip] = msg.substr(1);
		}

		// 保证内存安全，这里必须用 send
		SendMessage(g_hHWnd, WM_RECEIVE_TOC, NULL, (LPARAM)ip);
	}

	void startTicLoop(const int port)
	{
		static constexpr int BUFLEN = 512;

		SOCKET s;
		struct sockaddr_in server, si_other;
		int slen, recv_len;
		char buf[BUFLEN];

		slen = sizeof(si_other);

		//Create a socket
		if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
		{
			printErrorAndExit(L"无法创建Udp Server");
		}

		//Prepare the sockaddr_in structure
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = INADDR_ANY;
		server.sin_port = htons(port);

		//Bind
		if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
		{
			printErrorAndExit(L"无法绑定Udp端口");
		}

		//keep listening for data
		while (true)
		{
			//clear the buffer by filling null, it might have previously received data
			memset(buf, '\0', BUFLEN);

			//try to receive some data, this is a blocking call
			if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == SOCKET_ERROR)
			{
				PostMessage(g_hHWnd, WM_RECEIVE_TIC_ERROR, NULL, (LPARAM)"无法读取TIC数据");
				continue;
			}

			if (buf[0] == MSG_TIC)
			{
				char ip_buf[20]; // 使用 inet_ntop 而不是 inet_ntoa 来保证安全
				inet_ntop(AF_INET, &si_other.sin_addr, ip_buf, sizeof(ip_buf));
				handleTic(buf + 1, ip_buf);
			}
		}

		closesocket(s);
	}

	void sendTocTo(const char *ip)
	{
		char bufferOut[128];
		bufferOut[0] = MSG_TOC;
		int res = gethostname(bufferOut + 1, 127);
		assert(res == 0);

		SendingManager::send(ip, MESSAGE_RECV_PORT, bufferOut, strlen(bufferOut));
	}

	void sendBroadcastError(const char* msg)
	{
		SendMessage(g_hHWnd, WM_BROADCAST_TIC_ERROR, NULL, (LPARAM)msg);
	}

	void broadcastTic(const int port)
	{
		SOCKET sock;
		if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
		{
			return sendBroadcastError("刷新失败，无法创建广播 socket");
		}

		int ret;

		//设置套接字为广播类型,允许发送广播消息
		bool so_broadcast = true;
		ret = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&so_broadcast, sizeof(so_broadcast));
		if (ret < 0) 
		{
			closesocket(sock);
			return sendBroadcastError("刷新失败，setsockopt SO_SNDBUF error");
		}

		//设置套接字 发送缓冲区2K
		const int nSendBuf = 2 * 1024;
		ret = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char *)&nSendBuf, sizeof(int));
		if (ret < 0)
		{
			closesocket(sock);
			return sendBroadcastError("刷新失败，setsockopt SO_SNDBUF error");
		}

		char bufferOut[128];
		bufferOut[0] = MSG_TIC;
		ret = gethostname(bufferOut + 1, 127);
		assert(ret == 0);

		struct sockaddr_in addr;
		addr.sin_port = port;
		addr.sin_family = AF_INET;
		inet_pton(AF_INET, "255.255.255.255", (void *)&(addr.sin_addr)); // 广播消息
		int addr_len = sizeof(addr);
		ret = sendto(sock, bufferOut, strlen(bufferOut), 0, (struct sockaddr *)&addr, addr_len);
		if (ret == SOCKET_ERROR) 
		{
			sendBroadcastError("刷新失败，send error");
		}
		closesocket(sock);
	}

}