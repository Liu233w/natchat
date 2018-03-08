#include "stdafx.h"
#include "File.h"
#include "ChatService.h"
#include "MessageHeader.h"

#include <fstream>
#include <sstream>
#include <string>
#include <locale>
#include <codecvt>

// 声明的全局变量（跨文件）
std::wstring g_wsSaveFilePath;

namespace inner_network
{
	std::string getFileNameFromPath(const std::wstring &filePath)
	{
		auto idx = filePath.find_last_of(L'\\');
		auto fileName = filePath.substr(idx + 1);

		// 用 utf-8 编码转换
		using convert_type = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_type, wchar_t> converter;

		return converter.to_bytes(fileName);
	}

	void sendFileToIp(const std::wstring &filePath, const char* distIp)
	{

		std::ifstream fin(filePath, std::ios::in | std::ios::binary);
		std::ostringstream os;

		os << MSG_FILE << getFileNameFromPath(filePath) << "\r" << fin.rdbuf();

		std::string buf = os.str();

		SendingManager::send(distIp, MESSAGE_RECV_PORT, buf.c_str(), buf.size(), true);
	}

	void handleFileMessage(const std::string & message)
	{
		size_t fileNameEnd = message.find_first_of('\r');
		std::string fileName = message.substr(1, fileNameEnd - 1);

		// 用 utf-8 编码转换
		using convert_type = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_type, wchar_t> converter;

		std::wstring wFileName = converter.from_bytes(fileName);

		// 阻塞发送消息，这样就可以在主消息循环处理完文件位置之后直接返回并保存文件了。
		SendMessage(g_hHWnd, WM_RECEIVE_FILE, NULL, (LPARAM)wFileName.c_str());

		std::ofstream out(g_wsSaveFilePath, std::ios::out | std::ios::binary);
		out << message.substr(fileNameEnd + 1);
	}

}