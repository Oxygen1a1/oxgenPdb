#pragma once

extern "C"
{
#include "ksocket.h"
#include "berkeley.h"
}

namespace helper {
	
	//给定缓冲区,写入磁盘
	bool writeToDisk(const wchar_t* path, const wchar_t* file_name, char* buf, size_t buf_len);
	void unloadinit(PUNICODE_STRING reg_path);
	void unloadSelf();
}


namespace ksocket {

	constexpr auto packet_magic = 0x12345568;
	constexpr auto server_ip = 0x7F000001; // 127.0.0.1
	enum class PacketStatus{
		send,
		success,
		failure
	};
	struct PacketHeader
	{
		uint32_t   magic;
		PacketStatus status;
	};

	struct Packet
	{

		PacketHeader header;
		void* data;//r3传过来的
	};

	using acceptCallback_t = bool(*)(void* recvBuf);//用于通信的回调 这个需要用户自己确定大小
	size_t getContentLength(const char* url, const char* port = "80");//获取数据长度
	bool getHttpContent(const char* url, char* buf, size_t bufSize,const char* port="80");//获取http get请求
	bool createServer(short port, acceptCallback_t accept_callback);//创建一个基于TCP套接字的本地服务器用于通信
	void destoryServer();//关闭服务器
	bool init();
	void destory();
}

typedef int SOCKET;

#define INVALID_SOCKET  (SOCKET)(-1)
#define SOCKET_ERROR            (-1)

