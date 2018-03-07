#include "stdafx.h"
#include "SendingManager.h"
#include "SocketException.h"
#include "Resource.h"

namespace inner_network
{
	void SendingManager::startLoop()
	{
		while (true)
		{
			std::shared_ptr<ConcurrentSender> ptr;
			{
				// 锁定线程共享的队列从而取出一个 Sender
				std::unique_lock<std::mutex> lk(this->mut);
				this->dataCond.wait(lk, [this] {return !this->sendingQueue.empty(); });
				ptr = this->sendingQueue.front();
				this->sendingQueue.pop();
			}
			// 解锁之后再执行 Sender，防止在执行时锁定队列并阻塞主线程
			try
			{
				ptr->waitDone();
				// 只有传输的是文件的时候才会发送通知
				if (ptr->isSendingFile())
				{
					PostMessage(AfxGetMainWnd()->m_hWnd, IDC_SEND_FILE_DONE, 0, 0);
				}
			}
			catch (SocketException& e) 
			{
				auto msg = (LPARAM)e.what();
				if (ptr->isSendingFile())
				{
					// 防止引用失效，在发送此消息的时候阻塞线程，避免释放 SocketException 中的 string 内存
					SendMessage(AfxGetMainWnd()->m_hWnd, IDC_SEND_FILE_ERROR, 0, msg);
				}
				else
				{
					SendMessage(AfxGetMainWnd()->m_hWnd, IDC_SEND_MESSAGE_ERROR, 0, msg);
				}
			}
		}

	}

	void SendingManager::send(const char * address, int port, const char * buffer, size_t bufferSize)
	{
		std::lock_guard<std::mutex> lk(this->mut);
		this->sendingQueue.emplace(address, port, buffer, bufferSize);
		this->dataCond.notify_one();
	}

}