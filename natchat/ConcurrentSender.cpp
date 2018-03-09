#include "stdafx.h"
#include "ConcurrentSender.h"
#include "ChatService.h"

#include <cstdlib>
#include <chrono>

namespace inner_network
{

	ConcurrentSender::ConcurrentSender(const char * address, const int port, const char * buffer, 
		const size_t bufferSize, bool isSendingFile)
		:address(address), port(port), bufferSize(bufferSize), _isSendingFile(isSendingFile)
	{
		this->buffer = new char[bufferSize];

		memcpy(this->buffer, buffer, bufferSize);

		this->future = std::async(&ConcurrentSender::startSend, this);
	}

	ConcurrentSender::~ConcurrentSender()
	{
		// this->future.wait();
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

	bool ConcurrentSender::waitDone()
	{
		return this->future.get();
	}

	bool ConcurrentSender::startSend()
	{
		WSADATA wsdata;
		//Æô¶¯SOCKET¿â£¬°æ±¾Îª2.0  
		WSAStartup(0x0202, &wsdata);

		SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sclient == INVALID_SOCKET)
			return false;
		sockaddr_in serAddr;
		serAddr.sin_family = AF_INET;
		serAddr.sin_port = htons(this->port);
		inet_pton(AF_INET, this->address.c_str(), (void *)&(serAddr.sin_addr));
		if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
		{
			return false;
		}

		send(sclient, this->buffer, this->bufferSize, 0);
		send(sclient, MESSAGE_END, strlen(MESSAGE_END), 0);
		
		closesocket(sclient);
		return true;
	}

}