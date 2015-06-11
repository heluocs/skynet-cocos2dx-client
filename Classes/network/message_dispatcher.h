#ifndef _MESSAGE_DISPATCHER_H_
#define _MESSAGE_DISPATCHER_H_

#include <map>

class BaseController;

class MessageDispatcher
{
public:
	static MessageDispatcher* getInstance();

	void registController(unsigned int moduleno, BaseController * controller);
	void dispatchMessage(const unsigned int opcode, const char * buf, const unsigned short size);

private:
	MessageDispatcher();

	static MessageDispatcher* m_dispatcher;

	std::map<unsigned int, BaseController *> m_Controllers;
};

#endif