/// <summary>
/// 关于文件发送接收相关的代码，代码不会开启新的线程
/// </summary>

#include <string>

namespace inner_network
{
	/// <summary>
	/// 给某个ip发送path指定的文件（文件必须有访问权限）
	/// </summary>
	void sendFileToIp(const std::wstring &filePath, const char* distIp);

	void handleFileMessage(const std::string &message);
}