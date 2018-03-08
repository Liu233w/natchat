/// <summary>
/// 处理客户端探测的函数，这几个函数不会创建新的线程
/// </summary>
#pragma once

#include "stdafx.h"
#include <string>

namespace inner_network
{
	void broadcastTic();

	void sendTocTo(const char *ip);

	void handleTic(std::string msg, const char* ip);
}