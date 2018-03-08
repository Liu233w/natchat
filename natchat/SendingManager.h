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
	/// 管理Socket消息发送
	/// </summary>
	namespace SendingManager
	{
		/// <summary>
		/// 用来处理消息发送和队列管理的线程
		/// </summary>
		static std::thread sendingThread;

		static std::mutex mut;
		static std::queue<std::shared_ptr<ConcurrentSender>> sendingQueue;
		static std::condition_variable dataCond;

		void startLoop();

		static void initSendingManager()
		{
			sendingThread = std::thread(startLoop);
			sendingThread.detach();
		}

		/// <summary>
		/// 向指定目标发送数据（加入待发送队列）
		/// </summary>
		/// <param name="address"></param>
		/// <param name="port"></param>
		/// <param name="buffer"></param>
		/// <param name="bufferSize"></param>
		void send(const std::string address, int port, const char* buffer, size_t bufferSize, bool isFile = false);
	};

}