#include "stdafx.h"
#include "SendingManager.h"
#include "Resource.h"
#include "ChatService.h"

namespace inner_network
{
	void SendingManager::startLoop()
	{
		while (true)
		{
			std::shared_ptr<ConcurrentSender> ptr;
			{
				// 锁定线程共享的队列从而取出一个 Sender
				std::unique_lock<std::mutex> lk(SendingManager::mut);
				SendingManager::dataCond.wait(lk, [&] {return !SendingManager::sendingQueue.empty(); });
				ptr = SendingManager::sendingQueue.front();
				SendingManager::sendingQueue.pop();
			}
			// 解锁之后再执行 Sender，防止在执行时锁定队列并阻塞主线程
			bool success = ptr->waitDone();
			if (success)
			{
				// 只有传输的是文件的时候才会发送通知
				if (ptr->isSendingFile())
				{
					PostMessage(g_hHWnd, IDC_SEND_FILE_DONE, 0, 0);
				}

			}
			else
			{
				auto msg = (LPARAM)"发送失败";
				if (ptr->isSendingFile())
				{
					// 防止引用失效，在发送此消息的时候阻塞线程，避免释放 SocketException 中的 string 内存
					SendMessage(g_hHWnd, IDC_SEND_FILE_ERROR, 0, msg);
				}
				else
				{
					SendMessage(g_hHWnd, IDC_SEND_MESSAGE_ERROR, 0, msg);
				}
			}
		}

	}

	void SendingManager::send(const char * address, int port, const char * buffer, size_t bufferSize, bool isFile)
	{
		std::lock_guard<std::mutex> lk(SendingManager::mut);
		SendingManager::sendingQueue.emplace(new ConcurrentSender(address, port, buffer, bufferSize, isFile));
		SendingManager::dataCond.notify_one();
	}

}