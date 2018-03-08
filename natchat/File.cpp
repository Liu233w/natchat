#include "stdafx.h"
#include "File.h"
#include "ChatService.h"
#include "MessageHeader.h"

#include <fstream>
#include <sstream>

namespace inner_network
{
	void sendFileToIp(const wchar_t* filePath, const char* distIp)
	{
		std::ifstream fin(filePath, std::ios::in | std::ios::binary);
		std::ostringstream os;

		os << MSG_FILE << fin.rdbuf();

		std::string buf = os.str();

		l_pSendingManager->send(distIp, MESSAGE_RECV_PORT, buf.c_str(), buf.size(), true);
	}

}