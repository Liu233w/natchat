#pragma once

// 本文件存储发送消息的消息头，用来区分消息类型

namespace inner_network
{
	constexpr char MSG_REGULAR = 0x01;
	constexpr char MSG_FILE = 0x02;
	constexpr char MSG_PRIVATE = 0x03;

	// 下面的两个消息用于客户端检测中
	constexpr char MSG_TIC = 0x11; // 点击刷新按钮的客户端群发此消息
	constexpr char MSG_TOC = 0x12; // 收到上个消息的客户端向目标发送此消息
	constexpr char MSG_BYE = 0x13; // 退出时给其他客户端发送此消息
}