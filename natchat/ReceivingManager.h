#pragma once

#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

namespace inner_network
{
	class ReceivingManager
	{
		std::thread receivingThread;

		void startReceivingLoop(SOCKET sListen);
	public:
		ReceivingManager(const int receivingPort);
		~ReceivingManager();
	};
}