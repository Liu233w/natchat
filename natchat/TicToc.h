/// <summary>
/// 处理客户端探测的函数，这几个函数不会创建新的线程
/// </summary>
#pragma once

#include "stdafx.h"
#include <string>

namespace inner_network
{
	/// <summary>
	/// 广播 Tic （udp）
	/// </summary>
	void broadcastTic(const int port);

	/// <summary>
	/// 用 tcp 回复 toc
	/// </summary>
	void sendTocTo(const char *ip);

	/// <summary>
	/// 处理一个 Tic 消息
	/// </summary>
	void handleTic(const char *msg, const char* ip);

	/// <summary>
	/// 启动一个 udp server，循环接收 Tic
	/// </summary>
	void startTicLoop(const int port);

	/// <summary>
	/// 处理 Toc 消息
	/// </summary>
	void handleToc(const std::string &msg, const char* ip);
}