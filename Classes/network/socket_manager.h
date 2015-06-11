#ifndef _SOCKET_MANAGER_H_
#define _SOCKET_MANAGER_H_
#include <functional>

#include "ring_buffer.h"
#include "google/protobuf/message.h"

class BaseSock;

class SocketManager
{
public:
	~SocketManager();
	void sendMessage(unsigned int opcode, google::protobuf::Message& msg);

	static SocketManager* getInstance();

	bool connect(const char* ip, const int port);

	void startThread();

	void SetEventCallback(std::function<int(unsigned int, const char*)>& func);
	int OnEventCallback(unsigned int sz, const char* buffer);

private:
	SocketManager();

	void bufferHandle();
	void recvHandle();

	void parseRecvBuffer();
	void parseSendBuffer();

	unsigned int bitExtract(unsigned int size, int pos, int len);
	char* pbmsgPack(const unsigned int opcode);
	char* pbmsgPack(const unsigned short len, const unsigned int opcode, const google::protobuf::Message& msg);
	void pbmsgUnpack(const char* buf, unsigned short& len, unsigned int& opcode, google::protobuf::Message& msg);
	void pbmsgUnpack(const char* buf);

	BaseSock* m_sock;
	RingBuffer<char *> *m_send_buffer;
	RingBuffer<char *> *m_recv_buffer;

	static SocketManager* m_mgr;

	// function callback
	std::function<int(unsigned int, const char*)> m_eventcallback;
};

#endif