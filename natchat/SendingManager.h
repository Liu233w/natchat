#pragma once

#include <thread>
#include <mutex>  
#include <condition_variable>  
#include <memory>
#include <queue>

#include "ConcurrentSender.h"

namespace inner_network
{
	/// <summary>
	/// 管理Socket消息发送的类
	/// </summary>
	class SendingManager
	{
	private:
		/// <summary>
		/// 用来处理消息发送和队列管理的线程
		/// </summary>
		std::thread sendingThread;

		mutable std::mutex mut;
		std::queue<std::shared_ptr<ConcurrentSender>> sendingQueue;
		std::condition_variable dataCond;

		void startLoop();

	public:
		SendingManager()
			:sendingThread(&SendingManager::startLoop, this)
		{
		}
		~SendingManager() {}

		/// <summary>
		/// 向指定目标发送数据（加入待发送队列）
		/// </summary>
		/// <param name="address"></param>
		/// <param name="port"></param>
		/// <param name="buffer"></param>
		/// <param name="bufferSize"></param>
		void send(const char* address, int port, const char* buffer, size_t bufferSize);
	};

}