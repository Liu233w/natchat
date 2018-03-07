#pragma once

#include <future>

namespace inner_network
{
	/// <summary>
	/// 在另一个线程发送buffer
	/// </summary>
	class ConcurrentSender
	{
	public:
		/// <summary>
		/// 初始化对象并在另一个线程中立即开始发送数据
		/// </summary>
		/// <param name="address">目标ip地址</param>
		/// <param name="port">目标端口</param>
		/// <param name="buffer">要发送的数据（一次性发完）</param>
		/// <param name="bufferSize">发送数据的大小</param>
		/// <param name="isSendingFile">发送的是否是文件</param>
		ConcurrentSender(const char* address, const int port, const char* buffer, const size_t bufferSize,
			bool isSendingFile = false);
		~ConcurrentSender();

		/// <summary>
		/// 检查是否发送完成
		/// </summary>
		/// <returns>完成返回true，否则false</returns>
		bool checkDone();

		/// <summary>
		/// 阻塞当前线程，直到完成传输。只能调用一次，否则抛出异常
		/// </summary>
		void waitDone();

		/// <summary>
		/// 当前传输的是否是文件
		/// </summary>
		/// <returns></returns>
		bool isSendingFile()
		{
			return this->isSendingFile;
		}
	private:
		char* buffer;
		size_t bufferSize;
		std::future<bool> future;
		std::string address;
		int port;
		bool isSendingFile;

		bool startSend();
	};

}