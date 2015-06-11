#include "socket_manager.h"
#include "base_sock.h"
#include "ring_buffer.h"
#include <thread>
#include <stdio.h>
#include "proto_message.h"
#include "message_dispatcher.h"

#define DEFAULT_BUFF_SIZE (10240)

SocketManager* SocketManager::m_mgr = nullptr;

SocketManager* SocketManager::getInstance()
{
	if (m_mgr == nullptr)
	{
		m_mgr = new SocketManager();
	}

	return m_mgr;
}

SocketManager::SocketManager()
{
	m_recv_buffer = new RingBuffer<char *>(DEFAULT_BUFF_SIZE);
	m_send_buffer = new RingBuffer<char *>(DEFAULT_BUFF_SIZE);
}

SocketManager::~SocketManager()
{

}

bool SocketManager::connect(const char* ip, const int port)
{
	InitSocketSystem();
	m_sock = new BaseSock();
	m_sock->Create(false);
	if (!m_sock->Connect(ip, port))
	{
		printf("connect failed.\n");
		return false;
	}

	printf("---connect success---\n");
	return true;
}

void SocketManager::sendMessage(unsigned int opcode, google::protobuf::Message& msg)
{
	char* buf = pbmsgPack(msg.ByteSize(), opcode, msg);
	if (m_send_buffer->avaliable())
	{
		m_send_buffer->push(buf);
	}
}

void SocketManager::startThread()
{
	std::thread t1(&SocketManager::bufferHandle, m_mgr);
	t1.detach();

	std::thread t2(&SocketManager::recvHandle, m_mgr);
	t2.detach();
}

//处理两个消息队列
void SocketManager::bufferHandle()
{
	static int n = 0;
	while (1)
	{
		printf("in thread function ... fuck %d times\n", ++n);
		Sleep(2000);
		parseRecvBuffer();
		parseSendBuffer();
	}
}

//接收服务器消息
void SocketManager::recvHandle()
{
	long ret = 0;
	char buf[1024];
	memset(buf, 0, 1024);
	while (1)
	{
		ret = m_sock->Recv(buf, 1024);
		if (ret > 6) //正常数据包
		{
			if (m_recv_buffer->avaliable())
			{
				unsigned short pkg_size = ((unsigned char)buf[0] << 8) | (unsigned char)buf[1];
				if (pkg_size + 2 == ret)
				{
					m_recv_buffer->push(buf);
				}
			}
		}
	}
}

void SocketManager::parseRecvBuffer()
{
	if (m_recv_buffer->empty())
	{
		return;
	}

	char* buf = (char*)malloc(1024);
	memset(buf, 0, 1024);

	if (m_recv_buffer->pop(buf))
	{
		pbmsgUnpack(buf);
	}

}

void SocketManager::parseSendBuffer()
{
	if (m_send_buffer->empty())
	{
		return;
	}

	char* buf = (char*)malloc(1024);
	memset(buf, 0, 1024);
	if (m_send_buffer->pop(buf))
	{
		unsigned short size = ((unsigned char)buf[0] << 8) | (unsigned char)buf[1];
		size += 2;
		int ret = m_sock->Send(buf, size);
		printf("-----send %d----\n", ret);
	}
}

unsigned int SocketManager::bitExtract(unsigned int size, int pos, int len)
{
	unsigned char shift = sizeof(int) * 8 - pos - len;
	return size << shift >> pos + shift;
}

char* SocketManager::pbmsgPack(const unsigned int opcode)
{
	//消息内容
	static char* buf = (char *)malloc(2 + 4);

	//长度
	unsigned int s1 = bitExtract(6, 8, 8);
	unsigned int s2 = bitExtract(6, 0, 8);
	memset(buf, s1, sizeof(char));
	memset(buf + 1, s2, sizeof(char));

	//消息号
	unsigned int o1 = bitExtract(opcode, 24, 8);
	unsigned int o2 = bitExtract(opcode, 16, 8);
	unsigned int o3 = bitExtract(opcode, 8, 8);
	unsigned int o4 = bitExtract(opcode, 0, 8);
	memset(buf + 2, o1, sizeof(char));
	memset(buf + 3, o2, sizeof(char));
	memset(buf + 4, o3, sizeof(char));
	memset(buf + 5, o4, sizeof(char));

	return buf;
}

char* SocketManager::pbmsgPack(const unsigned short size, const unsigned int opcode, const google::protobuf::Message& msg)
{
	//消息内容
	static char* buf = (char *)malloc(size + 2 + 4);
	memset(buf, 0, size + 8);
	msg.SerializeToArray(buf + 2 + 4, size);

	//长度
	unsigned int s1 = bitExtract(size + 6, 8, 8);
	unsigned int s2 = bitExtract(size + 6, 0, 8);
	memset(buf, s1, sizeof(char));
	memset(buf + 1, s2, sizeof(char));

	//消息号
	unsigned int o1 = bitExtract(opcode, 24, 8);
	unsigned int o2 = bitExtract(opcode, 16, 8);
	unsigned int o3 = bitExtract(opcode, 8, 8);
	unsigned int o4 = bitExtract(opcode, 0, 8);
	memset(buf + 2, o1, sizeof(char));
	memset(buf + 3, o2, sizeof(char));
	memset(buf + 4, o3, sizeof(char));
	memset(buf + 5, o4, sizeof(char));

	return buf;
}

void SocketManager::pbmsgUnpack(const char* buf, unsigned short& size, unsigned int& opcode, google::protobuf::Message& msg)
{
	size = ((unsigned char)buf[0] << 8) | (unsigned char)buf[1];
	opcode = ((unsigned char)buf[2] << 24) | ((unsigned char)buf[2] << 16) | ((unsigned char)buf[4] << 8) | (unsigned char)buf[5];
	msg.ParseFromArray(buf + 6, size - 4);
}

void SocketManager::pbmsgUnpack(const char* buf)
{
	unsigned short size = ((unsigned char)buf[0] << 8) | (unsigned char)buf[1];
	unsigned int opcode = ((unsigned char)buf[2] << 24) | ((unsigned char)buf[3] << 16) | ((unsigned char)buf[4] << 8) | (unsigned char)buf[5];

	printf("---- len : %d\n", size);
	printf("--- opcode:%d\n", opcode);
	unsigned int module = opcode >> 16;
	unsigned int msgno = opcode & 0x0000FFFF;
	printf("---- %d  %d ---\n", module, msgno);
	buf = buf + 6;
	size -= 4;

	MessageDispatcher::getInstance()->dispatchMessage(opcode, buf, size);
}

void SocketManager::SetEventCallback(std::function<int(unsigned int, const char*)>& func)
{
	m_eventcallback = func;
}

int SocketManager::OnEventCallback(unsigned int sz, const char* buffer)
{
	if (m_eventcallback)
	{
		return m_eventcallback(sz, buffer);
	}

	return 0;
}