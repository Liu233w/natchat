#include "stdafx.h"
#include "ConcurrentSender.h"

#include <cstdlib>
#include <chrono>
#include "SocketException.h"

namespace inner_network
{

	ConcurrentSender::ConcurrentSender(const char * address, const int port, const char * buffer, 
		const size_t bufferSize, bool isSendingFile = false)
		:address(address), port(port), bufferSize(bufferSize), isSendingFile(isSendingFile)
	{
		this->buffer = new char[bufferSize];

		memcpy(this->buffer, buffer, bufferSize);

		this->future = std::async(std::launch::async, &ConcurrentSender::startSend, this);
	}

	ConcurrentSender::~ConcurrentSender()
	{
		this->future.wait();
		if (this->buffer != nullptr)
		{
			delete this->buffer;
		}
	}

	bool ConcurrentSender::checkDone()
	{
		using namespace std::chrono_literals;
		auto status = this->future.wait_for(0ms);

		return status == std::future_status::ready;
	}

	void ConcurrentSender::waitDone()
	{
		bool success = this->future.get();
		if (!success)
		{
			throw SocketException("无法发送消息");
		}
	}

	bool ConcurrentSender::startSend()
	{
		SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sclient == INVALID_SOCKET)
			return false;
		sockaddr_in serAddr;
		serAddr.sin_family = AF_INET;
		serAddr.sin_port = htons(this->port);
		serAddr.sin_addr.S_un.S_addr = inet_addr(this->address.c_str());
		if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
		{
			return false;
		}

		send(sclient, this->buffer, this->bufferSize, 0);
		send(sclient, "\f", 1, 0);
		
		closesocket(sclient);
	}

}